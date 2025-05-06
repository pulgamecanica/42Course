# frozen_string_literal: true

require 'sinatra/base'
require 'pg'
require 'dotenv/load'
require 'json'
require 'sinatra/reloader'

require_relative '../app/lib/api_doc'
Dir[File.expand_path('../app/controllers/*.rb', __dir__)].sort.each do |file|
  require file unless file.end_with?('base_controller.rb')
end
Dir['./app/models/*.rb'].each { |f| require f }
Dir['./app/helpers/*.rb'].each { |f| require f }
