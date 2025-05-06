# frozen_string_literal: true

require_relative './base_controller'
require_relative '../models/location_history'
require_relative '../helpers/location_validator'

class LocationController < BaseController
  # ---------------------------
  # GET /me/location
  # ---------------------------
  api_doc '/me/location', method: :get do
    tags 'User', 'Location'
    description 'Returns the last known latitude and longitude of the current user'
    response 200, 'Returns current location of the user', example: {
      data: {
        latitude: 48.8566,
        longitude: 2.3522,
        city: 'Paris',
        country: 'France',
        recorded_at: '2025-04-12T08:30:00Z'
      }
    }
  end

  get '/me/location' do
    { data: User.location(@current_user['id']) }.to_json
  end

  # ---------------------------
  # RECORD LOCATION
  # ---------------------------
  api_doc '/me/location', method: :post do
    tags 'User', 'Location'
    description "Record the current user's location (estimated from IP), or provided manually by the user"
    param :latitude, Integer, required: false
    param :longitude, Integer, required: false
    param :city, String, required: false
    param :country, String, required: false
    response 200, 'Location saved', example: {
      message: 'Location recorded',
      data: {
        latitude: 48.8566,
        longitude: 2.3522,
        city: 'Paris',
        country: 'France',
        ip_address: '192.0.2.1',
        recorded_at: '2025-04-12T08:30:00Z'
      }
    }
    response 422, 'Geolocation service failed', example: { error: 'Geolocation service failed' }
  end

  post '/me/location' do
    ip_address = request.ip
    user_agent = request.user_agent
    data = json_body

    begin
      if LocationValidator.manual_location_params?(data)
        LocationValidator.validate_manual_location!(data)

        location_history = LocationHistory.record(
          @current_user['id'],
          ip_address,
          user_agent,
          location: {
            country: data['country'],
            city: data['city'],
            latitude: data['latitude'],
            longitude: data['longitude']
          }
        )

        { message: 'Location recorded', data: location_history }.to_json
      else
        record = LocationHistory.record(@current_user['id'], ip_address, user_agent)
        { message: 'Location recorded', data: record }.to_json
      end
    rescue Errors::ValidationError => e
      halt 422, { error: e.message, details: e.details }.to_json
    end
  end

  # ---------------------------
  # USER LOCATIONS
  # ---------------------------
  api_doc '/me/location/history', method: :get do
    tags 'User', 'Location'
    description 'Get your full location history'
    response 200, 'List of location records', example: {
      data: [
        {
          latitude: 48.8566,
          longitude: 2.3522,
          city: 'Paris',
          country: 'France',
          ip_address: '192.0.2.1',
          recorded_at: '2025-04-12T08:30:00Z'
        },
        {
          latitude: 52.52,
          longitude: 13.405,
          city: 'Berlin',
          country: 'Germany',
          ip_address: '192.0.2.2',
          recorded_at: '2025-04-10T16:45:00Z'
        }
      ]
    }
  end

  get '/me/location/history' do
    records = User.locations(@current_user['id'])
    { data: records }.to_json
  end
end
