# frozen_string_literal: true

require_relative '../helpers/database'

class BlockedUser
  def self.block!(blocker_id, blocked_id)
    raise Errors::ValidationError, 'You cannot block yourself' if blocker_id == blocked_id

    Database.with_conn do |conn|
      conn.exec_params(
        'INSERT INTO blocked_users (blocker_id, blocked_id) VALUES ($1, $2) ON CONFLICT DO NOTHING',
        [blocker_id, blocked_id]
      )
    end
  end

  def self.unblock!(blocker_id, blocked_id)
    Database.with_conn do |conn|
      conn.exec_params(
        'DELETE FROM blocked_users WHERE blocker_id = $1 AND blocked_id = $2',
        [blocker_id, blocked_id]
      )
    end
  end

  def self.blocked_users_for(user_id)
    Database.with_conn do |conn|
      res = conn.exec_params(<<~SQL, [user_id])
        SELECT users.* FROM users
        JOIN blocked_users ON users.id = blocked_users.blocked_id
        WHERE blocked_users.blocker_id = $1
      SQL
      res.map { |user| UserSerializer.public_view(user) }
    end
  end

  def self.blocked_by(user_id)
    Database.with_conn do |conn|
      res = conn.exec_params(<<~SQL, [user_id])
        SELECT users.* FROM users
        JOIN blocked_users ON users.id = blocked_users.blocker_id
        WHERE blocked_users.blocked_id = $1
      SQL
      res.map { |user| UserSerializer.public_view(user) }
    end
  end

  def self.blocked?(blocker_id, blocked_id)
    Database.with_conn do |conn|
      res = conn.exec_params(
        'SELECT 1 FROM blocked_users WHERE blocker_id = $1 AND blocked_id = $2 LIMIT 1',
        [blocker_id, blocked_id]
      )
      !res.to_a.empty?
    end
  end
end
