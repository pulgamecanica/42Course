# frozen_string_literal: true

require_relative '../lib/errors'

module RequestHelper
  def self.safe_json_parse(body)
    raise Errors::ValidationError, 'Missing or empty request body' if body.nil? || body.strip.empty?

    JSON.parse(body)
  rescue JSON::ParserError
    raise Errors::ValidationError, 'Invalid JSON payload'
  end

  def self.normalize_params(params)
    params.transform_keys(&:to_s)
  end
end
