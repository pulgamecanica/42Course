# frozen_string_literal: true

require 'spec_helper'

describe 'ConnectionsController' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  def auth_headers(token)
    headers.merge('HTTP_AUTHORIZATION' => "Bearer #{token}")
  end

  before do
    @alice = User.create({
                           username: 'alice',
                           email: 'alice@example.com',
                           password: 'password123',
                           first_name: 'Alice',
                           last_name: 'Wonder',
                           gender: 'female',
                           sexual_preferences: 'male'
                         })
    @bob = User.create({
                         username: 'bob',
                         email: 'bob@example.com',
                         password: 'password123',
                         first_name: 'Bob',
                         last_name: 'Builder',
                         gender: 'male',
                         sexual_preferences: 'female'
                       })

    alice_profile = Picture.create(@alice['id'], 'https://example.com', is_profile: true)
    bob_profile = Picture.create(@bob['id'], 'https://example.com', is_profile: true)
    User.update(@alice['id'], { profile_picture_id: alice_profile['id'] })
    User.update(@bob['id'], { profile_picture_id: bob_profile['id'] })
    User.confirm!('alice')
    User.confirm!('bob')

    @alice_token = SessionToken.generate(@alice['id'])
    @bob_token = SessionToken.generate(@bob['id'])
  end

  describe 'POST /me/connect' do
    it 'creates a connection between matched users' do
      Like.like!(@alice['id'], @bob['id'])
      Like.like!(@bob['id'], @alice['id'])

      post '/me/connect', { username: 'bob' }.to_json, auth_headers(@alice_token)
      expect(last_response.status).to eq(200)
      json = JSON.parse(last_response.body)
      expect(json['message']).to match(/connected/i)
    end

    it 'does not allow connection to unmatched user' do
      Like.like!(@alice['id'], @bob['id']) # Bob didn't like back

      post '/me/connect', { username: 'bob' }.to_json, auth_headers(@alice_token)
      expect(last_response.status).to eq(403)
      json = JSON.parse(last_response.body)
      expect(json['error']).to match(/user is not matched with you/i)
    end

    it 'does not allow self-connections' do
      post '/me/connect', { username: 'alice' }.to_json, auth_headers(@alice_token)
      expect(last_response.status).to eq(403)
    end
  end

  describe 'GET /me/connections' do
    it 'lists all connections for the current user' do
      Like.like!(@alice['id'], @bob['id'])
      Like.like!(@bob['id'], @alice['id'])
      Connection.create(@alice['id'], @bob['id'])

      get '/me/connections', nil, auth_headers(@alice_token)
      expect(last_response.status).to eq(200)
      data = JSON.parse(last_response.body)['data']
      usernames = data.map { |u| u['username'] }
      expect(usernames).to include(@bob['username'])

      get '/me/connections', nil, auth_headers(@bob_token)
      expect(last_response.status).to eq(200)
      data = JSON.parse(last_response.body)['data']
      usernames = data.map { |u| u['username'] }
      expect(usernames).to include(@alice['username'])
    end
  end

  describe 'DELETE /me/connect' do
    before do
      Like.like!(@alice['id'], @bob['id'])
      Like.like!(@bob['id'], @alice['id'])
      Connection.create(@alice['id'], @bob['id'])
    end

    it 'successfully disconnects a connection if it exists' do
      delete '/me/connect', { username: 'bob' }.to_json, auth_headers(@alice_token)
      expect(last_response.status).to eq(200)
      body = JSON.parse(last_response.body)
      expect(body['message']).to match(/disconnected/i)

      # Ensure connection no longer appears in list
      get '/me/connections', nil, auth_headers(@alice_token)
      data = JSON.parse(last_response.body)['data']
      usernames = data.map { |u| u['username'] }
      expect(usernames).not_to include('bob')
    end

    it 'returns 403 if trying to disconnect someone not connected' do
      Connection.delete_between(@alice['id'], @bob['id']) # pre-remove
      delete '/me/connect', { username: 'bob' }.to_json, auth_headers(@alice_token)
      expect(last_response.status).to eq(403)
      body = JSON.parse(last_response.body)
      expect(body['error']).to match(/not connected/i)
    end
  end
end
