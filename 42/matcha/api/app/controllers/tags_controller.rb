# frozen_string_literal: true

require_relative './base_controller'
require_relative '../models/tag'
require_relative '../models/user_tag'
require_relative '../helpers/tag_validator'

class TagsController < BaseController
  # ---------------------------
  # TAGS
  # ---------------------------
  api_doc '/tags', method: :get do
    tags 'Tag'
    description 'List all tags'
    response 200, 'Returns a list of available tags', example: {
      data: [
        { id: 1, name: 'travel' },
        { id: 2, name: 'music' }
      ]
    }
  end

  get '/tags' do
    { data: Tag.all }.to_json
  end

  # ---------------------------
  # NEW TAG
  # ---------------------------
  api_doc '/tags', method: :post do
    tags 'Tag'
    description 'Create a new tag'
    param :name, String, required: true, desc: 'The name of the tag'
    response 201, 'Tag created', example: {
      message: 'Tag created',
      data: { id: 3, name: 'photography' }
    }
    response 422, 'Missing or invalid name', example: {
      error: 'Validation failed',
      details: { name: ['is too short'] }
    }
    response 422, 'Tag name already taken', example: { error: 'Tag name already taken' }
  end

  post '/tags' do
    data = json_body

    begin
      TagValidator.validate_name!(data['name'])
      tag = Tag.create(data['name'])
      status 201
      { message: 'Tag created', data: tag }.to_json
    rescue Errors::ValidationError => e
      halt 422, { error: e.message, details: e.details }.to_json
    rescue PG::UniqueViolation
      halt 422, { error: 'Tag name already taken' }.to_json
    end
  end

  # ---------------------------
  # USER TAGS
  # ---------------------------
  api_doc '/me/tags', method: :get do
    tags 'User', 'Tag'
    description 'List all tags for the current user'
    response 200, 'Returns user’s tags', example: {
      data: [
        { id: 1, name: 'travel' },
        { id: 3, name: 'photography' }
      ]
    }
  end

  get '/me/tags' do
    { data: User.tags(@current_user['id']) }.to_json
  end

  # ---------------------------
  # NEW USER TAG
  # ---------------------------
  api_doc '/me/tags', method: :post do
    tags 'User', 'Tag'
    description 'Add a tag to the current user'
    param :name, String, required: true, desc: "The name of the tag to add, if tag doesn't exist it's created"
    response 200, 'Tag added to user', example: {
      message: 'Tag added',
      data: { id: 4, name: 'sports' }
    }
    response 422, 'Tag name missing or invalid', example: { error: 'Missing tag name' }
  end

  post '/me/tags' do
    data = json_body
    halt 422, { error: 'Missing tag name' }.to_json unless data['name']

    tag = Tag.find_by_name(data['name']) || Tag.create(data['name'])
    UserTag.add_tag(@current_user['id'], tag['id'])
    { message: 'Tag added', data: tag }.to_json
  end

  # ---------------------------
  # DELETE USER TAG
  # ---------------------------
  api_doc '/me/tags', method: :delete do
    tags 'User', 'Tag'
    description 'Remove a tag from the current user'
    param :name, String, required: true, desc: 'The name of the tag to remove'
    response 200, 'Tag removed', example: { message: 'Tag removed' }
    response 422, 'Missing or invalid tag', example: { error: 'Tag not found' }
  end

  delete '/me/tags' do
    data = json_body
    halt 422, { error: 'Missing tag name' }.to_json unless data['name']

    tag = Tag.find_by_name(data['name'])
    halt 422, { error: 'Tag not found' }.to_json unless tag

    UserTag.remove_tag(@current_user['id'], tag['id'])
    { message: 'Tag removed' }.to_json
  end

  # ---------------------------
  # LOOKUP USER TAGS
  # ---------------------------
  api_doc '/users/:username/tags', method: :get do
    tags 'User', 'PublicProfile', 'Tag'
    description 'Fetch the tags of a user by their username'
    param :username, String, required: true, desc: 'The unique username of the user'
    response 200, 'Public user data', example: {
      data: [
        {
          id: 4, name: 'sports'
        },
        {
          id: 5, name: 'cycling'
        }
      ]
    }
    response 404, 'User not found or banned', example: { error: 'User not found' }
    response 404, 'User blocked you', example: { error: 'User blocked you' }
    response 404, 'User is blocked', example: { error: 'User is blocked' }
  end

  get '/users/:username/tags' do
    user = User.find_by_username(params[:username])
    halt 404, { error: 'User not found' }.to_json unless user
    halt 404, { error: 'User not available' }.to_json if user['is_banned'] == 't'
    halt 404, { error: 'User blocked you' }.to_json if BlockedUser.blocked?(user['id'], @current_user['id'])
    halt 404, { error: 'User is blocked' }.to_json if BlockedUser.blocked?(@current_user['id'], user['id'])

    tags = User.tags(user['id'])

    { data: tags }.to_json
  end
end
