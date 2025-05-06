# frozen_string_literal: true

require_relative './base_controller'
require_relative '../models/user'
require_relative '../models/connection'
require_relative '../helpers/connection_validator'

class ConnectionsController < BaseController
  # ---------------------------
  # LIST CONNECTIONS
  # ---------------------------
  api_doc '/me/connections', method: :get do
    tags 'User', 'Connection'
    description 'Get all users you are connected with'
    response 200, 'List of connected users', example: {
      data: [
        { username: 'janedoe', first_name: 'Jane', last_name: 'Doe' },
        { username: 'bobsmith', first_name: 'Bob', last_name: 'Smith' }
      ]
    }
  end

  get '/me/connections' do
    connections = User.connections(@current_user['id'])
    { data: connections }.to_json
  end

  # ---------------------------
  # CREATE CONNECTION
  # ---------------------------
  api_doc '/me/connect', method: :post do
    tags 'User', 'Connection'
    description 'Create a connection with a matched user'
    param :username, String, required: true, desc: 'The username of the user to connect with'
    response 200, 'Connection created', example: {
      message: 'Connected with janedoe',
      data: {
        id: 42,
        user_a_id: 1,
        user_b_id: 2,
        created_at: '2025-04-12T10:00:00Z',
        updated_at: '2025-04-12T10:00:00Z'
      }
    }
    response 403, 'User is not matched with you', example: { error: 'User is not matched with you' }
    response 404, 'User not found', example: { error: 'User not found' }
    response 422, 'Invalid request', example: {
      error: 'Validation failed',
      details: ['username is required']
    }
  end

  post '/me/connect' do
    data = json_body

    begin
      ConnectionValidator.validate!(data)
      unless @current_user['profile_picture_id']
        raise Errors::ValidationError.new('User is missing fields',
                                          ['User must have a valid profile picture'])
      end
    rescue Errors::ValidationError => e
      halt 422, { error: e.message, details: e.details }.to_json
    end

    target = User.find_by_username(data['username'])
    halt 404, { error: 'User not found' }.to_json unless target
    halt 403, { error: 'User is not matched with you' }.to_json unless User.matches(@current_user['id']).any? do |u|
      u['id'] == target['id']
    end

    connection = Connection.create(@current_user['id'], target['id'])
    if connection
      { message: "Connected with #{data['username']}", data: connection }.to_json
    else
      { message: 'Already connected' }.to_json
    end
  end

  # ---------------------------
  # DELETE CONNECTION
  # ---------------------------
  api_doc '/me/connect', method: :delete do
    tags 'User', 'Connection'
    description 'Remove an existing connection'
    param :username, String, required: true, desc: 'The username of the user to disconnect from'
    response 200, 'Connection removed', example: {
      message: 'Disconnected from janedoe'
    }
    response 403, 'You and username are not connected', example: { error: 'You and janedoe are not connected' }
    response 404, 'User not found', example: { error: 'User not found' }
  end

  delete '/me/connect' do
    data = json_body

    begin
      ConnectionValidator.validate!(data)
    rescue Errors::ValidationError => e
      halt 422, { error: e.message, details: e.details }.to_json
    end

    target = User.find_by_username(data['username'])
    halt 404, { error: 'User not found' }.to_json unless target

    halt 403, { error: "You and #{data['username']} are not connected" }.to_json unless User.connected_with?(
      @current_user['id'], target['id']
    )
    Connection.delete_between(@current_user['id'], target['id'])
    { message: "Disconnected from #{data['username']}" }.to_json
  end
end
