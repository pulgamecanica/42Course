# frozen_string_literal: true

require 'logger'
require_relative './config/environment'
require_relative './app/lib/cors'
require_relative './app/lib/logger'

class MatchaApp < Sinatra::Base
  configure :production do
    puts '🛡️ Rack::Protection Middleware disabled'
    use Rack::Protection::HostAuthorization, permitted_hosts: ['matcha42.fly.dev']
  end

  use CORS
  set :protection, false

  get '/' do
    { message: 'Welcome to MatchaApp' }.to_json
  end

  use AuthController
  use EmailActionsController
  use UsersController
  use TagsController
  use BlockedUsersController
  use LikesController
  use ProfileViewsController
  use PicturesController
  use LocationController
  use ConnectionsController
  use MessagesController
  use DatesController
  use NotificationsController
  use ReportsController
  use IceController
end

at_exit do
  puts 'Shutting down DB connections...'
  Database.pool.shutdown do |conn|
    conn.close
  rescue StandardError
    nil
  end
end
