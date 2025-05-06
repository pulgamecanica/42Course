# frozen_string_literal: true

require_relative '../../app/helpers/database'

Database.with_open_conn do |conn|
  conn.exec <<~SQL
    CREATE TABLE IF NOT EXISTS user_social_logins (
      id SERIAL PRIMARY KEY,
      user_id INTEGER REFERENCES users(id) ON DELETE CASCADE,
      provider_user_id VARCHAR(255) NOT NULL,
      provider VARCHAR(50) NOT NULL CHECK (provider IN ('github', 'google', 'intra')),
      created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
      UNIQUE (provider, provider_user_id)
    );
  SQL

  conn.close
end
