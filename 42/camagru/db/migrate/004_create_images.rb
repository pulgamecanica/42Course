# frozen_string_literal: true

require_relative '../../config/database'

Database.with_open_conn do |conn|
  conn.exec <<~SQL
    CREATE TABLE IF NOT EXISTS images (
      id SERIAL PRIMARY KEY,
      user_id INTEGER NOT NULL REFERENCES users(id),
      file_path VARCHAR(255) NOT NULL,
      status VARCHAR(25) NOT NULL DEFAULT 'pending',
      processed_at TIMESTAMP,
      width INTEGER,
      height INTEGER,
      format VARCHAR(25),
      feed_order INTEGER,
      metadata JSONB,
      created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
      updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
    CREATE UNIQUE INDEX IF NOT EXISTS idx_images_user_feed_order ON images(user_id, feed_order);
    CREATE INDEX IF NOT EXISTS idx_images_user_id ON images(user_id);
  SQL
  conn.close
end
