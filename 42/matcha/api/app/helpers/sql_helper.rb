# frozen_string_literal: true

require_relative '../helpers/database'

module SQLHelper
  def self.parse_unique_violation(error)
    if error.message =~ /unique constraint "(.*?)"/
      constraint = Regexp.last_match(1)

      case constraint
      when /users_(\w+)_key/
        field = Regexp.last_match(1)
        ["#{field} is already taken"]
      else
        ['Unique constraint violation']
      end
    else
      ['Unique constraint violation']
    end
  end

  def self.singularize(word)
    return word if word.end_with?('ss')

    word.sub(/s$/, '')
  end

  def self.pluralize(word)
    return word if word.end_with?('s')

    "#{word}s"
  end

  def self.table_exists?(table)
    Database.with_conn do |conn|
      sql = 'SELECT to_regclass($1) IS NOT NULL AS exists'
      result = conn.exec_params(sql, [table])
      result&.first&.[]('exists') == 't'
    end
  end

  def self.build_update_set(fields, allowed_fields)
    set_fragments = []
    values = []

    normalized_fields = fields.transform_keys(&:to_s)

    normalized_fields.each_with_index do |(key, value), index|
      next unless allowed_fields.include?(key)

      set_fragments << "#{key} = $#{index + 1}"
      values << value
    end

    [set_fragments.join(', '), values]
  end

  def self.create(table, fields, allowed_fields)
    Database.with_conn do |conn|
      normalized_fields = fields.transform_keys(&:to_s)
      filtered_fields = normalized_fields.select { |k, _| allowed_fields.include?(k) }
      keys = filtered_fields.keys
      values = filtered_fields.values
      placeholders = keys.each_index.map { |i| "$#{i + 1}" }

      sql = <<~SQL
        INSERT INTO #{table} (#{keys.join(', ')})
        VALUES (#{placeholders.join(', ')})
        RETURNING *
      SQL

      res = conn.exec_params(sql, values)
      res&.first
    end
  end

  def self.update(table, id, fields, allowed_fields)
    return find_by_id(table, id) if fields.empty?

    Database.with_conn do |conn|
      set_clause, values = build_update_set(fields, allowed_fields)
      sql = <<~SQL
        UPDATE #{table}
        SET #{set_clause}, updated_at = NOW()
        WHERE id = $#{values.size + 1}
        RETURNING *
      SQL

      values << id
      begin
        res = conn.exec_params(sql, values)
        res&.first
      rescue PG::UniqueViolation => e
        raise Errors::ValidationError.new('Validation failed', parse_unique_violation(e))
      end
    end
  end

  def self.update_column(table, column, value, conditions)
    Database.with_conn do |conn|
      condition_sql = conditions.keys.each_with_index.map { |k, i| "#{k} = $#{i + 2}" }.join(' AND ')
      sql = "UPDATE #{table} SET #{column} = $1, updated_at = NOW() WHERE #{condition_sql}"
      values = [value] + conditions.values
      begin
        conn.exec_params(sql, values)
      rescue PG::UniqueViolation => e
        raise Errors::ValidationError.new('Validation failed', parse_unique_violation(e))
      end
    end
  end

  def self.find_by(table, field, value)
    Database.with_conn do |conn|
      sql = "SELECT * FROM #{table} WHERE #{field} = $1 LIMIT 1"
      conn.exec_params(sql, [value])&.first
    end
  end

  def self.find_by_id(table, id)
    Database.with_conn do |conn|
      res = conn.exec_params("SELECT * FROM #{table} WHERE id = $1", [id])
      res&.first
    end
  end

  def self.find_many_by_ids(table, ids)
    return [] if ids.empty?

    Database.with_conn do |conn|
      placeholders = ids.each_index.map { |i| "$#{i + 1}" }.join(', ')
      sql = "SELECT * FROM #{table} WHERE id IN (#{placeholders})"

      res = conn.exec_params(sql, ids)
      res.to_a
    end
  end

  def self.delete(table, id)
    Database.with_conn do |conn|
      conn.exec_params("DELETE FROM #{table} WHERE id = $1", [id])
    rescue PG::UniqueViolation => e
      raise Errors::ValidationError.new('Validation failed', parse_unique_violation(e))
    end
  end

  def self.many_to_many(source_sym, target_sym, source_id)
    Database.with_conn do |conn|
      source      = source_sym.to_s
      target      = target_sym.to_s
      join_table  = "#{source}_#{target}"
      join_table  = "#{target}_#{source}" unless table_exists?(join_table)

      source_id_column = "#{source}_id"
      target_id_column = "#{singularize(target)}_id"

      sql = <<~SQL
        SELECT #{target}.* FROM #{target}
        JOIN #{join_table} ON #{join_table}.#{target_id_column} = #{target}.id
        WHERE #{join_table}.#{source_id_column} = $1
      SQL

      conn.exec_params(sql, [source_id]).to_a
    end
  end
end
