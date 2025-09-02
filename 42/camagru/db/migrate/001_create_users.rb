# frozen_string_literal: true

require_relative '../../config/database'

Database.with_open_conn do |conn|
  conn.exec <<~SQL
    CREATE TABLE IF NOT EXISTS users (
      id SERIAL PRIMARY KEY,
      username VARCHAR(25) UNIQUE NOT NULL,
      email VARCHAR(255) UNIQUE NOT NULL,
      password_digest VARCHAR(255) NOT NULL,
      confirmed_at TIMESTAMP,
      notifications_enabled BOOLEAN DEFAULT TRUE NOT NULL,
      created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
      updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
  SQL
  conn.close
end
