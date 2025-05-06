# frozen_string_literal: true

require 'spec_helper'

describe 'User Endpoints' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  def create_and_authenticate!(data)
    User.create(data)
    User.confirm!(data[:username])
    user = User.find_by_username(data[:username])
    [user, SessionToken.generate(user['id'])]
  end

  def auth_headers(token)
    headers.merge('HTTP_AUTHORIZATION' => "Bearer #{token}")
  end

  describe 'GET /me' do
    let(:user_data) do
      {
        username: 'authme',
        email: 'authme@example.com',
        password: 'supersecret',
        first_name: 'Authy',
        last_name: 'Tester',
        gender: 'other',
        sexual_preferences: 'everyone'
      }
    end

    before do
      @user, @token = create_and_authenticate!(user_data)
    end

    it 'returns the current authenticated user' do
      get '/me', nil, auth_headers(@token)

      expect(last_response.status).to eq(200)
      json = JSON.parse(last_response.body)
      expect(json).to have_key('data')
      user = json['data']
      expect(user['username']).to eq('authme')
      expect(user).not_to have_key('password_digest')
    end

    it 'returns 401 if token is missing' do
      get '/me', nil, headers
      expect(last_response.status).to eq(401)
    end

    it 'returns 401 if token is invalid' do
      get '/me', nil, auth_headers('Bearer invalid.token')
      expect(last_response.status).to eq(401)
    end

    it 'returns 403 if user is banned' do
      User.ban!(user_data[:username])
      get '/me', nil, auth_headers(@token)
      expect(last_response.status).to eq(403)
    end

    it 'returns 403 if user is not confirmed' do
      unconfirmed = user_data.merge(username: 'ghost', email: 'g@example.com')
      User.create(unconfirmed)
      user = User.find_by_username('ghost')
      token = SessionToken.generate(user['id'])

      get '/me', nil, auth_headers(token)
      expect(last_response.status).to eq(403)
    end
  end

  describe 'PATCH /me' do
    let(:user_data) do
      {
        username: 'patchme',
        email: 'patchme@example.com',
        password: 'securepass',
        first_name: 'Patch',
        last_name: 'User',
        gender: 'female',
        sexual_preferences: 'male'
      }
    end

    let(:user) { User.find_by_username('patchme') }
    let(:token) { SessionToken.generate(user['id']) }

    before do
      User.create(user_data)
      User.confirm!('patchme')
    end

    it 'updates allowed fields' do
      patch '/me', {
        first_name: 'Updated',
        biography: 'Updated bio'
      }.to_json, auth_headers(token)

      expect(last_response.status).to eq(200)
      json = JSON.parse(last_response.body)
      expect(json).to have_key('data')
      user = json['data']
      expect(user['first_name']).to eq('Updated')
      expect(user['biography']).to eq('Updated bio')
    end

    it 'returns 422 for disallowed field' do
      patch '/me', { password_digest: 'nope' }.to_json, auth_headers(token)
      expect(last_response.status).to eq(422)
    end

    it 'returns 401 if not authenticated' do
      patch '/me', { first_name: 'x' }.to_json, headers
      expect(last_response.status).to eq(401)
    end

    it 'returns 422 with invalid enum' do
      patch '/me', { gender: 'alien' }.to_json, auth_headers(token)
      expect(last_response.status).to eq(422)
      expect(JSON.parse(last_response.body)['details'].join).to match(/gender/i)
    end
  end

  describe 'GET /users/:username' do
    let(:user_data) do
      {
        username: 'bob',
        email: 'bob@example.com',
        password: 'pass123',
        first_name: 'Bob',
        last_name: 'Builder',
        gender: 'male',
        sexual_preferences: 'female',
        biography: 'Just building things',
        profile_picture_id: 1
      }
    end

    before do
      @user, @token = create_and_authenticate!(user_data)
      unbaned_user_data = user_data.merge(username: 'ghost', email: 'g@example.com')
      @user2, @token2 = create_and_authenticate!(unbaned_user_data)
    end

    it 'returns the public profile' do
      get '/users/bob', nil, auth_headers(@token)
      expect(last_response.status).to eq(200)
      json = JSON.parse(last_response.body)
      expect(json).to have_key('data')
      user = json['data']
      expect(user['username']).to eq('bob')
      expect(user['email']).to be_nil
    end

    it 'returns 404 for unknown user' do
      get '/users/polly', nil, auth_headers(@token)
      expect(last_response.status).to eq(404)
    end

    it 'returns 404 for banned user' do
      User.ban!('bob')
      get '/users/bob', nil, auth_headers(@token2) # we need to use a valid token after banning bob
      expect(last_response.status).to eq(404)
    end
  end

  describe 'DELETE /me' do
    let(:user_data) do
      {
        username: 'deleteme',
        email: 'deleteme@example.com',
        password: 'secure123',
        first_name: 'Del',
        last_name: 'Me',
        gender: 'other',
        sexual_preferences: 'everyone'
      }
    end

    before do
      @user, @token = create_and_authenticate!(user_data)
    end

    it 'deletes the authenticated user' do
      delete '/me', nil, auth_headers(@token)
      expect(last_response.status).to eq(204)

      user = User.find_by_id(@user['id'])
      expect(user).to be_nil
    end

    it 'returns 401 without a token' do
      delete '/me', nil, headers
      expect(last_response.status).to eq(401)
    end
  end
end
