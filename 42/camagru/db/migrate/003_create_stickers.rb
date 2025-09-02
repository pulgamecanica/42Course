# frozen_string_literal: true

require_relative '../../config/database'

Database.with_open_conn do |conn|
  conn.exec <<~SQL
    CREATE TABLE IF NOT EXISTS stickers (
      id SERIAL PRIMARY KEY,
      filename VARCHAR(255) NOT NULL,
      display_name VARCHAR(255),
      width INTEGER,
      height INTEGER,
      created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
      updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
  SQL
  conn.close
end
