# frozen_string_literal: true

require_relative '../../config/database'

Database.with_open_conn do |conn|
  conn.exec <<~SQL
    CREATE TABLE IF NOT EXISTS notifications (
      id SERIAL PRIMARY KEY,
      recipient_id INTEGER NOT NULL REFERENCES users(id),
      actor_id INTEGER REFERENCES users(id),
      action VARCHAR(50) NOT NULL,
      notifiable_type VARCHAR(50) NOT NULL,
      notifiable_id INTEGER NOT NULL,
      read_at TIMESTAMP,
      created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
      updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
    CREATE INDEX IF NOT EXISTS idx_notifications_recipient_id ON notifications(recipient_id);
    CREATE INDEX IF NOT EXISTS idx_notifications_notifiable ON notifications(notifiable_type, notifiable_id);
  SQL
  conn.close
end
