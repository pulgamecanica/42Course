# frozen_string_literal: true

require_relative './base_controller'
require_relative '../models/user'
require_relative '../models/connection'
require_relative '../models/date'
require_relative '../helpers/date_validator'

class DatesController < BaseController
  # ---------------------------
  # CREATE DATE
  # ---------------------------
  api_doc '/me/dates', method: :post do
    tags 'User', 'Date'
    description 'Schedule a date with a connected user'
    param :username, String, required: true, desc: "The other user's username"
    param :scheduled_at, String, required: true, desc: 'Scheduled time for the date (ISO 8601)'
    param :location, String, required: true, desc: 'Optional location for the date'
    param :note, String, required: false, desc: 'Oprional note for the date'
    response 201, 'Date scheduled', example: {
      message: 'Date scheduled',
      data: {
        id: '42',
        connection_id: '99',
        scheduled_at: '2025-04-12T20:00:00Z',
        location: 'Café Luna',
        created_at: '2025-04-12T18:22:00Z'
      }
    }
    response 404, 'User or connection not found', example: {
      error: 'No connection found with this user'
    }
    response 422, 'Validation error', example: {
      error: 'Invalid date creation',
      details: ['scheduled_at must be a valid datetime string']
    }
  end

  post '/me/dates' do
    data = json_body

    begin
      DateValidator.validate_create!(data)
    rescue Errors::ValidationError => e
      halt 422, { error: e.message, details: e.details }.to_json
    end

    other = User.find_by_username(data['username'])
    halt 404, { error: 'User not found' }.to_json unless other

    connection = Connection.find_between(@current_user['id'], other['id'])
    halt 404, { error: 'No connection found with this user' }.to_json unless connection

    date = Date.create(connection['id'], @current_user['id'], data['location'],
                       Time.parse(data['scheduled_at']), data['note'])

    Notification.create(
      other['id'],
      "#{@current_user['username']} scheduled a date with you",
      @current_user['id'],
      'date'
    )

    Notification.create(
      @current_user['id'],
      "#{other['username']} and you now have date",
      other['id'],
      'date'
    )
    status 201
    { message: 'Date scheduled', data: date }.to_json
  end

  # ---------------------------
  # VIEW ALL DATES
  # ---------------------------
  api_doc '/me/dates', method: :get do
    tags 'User', 'Date'
    description 'List all scheduled dates for the current user'
    response 200, 'Array of scheduled dates', example: {
      data: [
        {
          id: '42',
          connection_id: '99',
          scheduled_at: '2025-04-12T20:00:00Z',
          location: 'Café Luna',
          created_at: '2025-04-12T18:22:00Z',
          username: 'bob',
          first_name: 'Bob',
          last_name: 'Smith'
        }
      ]
    }
  end

  get '/me/dates' do
    { data: User.dates(@current_user['id']) }.to_json
  end

  # ---------------------------
  # DELETE DATE
  # ---------------------------
  api_doc '/me/dates/:id', method: :delete do
    tags 'Date'
    description 'Cancel or delete a date request'
    param :id, Integer, required: true, desc: 'ID of the date request'
    response 200, 'Date request deleted', example: {
      message: 'Date deleted'
    }
    response 404, 'Date not found', example: { error: 'Date not found' }
    response 404, 'Connection not found', example: { error: 'Connection not found' }
    response 403, 'Not authorized to delete this date', example: { error: 'Unauthorized' }
  end

  delete '/me/dates/:id' do
    date = Date.find_by_id(params[:id])
    halt 404, { error: 'Date not found' }.to_json unless date
    connection = Connection.find_by_id(date['connection_id'])
    halt 404, { error: 'Connection not found' }.to_json unless date
    connection_ids = [connection['user_a_id'], connection['user_b_id']]
    halt 403, { error: 'Unauthorized' }.to_json unless connection_ids.include?(@current_user['id'])

    other_id = connection_ids[0] == @current_user['id'] ? connection_ids[1] : connection_ids[0]

    Notification.create(
      other_id,
      "#{@current_user['username']} canceled a date with you",
      @current_user['id'],
      'date'
    )

    Notification.create(
      @current_user['id'],
      'Canceled the date',
      other_id,
      'date'
    )

    Date.delete(date['id'])
    { message: 'Date deleted' }.to_json
  end
end
