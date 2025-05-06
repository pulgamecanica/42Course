# frozen_string_literal: true

require_relative '../helpers/database'
require_relative '../helpers/user_serializer'

class Connection
  def self.all_for_user(user_id)
    Database.with_conn do |conn|
      res = conn.exec_params(<<~SQL, [user_id])
        SELECT users.* FROM connections
        JOIN users
          ON users.id = CASE
            WHEN connections.user_a_id = $1 THEN connections.user_b_id
            ELSE connections.user_a_id
          END
        WHERE connections.user_a_id = $1 OR connections.user_b_id = $1
        ORDER BY connections.created_at DESC
      SQL
      res.map { |user| UserSerializer.public_view(user) }
    end
  end

  def self.find_by_id(id)
    Database.with_conn do |conn|
      res = conn.exec_params('SELECT * FROM connections WHERE id = $1', [id])
      res.to_a&.first
    end
  end

  def self.find_between(user_a_id, user_b_id)
    Database.with_conn do |conn|
      res = conn.exec_params(<<~SQL, [user_a_id, user_b_id, user_b_id, user_a_id])
        SELECT * FROM connections
        WHERE (user_a_id = $1 AND user_b_id = $2)
           OR (user_a_id = $3 AND user_b_id = $4)
        LIMIT 1
      SQL
      res.to_a&.first
    end
  end

  def self.create(user_a_id, user_b_id)
    return if user_a_id == user_b_id
    return if find_between(user_a_id, user_b_id)

    connection = Database.with_conn do |conn|
      conn.exec_params(<<~SQL, [user_a_id, user_b_id])
        INSERT INTO connections (user_a_id, user_b_id, created_at)
        VALUES ($1, $2, NOW())
        RETURNING *
      SQL
    end&.to_a&.first
    User.update_fame!(user_a_id)
    User.update_fame!(user_b_id)
    connection
  end

  def self.delete_between(user_a_id, user_b_id)
    Database.with_conn do |conn|
      conn.exec_params(<<~SQL, [user_a_id, user_b_id, user_b_id, user_a_id])
        DELETE FROM connections
        WHERE (user_a_id = $1 AND user_b_id = $2)
           OR (user_a_id = $3 AND user_b_id = $4)
      SQL
    end
    User.update_fame!(user_a_id)
    User.update_fame!(user_b_id)
  end
end
