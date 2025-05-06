# frozen_string_literal: true

require_relative '../lib/errors'

module MessageValidator
  def self.validate_create!(params)
    Validator.validate!(
      params: { content: params },
      required: [:content],
      length: {
        name: { min: 1, max: 1000 }
      }
    )
  end
end
