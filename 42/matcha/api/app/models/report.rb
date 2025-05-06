# frozen_string_literal: true

require_relative '../helpers/database'

module Report
  def self.create(reporter_id, reported_user_id, reason, description = nil)
    Database.with_conn do |conn|
      conn.exec_params(
        <<~SQL,
          INSERT INTO reports (reporter_id, reported_user_id, reason, description)
          VALUES ($1, $2, $3, $4)
          RETURNING *
        SQL
        [reporter_id, reported_user_id, reason, description]
      )&.first
    end
  end

  def self.find_by_reporter(reporter_id)
    Database.with_conn do |conn|
      conn.exec_params(
        'SELECT * FROM reports WHERE reporter_id = $1 ORDER BY created_at DESC',
        [reporter_id]
      ).to_a
    end
  end

  def self.find_reports_against_user(user_id)
    Database.with_conn do |conn|
      conn.exec_params(
        'SELECT * FROM reports WHERE reported_user_id = $1 ORDER BY created_at DESC',
        [user_id]
      ).to_a
    end
  end
end
