# frozen_string_literal: true

require 'sinatra/base'
require 'json'

class BaseController < Sinatra::Base
  include APIDoc

  # ---------- common Rack middleware ----------
  use Rack::Protection
  use Rack::Protection::HostAuthorization,
      permitted_hosts: [ENV.fetch('ALLOWED_HOST', 'localhost')]

  # ---------- global settings ----------
  before do
    content_type :json

    next if request.request_method == 'OPTIONS'
    next if request.path_info == '/' || request.path_info.start_with?('/auth') || request.path_info.start_with?('/email')

    require_auth!
  end

  # ---------- error handling ----------
  error do |err|
    logger.error(err.full_message)
    halt 500, { error: 'Internal Server Error' }.to_json
  end

  not_found do
    halt 404, { error: 'Not Found' }.to_json
  end

  # ---------- helpers methods  ----------

  helpers do
    def current_user
      @current_user
    end

    def json_body
      body = request.body.read

      begin
        RequestHelper.safe_json_parse(body)
      rescue Errors::ValidationError => e
        halt 400, { error: e.message }.to_json
      end
    end

    def require_auth!
      return if @current_user
      token = request.env['HTTP_AUTHORIZATION']&.sub(/^Bearer /, '')
      halt 401, { error: 'Missing or invalid Authorization header' }.to_json unless token

      payload = SessionToken.decode(token)
      halt 401, { error: 'Invalid or expired session token' }.to_json unless payload

      user = User.find_by_id(payload['user_id'])
      halt 401, { error: 'Invalid user' }.to_json unless user
      halt 403, { error: 'Email not verified' }.to_json unless user['is_email_verified'] == 't'
      halt 403, { error: 'Account is banned' }.to_json if user['is_banned'] == 't'

      @current_user = user
    end
  end
end
