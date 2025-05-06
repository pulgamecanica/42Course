# frozen_string_literal: true

require_relative '../helpers/database'
require_relative '../lib/websocket_push'

class Notification
  def self.db
    Database.pool
  end

  def self.create(user_id, message, from_user_id = nil, type = 'other')
    notification = db.with do |conn|
      conn.exec_params(<<~SQL, [user_id, from_user_id, type, message])
        INSERT INTO notifications (to_user_id, from_user_id, type, message)
        VALUES ($1, $2, $3, $4)
        RETURNING *
      SQL
    end&.first
    WebSocketPush.send_notification(user_id, notification)
    notification
  end

  def self.for_user(user_id)
    db.with do |conn|
      res = conn.exec_params(<<~SQL, [user_id])
        SELECT notifications.*, u.username AS from_username
        FROM notifications
        LEFT JOIN users u ON u.id = notifications.from_user_id
        WHERE notifications.to_user_id = $1
        ORDER BY notifications.created_at DESC
      SQL
      res.to_a
    end
  end

  def self.mark_as_read(notification_id)
    db.with do |conn|
      conn.exec_params('UPDATE notifications SET read = TRUE WHERE id = $1', [notification_id])
    end
  end

  def self.delete(id)
    db.with do |conn|
      conn.exec_params('DELETE FROM notifications WHERE id = $1', [id])
    end
  end
end
