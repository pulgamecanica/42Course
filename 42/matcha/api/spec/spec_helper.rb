# frozen_string_literal: true

ENV['APP_ENV'] = 'test'

require 'rack/test'
require 'rspec'
require_relative '../app'

RSpec.configure do |config|
  config.include Rack::Test::Methods
  config.formatter = :documentation
  config.color = true

  def app
    MatchaApp
  end

  config.before(:each) do
    require_relative '../app/helpers/database'
    Database.with_open_conn do |conn|
      %w[users tags].each do |table|
        conn.exec("DELETE FROM #{table}")
      end
      conn.close
    end
  end
end
