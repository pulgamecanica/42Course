# frozen_string_literal: true

require_relative '../helpers/database'

class Message
  def self.create(connection_id, sender_id, content)
    Database.with_conn do |conn|
      res = conn.exec_params(<<~SQL, [connection_id, sender_id, content])
        INSERT INTO messages (connection_id, sender_id, content, created_at)
        VALUES ($1, $2, $3, NOW())
        RETURNING *
      SQL
      res&.first
    end
  end

  def self.for_connection(connection_id)
    Database.with_conn do |conn|
      res = conn.exec_params(<<~SQL, [connection_id])
        SELECT messages.*, users.username AS sender_username
        FROM messages
        JOIN users ON users.id = messages.sender_id
        WHERE messages.connection_id = $1
        ORDER BY messages.created_at ASC
      SQL
      res.to_a
    end
  end
end
