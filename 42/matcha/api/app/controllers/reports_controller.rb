# frozen_string_literal: true

require_relative './base_controller'
require_relative '../helpers/report_validator'

class ReportsController < BaseController
  # ---------------------------
  # REPORT A USER
  # ---------------------------
  api_doc '/me/report', method: :post do
    tags 'User', 'Report'
    description 'Report a user for inappropriate behavior'
    param :username, String, required: true, desc: 'The username of the user to report'
    param :reason, String, required: true, desc: 'enum: [fake_account scam sexual_harassment]'
    param :description, String, required: false, desc: 'Optional description for the report'
    response 200, 'User reported successfully', example: {
      message: 'User reported successfully',
      data: {
        username: 'fakeuser'
      }
    }
    response 404, 'User not found', example: { error: 'User not found' }
    response 422, 'Cannot report yourself', example: { error: 'You cannot report yourself' }
  end

  post '/me/report' do
    data = json_body

    begin
      ReportValidator.validate!(data)
    rescue Errors::ValidationError => e
      halt 422, { error: e.message, details: e.details }.to_json
    end

    target = User.find_by_username(data['username'])
    halt 404, { error: 'User not found' }.to_json unless target

    halt 422, { error: 'You cannot report yourself' }.to_json if target['id'].to_s == @current_user['id'].to_s

    Report.create(
      @current_user['id'],
      target['id'],
      data['reason'],
      data['description']
    )

    check_ban_user(target)

    { message: 'User reported successfully', data: { username: target['username'] } }.to_json
  end

  private

  def check_ban_user(user)
    reports = Report.find_reports_against_user(user['id'])
    return unless reports.size >= 3

    User.ban!(user['username'])
  end
end
