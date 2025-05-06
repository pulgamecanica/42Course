# frozen_string_literal: true

require_relative '../../app/helpers/database'

Database.with_open_conn do |conn|
  conn.exec <<~SQL
    CREATE TABLE IF NOT EXISTS users (
      id SERIAL PRIMARY KEY,
      username VARCHAR(25) UNIQUE NOT NULL,
      email VARCHAR(255) UNIQUE NOT NULL,
      password_digest VARCHAR(255) NOT NULL,
      first_name VARCHAR(50) NOT NULL,
      last_name VARCHAR(50) NOT NULL,
      gender VARCHAR(10) CHECK (gender IN ('male', 'female', 'other')),
      sexual_preferences VARCHAR(20) CHECK (sexual_preferences IN ('male', 'female', 'non_binary', 'everyone')),
      biography TEXT,
      is_email_verified BOOLEAN DEFAULT FALSE,
      is_banned BOOLEAN DEFAULT FALSE,
      fame_rating FLOAT DEFAULT 0.0,
      birth_year INTEGER,
      latitude FLOAT,
      longitude FLOAT,
      city VARCHAR(255),
      country VARCHAR(255),
      online_status BOOLEAN DEFAULT FALSE,
      last_seen_at TIMESTAMP,
      created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
      updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
  SQL

  conn.close
end
