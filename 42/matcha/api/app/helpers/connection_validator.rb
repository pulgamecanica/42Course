# frozen_string_literal: true

require_relative '../lib/errors'

module ConnectionValidator
  def self.validate!(params)
    Validator.validate!(
      params: params,
      required: [:username]
    )
  end
end
