# frozen_string_literal: true

require 'logger'
require_relative './base_controller'
require_relative '../models/blocked_user'
require_relative '../models/profile_view'
require_relative '../lib/logger'

class UsersController < BaseController
  # ---------------------------
  # ME
  # ---------------------------
  api_doc '/me', method: :get do
    tags 'User'
    description 'Get the currently authenticated user'
    response 200, 'User object', example: {
      data: {
        id: 1,
        username: 'johndoe',
        first_name: 'John',
        last_name: 'Doe',
        email: 'john@example.com',
        gender: 'male',
        sexual_preferences: 'everyone',
        biography: 'Just a regular person.',
        latitude: 48.8566,
        longitude: 2.3522
      }
    }
    response 403, 'User not confirmed or banned', example: { error: 'Access forbidden' }
  end

  get '/me' do
    user_data = @current_user.reject { |k, _| k == 'password_digest' }
    { data: user_data }.to_json
  end

  # ---------------------------
  # EDIT ME
  # ---------------------------
  api_doc '/me', method: :patch do
    tags 'User'
    description 'Update profile fields for the current authenticated user'
    param :username, String, required: false, desc: 'New username (must be unique)'
    param :first_name, String, required: false
    param :last_name, String, required: false
    param :gender, String, required: false, desc: 'One of: male, female, other'
    param :sexual_preferences, String, required: false, desc: 'One of: male, female, non_binary, everyone'
    param :biography, String, required: false
    param :birth_year, Integer, required: false
    response 200, 'Profile updated & user object', example: {
      message: 'Profile updated!',
      data: {
        id: 1,
        username: 'newname',
        biography: 'Updated bio.'
      }
    }
    response 422, 'Validation failed', example: {
      error: 'Validation failed',
      details: { username: ['has already been taken'] }
    }
  end

  patch '/me' do
    ip_address = request.ip
    user_agent = request.user_agent
    data = json_body

    begin
      LocationHistory.record(@current_user['id'], ip_address, user_agent)
    rescue Errors::ValidationError => e
      LOGGER.error("LocationHistory failed for user #{@current_user['id']}: #{e.message}")
    end

    begin
      UserValidator.validate_update!(data)
      updated_user = User.update(@current_user['id'], data)

      { message: 'Profile updated!', data: updated_user.reject { |k, _| k == 'password_digest' } }.to_json
    rescue Errors::ValidationError => e
      halt 422, { error: e.message, details: e.details }.to_json
    end
  end

  # ---------------------------
  # LOOKUP USERNAME
  # ---------------------------
  api_doc '/users/:username', method: :get do
    tags 'User', 'PublicProfile'
    description 'Fetch the public profile of a user by their username'
    param :username, String, required: true, desc: 'The unique username of the user'
    param :count_view, :bool, required: false, desc: 'Whether to record the view and send notification (default: true)'
    response 200, 'Public user data', example: {
      data: {
        username: 'janedoe',
        first_name: 'Jane',
        last_name: 'Doe',
        biography: 'Hi there!',
        gender: 'female',
        sexual_preferences: 'male',
        birth_year: '2000',
        profile_picture_id: 42,
        online_status: true,
        last_seen_at: '2025-04-11T14:53:00Z',

        tags: %w[yoga hiking coding],
        pictures: [
          'https://yourdomain.com/uploads/pictures/12.jpg',
          'https://yourdomain.com/uploads/pictures/13.jpg'
        ],
        views: 18,
        visitors: [
          {
            id: 2,
            username: 'johnsmith',
            profile_picture_url: 'https://yourdomain.com/uploads/pictures/99.jpg',
            viewed_at: '2025-04-11T14:53:00Z'
          }
        ],
        total_likes_sent: 7,
        total_likes_received: 8
      }
    }

    response 404, 'User not found or banned', example: { error: 'User not found' }
    response 404, 'User blocked you', example: { error: 'User blocked you' }
    response 404, 'User is blocked', example: { error: 'User is blocked' }
  end

  get '/users/:username' do
    user = User.find_by_username(params[:username])
    halt 404, { error: 'User not found' }.to_json unless user
    halt 404, { error: 'User not available' }.to_json if user['is_banned'] == 't'
    halt 404, { error: 'User blocked you' }.to_json if BlockedUser.blocked?(user['id'], @current_user['id'])
    halt 404, { error: 'User is blocked' }.to_json if BlockedUser.blocked?(@current_user['id'], user['id'])

    count_view = params[:count_view].to_s != 'false'
    if count_view
      ProfileView.record(@current_user['id'], user['id'])
      Notification.create(
        user['id'],
        "#{@current_user['username']} viewed your profile",
        @current_user['id']
      )
    end

    base = UserSerializer.public_view(user)

    id = user['id']
    additional = {
      tags: User.tags(id),
      pictures: User.pictures(id),
      views: User.views(id),
      visitors: User.visitors_for(id),
      likes: User.likes(id),
      liked_by: User.liked_by(id)
    }

    { data: base.merge(additional) }.to_json
  end

  # ---------------------------
  # DELETE USER
  # ---------------------------
  api_doc '/me', method: :delete do
    tags 'User'
    description 'Delete the current authenticated user account and all related data'
    response 204, 'User deleted'
  end

  delete '/me' do
    halt 401, { error: 'Unauthorized' }.to_json unless @current_user
    User.delete(@current_user['id'])
    status 204
  end

  # ---------------------------
  # DISCOVER USERS
  # ---------------------------
  api_doc '/me/discover', method: :post do
    tags 'User', 'Discover'
    description 'Discover users based on preferences (location, age, fame, tags)'
    param :location, Hash, required: false, desc: 'Latitude, longitude and max_distance_km'
    param :min_age, Integer, required: false, desc: 'Minimum age filter'
    param :max_distance_km, Integer, required: false, desc: 'Maximum distance in km'
    param :max_age, Integer, required: false, desc: 'Maximum age filter'
    param :min_fame, Float, required: false, desc: 'Minimum fame rating filter'
    param :tags, Array, required: false, desc: 'Filter by shared tags'
    response 200, 'Returns list of recommended users', example: {
      "data": [
        {
          "user": {
            "id": '247',
            "username": 'marquerite',
            "first_name": 'Savannah',
            "last_name": 'Anderson',
            "biography": 'null',
            "gender": 'female',
            "sexual_preferences": 'everyone',
            "birth_year": '2003',
            "fame_rating": '3.75',
            "profile_picture_id": 'null',
            "online_status": false,
            "last_seen_at": 'null',
            "tags": [
              {
                "id": '33',
                "name": 'yoga'
              },
              {
                "id": '37',
                "name": 'dogs'
              },
              {
                "id": '40',
                "name": 'dancing'
              }
            ],
            "pictures": [
              {
                "id": '28',
                "user_id": '247',
                "url": 'https://robohash.org/marquerite.png?size=300x300&set=set1',
                "is_profile": 't',
                "created_at": '2025-04-15 19:03:26',
                "updated_at": '2025-04-15 19:03:26'
              }
            ],
            "views_count": 5,
            "likes_count": 4
          },
          "score": {
            "location_score": 100.0,
            "tag_score": 100.0,
            "fame_score": 3.75,
            "total": 67.92
          }
        }
      ]
    }
  end

  post '/me/discover' do
    filters = json_body
    candidates = User.discover(@current_user, filters)
    { data: candidates }.to_json
  end
end
