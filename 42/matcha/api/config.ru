# frozen_string_literal: true

require_relative './app'
require_relative './ws_server'
require 'rack/protection'

map '/' do
  run MatchaApp
end

map '/ws' do
  run WebSocketServer
end
