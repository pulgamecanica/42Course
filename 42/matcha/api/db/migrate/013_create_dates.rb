# frozen_string_literal: true

require_relative '../../app/helpers/database'

Database.with_open_conn do |conn|
  conn.exec <<~SQL
    CREATE TABLE IF NOT EXISTS dates (
      id SERIAL PRIMARY KEY,
      connection_id INTEGER NOT NULL REFERENCES connections(id) ON DELETE CASCADE,
      initiator_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
      location TEXT NOT NULL,
      scheduled_at TIMESTAMP NOT NULL,
      note TEXT,
      created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
  SQL
end
