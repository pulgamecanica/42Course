# frozen_string_literal: true

require 'securerandom'

class EmailAction
  ACTION_TYPES = %w[email_confirmation password_reset].freeze
  CODE_EXPIRATION = {
    'email_confirmation' => 24 * 60 * 60,
    'password_reset' => 15 * 60
  }.freeze

  def self.create(user_id, action_type)
    raise 'Invalid action type' unless ACTION_TYPES.include?(action_type)

    code = generate_unique_code
    expires_at = Time.now + CODE_EXPIRATION[action_type]

    params = {}
    params['user_id'] = user_id
    params['action_type'] = action_type
    params['code'] = code
    params['expires_at'] = expires_at

    allowed_fields = %w[
      user_id action_type code expires_at
    ]
    SQLHelper.create(:email_actions, params, allowed_fields)
  end

  def self.find_by_code(code)
    SQLHelper.find_by(:email_actions, :code, code)
  end

  def self.consume(code)
    action = find_by_code(code)
    SQLHelper.delete(:email_actions, action['id'])
  end

  def self.generate_unique_code
    SecureRandom.hex(20)
  end
end
