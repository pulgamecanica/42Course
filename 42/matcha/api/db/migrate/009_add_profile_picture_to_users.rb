# frozen_string_literal: true

require_relative '../../app/helpers/database'

Database.with_open_conn do |conn|
  constraint_exists = conn.exec(<<~SQL).any?
    SELECT 1
    FROM information_schema.table_constraints
    WHERE constraint_name = 'fk_profile_picture'
    AND table_name = 'users';
  SQL

  if constraint_exists
    puts 'NOTICE:  column   "fk_profile_picture" already exists, skipping'
  else
    conn.exec <<~SQL
      ALTER TABLE users
      ADD COLUMN profile_picture_id INTEGER,
      ADD CONSTRAINT fk_profile_picture
      FOREIGN KEY (profile_picture_id)
      REFERENCES pictures(id)
      ON DELETE SET NULL;
    SQL
  end

  conn.close
end
