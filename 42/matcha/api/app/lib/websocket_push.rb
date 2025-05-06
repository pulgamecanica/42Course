# frozen_string_literal: true

require_relative '../../ws_server'
require 'json'

module WebSocketPush
  def self.send_notification(user_id, payload)
    ws = WebSocketServer::CHANNELS[user_id]
    return unless ws && ws.ready_state == Faye::WebSocket::API::OPEN

    message = {
      type: 'notification',
      payload: payload
    }

    ws.send(message.to_json)
  end
end
