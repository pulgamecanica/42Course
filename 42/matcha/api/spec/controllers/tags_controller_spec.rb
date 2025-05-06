# frozen_string_literal: true

require 'spec_helper'

describe 'TagsController' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  def parse_data(body)
    json = JSON.parse(body)
    expect(json).to have_key('data')
    json['data']
  end

  let(:user_data) do
    {
      username: 'tagger',
      email: 'tagger@example.com',
      password: 'taggytag',
      first_name: 'Taggy',
      last_name: 'McTagface',
      gender: 'other',
      sexual_preferences: 'everyone'
    }
  end

  let(:auth_token) do
    User.create(user_data)
    User.confirm!(user_data[:username])
    SessionToken.generate(User.find_by_username(user_data[:username])['id'])
  end

  let(:auth_headers) { headers.merge('HTTP_AUTHORIZATION' => "Bearer #{auth_token}") }

  before do
    auth_token # force user creation
  end

  describe 'GET /tags' do
    it 'returns all tags' do
      Tag.create('ruby')
      Tag.create('sinatra')

      get '/tags', nil, auth_headers
      expect(last_response.status).to eq(200)
      tags = parse_data(last_response.body)
      expect(tags.map { |t| t['name'] }).to include('ruby', 'sinatra')
    end
  end

  describe 'POST /tags' do
    it 'creates a tag with valid name' do
      post '/tags', { name: 'love' }.to_json, auth_headers
      expect(last_response.status).to eq(201)
      tag = parse_data(last_response.body)
      expect(tag['name']).to eq('love')
    end

    it 'fails if name is too short' do
      post '/tags', { name: 'a' }.to_json, auth_headers
      expect(last_response.status).to eq(422)
    end

    it 'fails if tag already exists' do
      Tag.create('unique')
      post '/tags', { name: 'unique' }.to_json, auth_headers
      expect(last_response.status).to eq(422)
    end
  end

  describe 'GET /me/tags' do
    it "returns user's tags" do
      tag = Tag.create('fun')
      user = User.find_by_username(user_data[:username])
      UserTag.add_tag(user['id'], tag['id'])

      get '/me/tags', nil, auth_headers
      expect(last_response.status).to eq(200)
      tags = parse_data(last_response.body)
      expect(tags.map { |t| t['name'] }).to include('fun')
    end
  end

  describe 'POST /me/tags' do
    it 'adds a new tag to user (creating it if necessary)' do
      post '/me/tags', { name: 'vibes' }.to_json, auth_headers
      expect(last_response.status).to eq(200)
      tag = parse_data(last_response.body)
      expect(tag['name']).to eq('vibes')
    end

    it 'does not duplicate existing tag for user' do
      Tag.create('zen')
      post '/me/tags', { name: 'zen' }.to_json, auth_headers
      post '/me/tags', { name: 'zen' }.to_json, auth_headers
      expect(last_response.status).to eq(200)

      get '/me/tags', nil, auth_headers
      tags = parse_data(last_response.body)
      names = tags.map { |t| t['name'] }
      expect(names.count('zen')).to eq(1)
    end
  end

  describe 'DELETE /me/tags' do
    it 'removes an existing tag from the user' do
      tag = Tag.create('remove-me')
      user_id = User.find_by_username(user_data[:username])['id']
      UserTag.add_tag(user_id, tag['id'])

      delete '/me/tags', { name: 'remove-me' }.to_json, auth_headers
      expect(last_response.status).to eq(200)

      get '/me/tags', nil, auth_headers
      tags = parse_data(last_response.body)
      expect(tags.map { |t| t['name'] }).not_to include('remove-me')
    end

    it 'fails to remove a non-existent tag' do
      delete '/me/tags', { name: 'nonexistent' }.to_json, auth_headers
      expect(last_response.status).to eq(422)
    end
  end

  describe 'GET /users/:username/tags' do
    it 'get tags from a user' do
      tag1 = Tag.create('movies')
      tag2 = Tag.create('pizza')
      user_id = User.find_by_username(user_data[:username])['id']
      UserTag.add_tag(user_id, tag1['id'])
      UserTag.add_tag(user_id, tag2['id'])

      get "/users/#{user_data[:username]}/tags", nil, auth_headers
      expect(last_response.status).to eq(200)
      tags = parse_data(last_response.body)
      expect(tags.map { |t| t['name'] }).to include('movies')
      expect(tags.map { |t| t['name'] }).to include('pizza')
    end

    it 'fails to remove a non-existent user' do
      get '/users/nouser/tags', nil, auth_headers
      expect(last_response.status).to eq(404)
    end
  end
end
