# frozen_string_literal: true

require_relative '../../app/helpers/database'

Database.with_open_conn do |conn|
  conn.exec <<~SQL
    CREATE TABLE IF NOT EXISTS likes (
      liker_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
      liked_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
      created_at TIMESTAMP DEFAULT NOW(),
      PRIMARY KEY (liker_id, liked_id)
    );
  SQL
  conn.close
end
