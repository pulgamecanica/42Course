# frozen_string_literal: true

require 'net/http'
require 'uri'

module ImageAnalyzer
  def self.valid_image_url?(url)
    uri = URI.parse(url)
    return false unless uri.is_a?(URI::HTTP) || uri.is_a?(URI::HTTPS)

    response = Net::HTTP.start(uri.host, uri.port, use_ssl: uri.scheme == 'https') do |http|
      http.head(uri.request_uri)
    end

    content_type = response['content-type']
    content_type&.start_with?('image/')
  rescue StandardError
    false
  end
end
