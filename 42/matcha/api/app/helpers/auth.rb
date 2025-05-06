# frozen_string_literal: true

require_relative '../models/user'
require_relative './session_token'

module Auth
  def self.authenticate!(request)
    token = request.env['HTTP_AUTHORIZATION']&.sub(/^Bearer /, '')
    return nil unless token

    decoded = SessionToken.decode(token)
    return nil unless decoded

    user = User.find_by_id(decoded['user_id'])
    return nil if user.nil? || user['is_banned'] == 't' || user['is_email_verified'] != 't'

    user
  end
end
