# frozen_string_literal: true

require_relative './base_controller'
require_relative '../models/picture'
require_relative '../models/user'
require_relative '../helpers/picture_validator'

class PicturesController < BaseController
  # ---------------------------
  # USER PICTURES
  # ---------------------------
  api_doc '/me/pictures', method: :get do
    tags 'User', 'Picture'
    description 'List all pictures uploaded by the current user'
    response 200, 'Returns list of pictures', example: {
      data: [
        { id: 1, url: 'https://cdn.example.com/pic1.jpg', is_profile: false },
        { id: 2, url: 'https://cdn.example.com/pic2.jpg', is_profile: true }
      ]
    }
  end

  get '/me/pictures' do
    pictures = User.pictures(@current_user['id'])
    { data: pictures }.to_json
  end

  # ---------------------------
  # ADD NEW PICTURE
  # ---------------------------
  api_doc '/me/pictures', method: :post do
    tags 'User', 'Picture'
    description 'Upload a new picture'
    param :url, String, required: true, desc: 'URL of the picture'
    param :is_profile, TrueClass, required: false, desc: 'Set as profile picture'
    response 201, 'Picture created', example: {
      message: 'Picture uploaded!',
      data: { id: 3, url: 'https://cdn.example.com/pic3.jpg', is_profile: false }
    }
    response 422, 'Invalid data', example: {
      error: 'Validation failed',
      details: ['is not a valid URL']
    }
    response 422, 'Too many pictures', example: {
      error: 'Too many pictures',
      details: ['<= 5']
    }
  end

  post '/me/pictures' do
    data = json_body

    begin
      PictureValidator.validate_create!(data)
      amount_of_pictures = User.pictures(@current_user['id'])&.size || 0
      raise Errors::ValidationError.new('Too many pictures', ['<= 5']) unless amount_of_pictures < 5
    rescue Errors::ValidationError => e
      halt 422, { error: e.message, details: e.details }.to_json
    end

    picture = Picture.create(@current_user['id'], data['url'], is_profile: data['is_profile'])

    User.update(@current_user['id'], { profile_picture_id: picture['id'] }) if data['is_profile']

    status 201
    { message: 'Picture uploaded!', data: picture }.to_json
  end

  # ---------------------------
  # UPDATE PICTURE
  # ---------------------------
  api_doc '/me/pictures/:id', method: :patch do
    tags 'User', 'Picture'
    description 'Edit a picture (e.g., set as profile)'
    param :id, Integer, required: true
    param :is_profile, TrueClass, required: false
    param :url, String, required: false
    response 200, 'Picture updated', example: {
      message: 'Picture updated!',
      data: { id: 2, url: 'https://cdn.example.com/updated.jpg', is_profile: true }
    }
    response 404, 'Picture not found', example: { error: 'Picture not found' }
    response 403, 'Not your picture', example: { error: 'Unauthorized' }
  end

  patch '/me/pictures/:id' do
    picture = Picture.find_by_id(params[:id])
    halt 404, { error: 'Picture not found' }.to_json unless picture
    halt 403, { error: 'Unauthorized' }.to_json if picture['user_id'] != @current_user['id']

    data = json_body

    begin
      PictureValidator.validate_update!(data)
    rescue Errors::ValidationError => e
      halt 422, { error: e.message, details: e.details }.to_json
    end

    updates = data.slice('url', 'is_profile')

    picture = Picture.update(picture['id'], updates)

    if updates['is_profile']
      Picture.set_profile(@current_user['id'], picture['id'])
      User.update(@current_user['id'], { profile_picture_id: picture['id'] })
    end
    { message: 'Picture updated!', data: picture }.to_json
  end

  # ---------------------------
  # DELETE PICTURE
  # ---------------------------
  api_doc '/me/pictures/:id', method: :delete do
    tags 'User', 'Picture'
    description 'Delete a picture'
    param :id, Integer, required: true
    response 200, 'Picture deleted', example: { message: 'Picture deleted' }
    response 404, 'Not found', example: { error: 'Picture not found' }
    response 403, 'Unauthorized', example: { error: 'Unauthorized' }
  end

  delete '/me/pictures/:id' do
    picture = Picture.find_by_id(params[:id])
    halt 404, { error: 'Picture not found' }.to_json unless picture
    halt 403, { error: 'Unauthorized' }.to_json if picture['user_id'] != @current_user['id']

    Picture.delete(picture['id'])

    # Unset profile picture reference if it was removed
    User.update(@current_user['id'], { profile_picture_id: nil }) if @current_user['profile_picture_id'] == picture['id']

    { message: 'Picture deleted' }.to_json
  end

  # ---------------------------
  # LOOKUP USER PICTURES
  # ---------------------------
  api_doc '/users/:username/pictures', method: :get do
    tags 'User', 'PublicProfile', 'Picture'
    description 'Fetch the pictures of a user by their username'
    param :username, String, required: true, desc: 'The unique username of the user'
    response 200, 'Public user data', example: {
      data: [
        {
          id: 217,
          user_id: 2248,
          url: 'https://robohash.org/wallace.png?size=300x300&set=set1',
          is_profile: 't',
          created_at: '2025-04-15 07:49:41',
          updated_at: '2025-04-15 07:49:41'
        }
      ]
    }
    response 404, 'User not found or banned', example: { error: 'User not found' }
    response 404, 'User blocked you', example: { error: 'User blocked you' }
    response 404, 'User is blocked', example: { error: 'User is blocked' }
  end

  get '/users/:username/pictures' do
    user = User.find_by_username(params[:username])
    halt 404, { error: 'User not found' }.to_json unless user
    halt 404, { error: 'User not available' }.to_json if user['is_banned'] == 't'
    halt 404, { error: 'User blocked you' }.to_json if BlockedUser.blocked?(user['id'], @current_user['id'])
    halt 404, { error: 'User is blocked' }.to_json if BlockedUser.blocked?(@current_user['id'], user['id'])

    pictures = User.pictures(user['id'])

    { data: pictures }.to_json
  end
end
