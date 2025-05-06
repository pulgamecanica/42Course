# frozen_string_literal: true

require_relative '../helpers/database'
require_relative '../helpers/sql_helper'

class Tag
  def self.all
    Database.with_conn do |conn|
      res = conn.exec('SELECT * FROM tags ORDER BY name ASC')
      res.to_a
    end
  end

  def self.create(name)
    allowed_fields = %w[
      name
    ]
    SQLHelper.create(:tags, { name: name }, allowed_fields)
  end

  def self.find_by_name(name)
    SQLHelper.find_by(:tags, :name, name)
  end

  def self.users(tag_id)
    SQLHelper.many_to_many(:tag, :users, tag_id)
  end
end
