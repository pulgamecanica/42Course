# frozen_string_literal: true

require 'pg'
require_relative '../../config/database'

class BaseRecord
  class << self
    # ------------ CONFIGURATION ------------------------------------------
    # overriding this is optional; by default table = underscored class name
    def table_name
      @table_name ||= name.gsub(/(.)([A-Z])/, '\1_\2').downcase + 's'
    end

    # columns cached on first touch so we don't hit pg every call
    def columns
      @columns ||= Database.with_conn { |c| c.exec("SELECT * FROM #{table_name} LIMIT 0").fields }
    end

    # ------------ CRUD ---------------------------------------------------
    def create(attrs)
      check!(attrs.keys)
      keys   = attrs.keys
      values = attrs.values
      idx    = (1..keys.size).map { |i| "$#{i}" }.join(', ')

      sql = %(
        INSERT INTO #{table_name} (#{keys.join(', ')})
        VALUES (#{idx})
        RETURNING *
      )
      record_from_sql(sql, values)
    end

    def find_by(criteria)
      where_sql, values = where_clause(criteria)
      sql = "SELECT * FROM #{table_name} #{where_sql} LIMIT 1"
      record_from_sql(sql, values)
    end

    def find_by_id(id) = find_by(id: id)

    def create_or_find(attrs)
      find_by(attrs) || create(attrs)
    end

    def delete_by(criteria)
      where_sql, values = where_clause(criteria)
      sql = "DELETE FROM #{table_name} #{where_sql} RETURNING *"
      record_from_sql(sql, values)
    end

    def delete_by_id(id) = delete_by(id: id)

    def update_by(criteria, updates)
      check!(updates.keys)
      where_sql, where_vals   = where_clause(criteria)
      set_sql,   update_vals  = set_clause(updates)
      sql = %(
        UPDATE #{table_name}
        #{set_sql}
        #{where_sql}
        RETURNING *
      )
      record_from_sql(sql, update_vals + where_vals)
    end

    def update_by_id(id, updates) = update_by({ id: id }, updates)

    # ------------ INTERNAL HELPERS --------------------------------------
    private

    def check!(keys)
      unknown = keys - columns
      return if unknown.empty?

      raise ArgumentError, "Unknown columns: #{unknown.join(', ')}"
    end

    def where_clause(hash)
      return ['', []] if hash.empty?
      keys   = hash.keys
      values = hash.values
      conds  = keys.map.with_index { |k, i| "#{k} = $#{i + 1}" }
      ["WHERE #{conds.join(' AND ')}", values]
    end

    def set_clause(hash)
      keys   = hash.keys
      values = hash.values
      sets   = keys.map.with_index { |k, i| "#{k} = $#{i + 1}" }
      ["SET #{sets.join(', ')}", values]
    end

    def record_from_sql(sql, params)
      Database.with_conn { |c| c.exec_params(sql, params).first }
    end
  end
end
