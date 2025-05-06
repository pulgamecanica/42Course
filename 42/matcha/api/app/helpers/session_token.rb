# frozen_string_literal: true

require 'base64'
require 'json'
require 'openssl'

module SessionToken
  HEADER = { alg: 'HS256', typ: 'JWT' }.freeze
  SECRET = ENV['JWT_SECRET'] || 'development_secret'

  def self.generate(user_id)
    payload = {
      user_id: user_id,
      exp: Time.now.to_i + (60 * 60 * 92) # 92h expiry
    }

    header_b64  = Base64.urlsafe_encode64(HEADER.to_json)
    payload_b64 = Base64.urlsafe_encode64(payload.to_json)
    signature   = sign("#{header_b64}.#{payload_b64}")

    [header_b64, payload_b64, signature].join('.')
  end

  def self.decode(token)
    header_b64, payload_b64, signature = token.split('.')

    return nil unless secure_compare(signature, sign("#{header_b64}.#{payload_b64}"))

    payload = JSON.parse(Base64.urlsafe_decode64(payload_b64))
    return nil if payload['exp'] && Time.now.to_i > payload['exp']

    payload
  rescue StandardError
    nil
  end

  def self.sign(data)
    OpenSSL::HMAC.hexdigest('SHA256', SECRET, data)
  end

  def self.secure_compare(singn_a, singn_b)
    return false if singn_a.nil? || singn_b.nil? || singn_a.bytesize != singn_b.bytesize

    l = singn_a.unpack "C#{singn_a.bytesize}"
    res = 0
    singn_b.each_byte { |byte| res |= byte ^ l.shift }
    res.zero?
  end
end
