# frozen_string_literal: true

require_relative '../../config/database'

Database.with_open_conn do |conn|
  conn.exec <<~SQL
    CREATE TABLE IF NOT EXISTS identities (
      id SERIAL PRIMARY KEY,
      user_id INTEGER NOT NULL REFERENCES users(id),
      provider VARCHAR(50) NOT NULL,
      uid VARCHAR(255) NOT NULL,
      access_token VARCHAR(255),
      expires_at TIMESTAMP,
      info JSONB,
      created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
      updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
      CONSTRAINT uniq_provider_uid UNIQUE (provider, uid)
    );
    CREATE INDEX IF NOT EXISTS idx_identities_user_id ON identities(user_id);
  SQL
  conn.close
end
