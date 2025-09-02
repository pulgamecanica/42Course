# frozen_string_literal: true

require 'logger'
require_relative './config/environment'
require_relative './config/database'
require_relative './config/cors'

class CamagruApp < Sinatra::Base
  use CORS
  get '/' do
    { message: 'Welcome to Camagru42' }.to_json
  end
  
  use AuthController
end

at_exit do
  puts 'Shutting down DB connections...'
  Database.pool.shutdown do |conn|
    conn.close
  rescue StandardError
    nil
  end
end
