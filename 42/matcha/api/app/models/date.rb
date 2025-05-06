# frozen_string_literal: true

require_relative '../helpers/database'

class Date
  def self.db
    Database.pool
  end

  def self.create(connection_id, initiator_id, location, scheduled_at, note = nil)
    date = db.with do |conn|
      conn.exec_params(<<~SQL, [connection_id, initiator_id, location, scheduled_at, note])
        INSERT INTO dates (connection_id, initiator_id, location, scheduled_at, note, created_at)
        VALUES ($1, $2, $3, $4, $5, NOW())
        RETURNING *
      SQL
    end.to_a&.first
    User.update_fame!(initiator_id)
    date
  end

  def self.find_by_id(id)
    db.with do |conn|
      res = conn.exec_params('SELECT * FROM dates WHERE id = $1', [id])
      res.to_a&.first
    end
  end

  def self.all_for_user(user_id)
    db.with do |conn|
      res = conn.exec_params(<<~SQL, [user_id, user_id])
        SELECT dates.*, users.username AS partner_username FROM dates
        JOIN connections ON dates.connection_id = connections.id
        JOIN users ON users.id = CASE
          WHEN connections.user_a_id = $1 THEN connections.user_b_id
          ELSE connections.user_a_id
        END
        WHERE connections.user_a_id = $1 OR connections.user_b_id = $2
        ORDER BY scheduled_at DESC
      SQL
      res.to_a
    end
  end

  def self.delete(id)
    db.with do |conn|
      conn.exec_params('DELETE FROM dates WHERE id = $1', [id])
    end
  end
end
