# frozen_string_literal: true

require_relative '../../config/database'

Database.with_open_conn do |conn|
  conn.exec <<~SQL
    CREATE TABLE IF NOT EXISTS image_sticker_placements (
      id SERIAL PRIMARY KEY,
      image_id INTEGER NOT NULL REFERENCES images(id) ON DELETE CASCADE,
      sticker_id INTEGER NOT NULL REFERENCES stickers(id),
      position_x INTEGER NOT NULL,
      position_y INTEGER NOT NULL,
      width INTEGER NOT NULL,
      height INTEGER NOT NULL,
      rotation FLOAT NOT NULL DEFAULT 0.0,
      created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
      updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
    CREATE INDEX IF NOT EXISTS idx_isp_image_id ON image_sticker_placements(image_id);
    CREATE INDEX IF NOT EXISTS idx_isp_sticker_id ON image_sticker_placements(sticker_id);
  SQL
  conn.close
end
