# frozen_string_literal: true

require_relative './validator'

module UserValidator
  VALID_GENDERS = %w[male female other].freeze
  VALID_PREFS   = %w[male female non_binary everyone].freeze
  COMMON_PASSWORDS = %w[
    password 123456 12345678 123456789 qwerty abc123 letmein admin welcome
    iloveyou monkey football 123123 dragon sunshine aserty fuckit
  ].freeze

  def self.validate!(params)
    Validator.validate!(
      params: params,
      required: %i[
        username email password
        first_name last_name
      ]
    )

    return unless COMMON_PASSWORDS.include?(params['password'].to_s.downcase) || params['password'].size < 5

    raise Errors::ValidationError.new('Password is too common or insecure', ['password is not valid'])
  end

  def self.validate_update!(params)
    allowed_keys = %w[
      username first_name last_name biography
      gender sexual_preferences birth_year
    ]

    unknown_keys = params.keys - allowed_keys
    raise Errors::ValidationError.new('Unknown fields', unknown_keys) unless unknown_keys.empty?

    Validator.validate!(
      params: params,
      enums: {
        gender: VALID_GENDERS,
        sexual_preferences: VALID_PREFS
      },
      range: {
        birth_year: { min: 1900, max: Time.now.year.to_i - 18 }
      }
    )
  end
end
