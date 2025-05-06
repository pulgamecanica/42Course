# frozen_string_literal: true

require_relative './base_controller'
require_relative '../models/email_action'
require_relative '../helpers/mailer'

class EmailActionsController < BaseController
  # ---------------------------
  # REQUEST PASSWORD RESET
  # ---------------------------
  api_doc '/email/reset-password', method: :post do
    tags 'User', 'Email'
    description 'Send password reset email'
    param :email, String, required: true, desc: 'Your account email'
    response 200, 'Email sent', example: { message: 'Password reset email sent' }
    @data[:auth_required] = false
  end

  post '/email/reset-password' do
    data = json_body
    halt 422, { error: 'Missing email' }.to_json unless data['email']

    user = User.find_by_email(data['email'])
    halt 404, { error: 'User not found' }.to_json unless user

    action = EmailAction.create(user['id'], 'password_reset')
    Mailer.send_password_reset_email(user['email'], action['code'])

    { message: 'Password reset email sent' }.to_json
  end

  # ---------------------------
  # VALIDATE CONFIRMATION CODE
  # ---------------------------
  api_doc '/email/confirm/:code', method: :get do
    tags 'User', 'Email'
    description 'Validate confirmation code and confrim account'
    param :code, String, required: true, desc: 'Confirmation code'
    response 200, 'Account confirmed', example: { message: 'Account confirmed' }
    response 404, 'Invalid or expired code', example: { error: 'Invalid or expired code' }
    @data[:auth_required] = false
  end

  get '/email/confirm/:code' do
    code = params[:code]
    action = EmailAction.find_by_code(code)
    halt 404, { error: 'Invalid or expired code' }.to_json unless action
    halt 404, { error: 'Wrong action type' }.to_json unless action['action_type'] == 'email_confirmation'

    user = User.find_by_id(action['user_id'])
    User.confirm!(user['username'])
    EmailAction.consume(code)

    { message: 'Account confirmed' }.to_json
  end

  # ---------------------------
  # RESET PASSWORD USING A VALID CODE
  # ---------------------------
  api_doc '/email/reset-password/:code', method: :post do
    tags 'User', 'Email'
    description 'Reset your password using a valid reset code'
    param :code, String, required: true, desc: 'Reset password code received by email'
    param :new_password, String, required: true, desc: 'Your new password'
    response 200, 'Password reset successfully', example: { message: 'Password reset successfully' }
    response 404, 'Invalid or expired code', example: { error: 'Invalid or expired code' }
    response 422, 'Missing or invalid input', example: { error: 'Invalid password' }
    @data[:auth_required] = false
  end

  post '/email/reset-password/:code' do
    code = params[:code]
    data = json_body

    halt 422, { error: 'Missing new password' }.to_json unless data['new_password']

    action = EmailAction.find_by_code(code)
    halt 404, { error: 'Invalid or expired code' }.to_json unless action
    halt 404, { error: 'Wrong action type' }.to_json unless action['action_type'] == 'password_reset'

    new_password = data['new_password']

    halt 422, { error: 'Password too short. Minimum 6 characters.' }.to_json if new_password.length < 6

    User.update_password!(action['user_id'], new_password)

    EmailAction.consume(code)

    { message: 'Password reset successfully' }.to_json
  end
end
