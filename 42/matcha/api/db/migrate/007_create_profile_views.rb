# frozen_string_literal: true

require_relative '../../app/helpers/database'

Database.with_open_conn do |conn|
  conn.exec <<~SQL
    CREATE TABLE IF NOT EXISTS profile_views (
      id SERIAL PRIMARY KEY,
      viewer_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
      viewed_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
      visited_at TIMESTAMP NOT NULL DEFAULT NOW()
    );
  SQL
  conn.close
end
