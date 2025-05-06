# frozen_string_literal: true

require_relative './validator'

module ReportValidator
  VALID_REASONS = %w[fake_account scam sexual_harassment].freeze

  def self.validate!(params)
    Validator.validate!(
      params: params,
      required: %i[username reason],
      enums: {
        reason: VALID_REASONS
      }
    )
  end
end
