# frozen_string_literal: true

require_relative './app/helpers/session_token'
require_relative './app/models/user'
require 'sinatra/base'
require 'faye/websocket'
require 'json'
require 'concurrent'

class WebSocketServer < Sinatra::Base
  puts '🔧 WebSocketServer mounted at /ws'

  KEEPALIVE_TIME = 30 # in seconds
  CHANNELS = Concurrent::Map.new

  get '/' do
    if Faye::WebSocket.websocket?(env)
      ws = Faye::WebSocket.new(env, nil, ping: KEEPALIVE_TIME)
      user = nil

      ws.on :open do |_event|
        user = extract_current_user(env)
        if user
          CHANNELS[user['id']] = ws
          User.set_online!(user['id'])
          puts "🔌 WebSocket connected: user #{user['username']}"
        else
          ws.close(4001, 'Unauthorized')
        end
      end

      ws.on :message do |event|
        next unless user

        begin
          data = JSON.parse(event.data)

          type = data['type']
          payload = data['payload']

          case type
          when 'message'
            handle_message(user, payload)
          when 'typing'
            handle_typing(user, payload)
          when 'call:offer', 'call:answer', 'call:ice-candidate',
                'call:end', 'call:decline', 'call:busy', 'call:unavailable'
            relay_call_signal(user, type, payload)
          else
            ws.send({ type: 'error', payload: "Unknown message type [#{type} => #{payload}]" }.to_json)
          end
        rescue StandardError => e
          ws.send({ type: 'error', payload: e.message }.to_json)
        end
      end

      ws.on :close do |_event|
        if user
          CHANNELS.delete(user['id'])
          User.set_offline!(user['id'])
          puts "❌ Disconnected: user #{user['username']}"
        end
        ws = nil
      end

      ws.rack_response
    else
      [400, { 'Content-Type' => 'text/plain' }, ['WebSocket only']]
    end
  end

  private

  def relay_call_signal(sender, type, payload)
    to_user_id = payload['to_user_id']
    return unless to_user_id

    target_ws = CHANNELS[to_user_id]
    return unless target_ws && target_ws.ready_state == Faye::WebSocket::API::OPEN

    signal =
      case type
      when 'call:offer' then { offer: payload['offer'] }
      when 'call:answer' then { answer: payload['answer'] }
      when 'call:ice-candidate' then { candidate: payload['candidate'] }
      end

    message = {
      type: type,
      payload: {
        from_user_id: sender['id']
      }.merge(signal || {})
    }

    target_ws.send(message.to_json)
  end

  def extract_current_user(env)
    return nil unless env

    req = Rack::Request.new(env)
    token = env['HTTP_TOKEN'] || req.params['token'] if req
    return nil unless token

    payload = SessionToken.decode(token)
    return nil unless payload

    User.find_by_id(payload['user_id'])
  end

  def handle_message(sender, payload)
    to_user_id = payload['to_user_id']
    content = payload['content']

    conn = Connection.find_between(sender['id'], to_user_id)
    return unless conn

    message = Message.create(conn['id'], sender['id'], content)

    Notification.create(to_user_id, "#{sender['username']} sent you a message", sender['id'], 'message')

    target_ws = CHANNELS[to_user_id]
    return unless target_ws

    target_ws.send({
      type: 'message',
      payload: message
    }.to_json)
  end

  def handle_typing(sender, payload)
    to_user_id = payload['to_user_id']

    target_ws = CHANNELS[to_user_id]
    return unless target_ws

    target_ws.send({
      type: 'typing',
      payload: {
        from: sender['id'],
        connection_id: payload['connection_id']
      }
    }.to_json)
  end
end
