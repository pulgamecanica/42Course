# frozen_string_literal: true

require_relative './base_controller'
require 'twilio-ruby'

class IceController < BaseController
  api_doc '/ice', method: :get do
    tags 'Ice', 'Voice'
    description 'Retrieve ICE (STUN/TURN) servers from Twilio for WebRTC'
    response 200, 'ICE Servers', example: {
      message: 'ICE servers retrieved',
      data: [
        {
          url: 'stun:global.stun.twilio.com:3478',
          urls: 'stun:global.stun.twilio.com:3478'
        },
        {
          credential: 'xxxx/xxxx',
          url: 'turn:global.turn.twilio.com:3478?transport=udp',
          urls: 'turn:global.turn.twilio.com:3478?transport=udp',
          username: 'xxxx/xxxx'
        },
        {
          credential: 'xxxx/xxxx',
          url: 'turn:global.turn.twilio.com:3478?transport=tcp',
          urls: 'turn:global.turn.twilio.com:3478?transport=tcp',
          username: 'xxxx/xxxx'
        },
        {
          credential: 'xxxx/xxxx',
          url: 'turn:global.turn.twilio.com:443?transport=tcp',
          urls: 'turn:global.turn.twilio.com:443?transport=tcp',
          username: 'xxxx/xxxx'
        }
      ]
    }
    response 500, 'Twilio Error', example: { error: 'Failed to get ICE servers from Twilio' }
  end

  get '/ice' do
    client = Twilio::REST::Client.new(ENV['TWILIO_ACCOUNT_SID'], ENV['TWILIO_AUTH_TOKEN'])
    token = client.api.v2010.tokens.create

    { message: 'ICE servers retrieved', data: token.ice_servers }.to_json
  rescue StandardError => e
    LOGGER.error("Twilio ICE error: #{e.message}")
    halt 500, { error: 'Failed to get ICE servers from Twilio' }.to_json
  end
end
