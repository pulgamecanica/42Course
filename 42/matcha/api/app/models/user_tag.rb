# frozen_string_literal: true

require_relative '../helpers/database'
require_relative './user'
require_relative './tag'

class UserTag
  def self.add_tag(user_id, tag_id)
    Database.with_conn do |conn|
      conn.exec_params(
        'INSERT INTO user_tags (user_id, tag_id) VALUES ($1, $2) ON CONFLICT DO NOTHING',
        [user_id, tag_id]
      )
    end
  end

  def self.remove_tag(user_id, tag_id)
    Database.with_conn do |conn|
      conn.exec_params(
        'DELETE FROM user_tags WHERE user_id = $1 AND tag_id = $2',
        [user_id, tag_id]
      )
    end
  end

  def self.user(user_id, tag_id)
    Database.with_conn do |conn|
      res = conn.exec_params(
        "SELECT users.* FROM users
         JOIN user_tags ut ON ut.user_id = users.id
         WHERE ut.user_id = $1 AND ut.tag_id = $2",
        [user_id, tag_id]
      )
      UserSerializer.public_view(res&.first)
    end
  end

  def self.tag(user_id, tag_id)
    Database.with_conn do |conn|
      res = conn.exec_params(
        "SELECT tags.* FROM tags
         JOIN user_tags ut ON ut.tag_id = tags.id
         WHERE ut.user_id = $1 AND ut.tag_id = $2",
        [user_id, tag_id]
      )
      UserSerializer.public_view(res&.first)
    end
  end
end
