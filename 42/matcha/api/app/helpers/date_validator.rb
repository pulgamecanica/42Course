# frozen_string_literal: true

# app/helpers/date_validator.rb
require_relative '../lib/errors'

module DateValidator
  def self.validate_create!(params)
    Validator.validate!(
      params: params,
      required: %i[username scheduled_at location]
    )

    return if Time.parse(params['scheduled_at'])

    raise Errors::ValidationError.new('Invalid date creation',
                                      'scheduled_at must be a valid datetime string')
  end
end
