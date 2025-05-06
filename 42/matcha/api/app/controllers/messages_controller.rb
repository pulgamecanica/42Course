# frozen_string_literal: true

require_relative './base_controller'
require_relative '../models/message'
require_relative '../models/connection'
require_relative '../models/user'
require_relative '../helpers/message_validator'

class MessagesController < BaseController
  # ---------------------------
  # SEND A MESSAGE
  # ---------------------------
  api_doc '/me/messages', method: :post do
    tags 'Message'
    description 'Send a message to a user (requires existing connection)'
    param :username, String, required: true, desc: "Recipient's username"
    param :content, String, required: true, desc: 'Message content'
    response 200, 'Message sent', example: {
      message: 'Message sent',
      data: {
        id: 40,
        connection_id: 42,
        sender_id: 121,
        content: 'Hey there!',
        created_at: '2025-04-12 18:07:25.031215'
      }
    }
    response 404, 'User or connection not found', example: {
      error: 'No connection found with janedoe'
    }
    response 422, 'Validation error', example: {
      error: 'Invalid request',
      details: ['content must not be empty']
    }
  end

  post '/me/messages' do
    data = json_body

    begin
      MessageValidator.validate_create!(data)
    rescue Errors::ValidationError => e
      halt 422, { error: e.message, details: e.details }.to_json
    end

    recipient = User.find_by_username(data['username'])
    halt 404, { error: 'User not found' }.to_json unless recipient

    connection = Connection.find_between(@current_user['id'], recipient['id'])
    halt 404, { error: "No connection found with #{data['username']}" }.to_json unless connection

    message = Message.create(connection['id'], @current_user['id'], data['content'])

    Notification.create(
      recipient['id'],
      message['content'],
      @current_user['id']
    )
    { message: 'Message sent', data: message }.to_json
  end

  # ---------------------------
  # ALL MESSAGES WITH USER
  # ---------------------------
  api_doc '/me/messages/:username', method: :get do
    tags 'Message'
    description 'Get all messages exchanged with a given user'
    param :username, String, required: true, desc: "Other user's username"
    response 200, 'Message thread', example: {
      data: {
        user: {
          id: 127,
          username: 'bob',
          email: 'bob@example.com',
          first_name: 'Bob',
          last_name: 'B',
          gender: 'male',
          sexual_preferences: 'female',
          biography: nil,
          is_email_verified: true,
          is_banned: false,
          fame_rating: 0,
          latitude: nil,
          longitude: nil,
          online_status: false,
          last_seen_at: nil,
          created_at: '2025-04-12 18:08:00.699235',
          updated_at: '2025-04-12 18:08:00.702444',
          profile_picture_id: nil
        },
        messages: [
          {
            id: 41,
            connection_id: 44,
            sender_id: 127,
            content: 'Hi Bob!',
            created_at: '2025-04-12 18:08:00.704577',
            sender_username: 'bob'
          }
        ]
      }
    }
    response 404, 'User or connection not found', example: {
      error: 'No connection found with bob'
    }
  end

  get '/me/messages/:username' do
    other = User.find_by_username(params[:username])
    halt 404, { error: 'User not found' }.to_json unless other

    connection = Connection.find_between(@current_user['id'], other['id'])
    halt 404, { error: "No connection found with #{params[:username]}" }.to_json unless connection

    messages = Message.for_connection(connection['id'])
    {
      data: {
        user: other,
        messages: messages
      }
    }.to_json
  end

  # ---------------------------
  # ALL MESSAGES (GROUPED)
  # ---------------------------
  api_doc '/me/messages', method: :get do
    tags 'Message'
    description 'Get all conversations grouped by user'
    response 200, 'List of message threads', example: {
      data: [
        {
          user: {
            id: 129,
            username: 'bob',
            first_name: 'Bob',
            last_name: 'B',
            biography: nil,
            gender: 'male',
            sexual_preferences: 'female',
            profile_picture_id: nil,
            online_status: false,
            last_seen_at: nil
          },
          messages: [
            {
              id: 42,
              connection_id: 45,
              sender_id: 128,
              content: 'Hey again!',
              created_at: '2025-04-12 18:08:09.476871',
              sender_username: 'alice'
            }
          ]
        }
      ]
    }
  end

  get '/me/messages' do
    messages = User.messages(@current_user['id'])

    { data: messages }.to_json
  end
end
