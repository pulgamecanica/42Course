# frozen_string_literal: true

require_relative './validator'

module TagValidator
  def self.validate_name!(name)
    Validator.validate!(
      params: { name: name },
      required: [:name],
      length: {
        name: { min: 2, max: 20 }
      }
    )
  end
end
