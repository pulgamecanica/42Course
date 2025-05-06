# frozen_string_literal: true

module LocationValidator
  class ValidationError < StandardError; end

  def self.validate_manual_location!(params)
    Validator.validate!(
      params: params,
      required: %i[latitude longitude]
    )
  end

  def self.manual_location_params?(params)
    %w[latitude longitude city country].any? { |key| params.key?(key) }
  end
end
