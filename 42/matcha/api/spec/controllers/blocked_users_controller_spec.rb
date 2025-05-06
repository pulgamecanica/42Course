# frozen_string_literal: true

require 'spec_helper'

describe 'User blocking behavior' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  def create_and_authenticate!(data)
    User.create(data)
    User.confirm!(data[:username])
    user = User.find_by_username(data[:username])
    SessionToken.generate(user['id'])
  end

  def auth_headers(token)
    headers.merge('HTTP_AUTHORIZATION' => "Bearer #{token}")
  end

  let(:alice) do
    { username: 'alice', email: 'alice@example.com', password: 'pass', first_name: 'A', last_name: 'A', gender: 'female',
      sexual_preferences: 'male' }
  end
  let(:bob) do
    { username: 'bob', email: 'bob@example.com', password: 'pass', first_name: 'B', last_name: 'B', gender: 'male',
      sexual_preferences: 'female' }
  end
  let(:carol) do
    { username: 'carol', email: 'carol@example.com', password: 'pass', first_name: 'C', last_name: 'C', gender: 'female',
      sexual_preferences: 'everyone' }
  end
  let(:blocker_data) do
    {
      username: 'blocker',
      email: 'blocker@example.com',
      password: 'securepass',
      first_name: 'Block',
      last_name: 'Her',
      gender: 'female',
      sexual_preferences: 'male'
    }
  end

  let(:victim_data) do
    {
      username: 'victim',
      email: 'victim@example.com',
      password: 'victimpass',
      first_name: 'Victim',
      last_name: 'User',
      gender: 'male',
      sexual_preferences: 'female'
    }
  end

  before do
    @alice_token = create_and_authenticate!(alice)
    @bob_token = create_and_authenticate!(bob)
    @carol_token = create_and_authenticate!(carol)
    @blocker_token = create_and_authenticate!(blocker_data)
    @victim_token = create_and_authenticate!(victim_data)

    # alice blocks bob
    blocker = User.find_by_username('alice')
    blocked = User.find_by_username('bob')
    BlockedUser.block!(blocker['id'], blocked['id'])

    # carol blocks alice
    carol_user = User.find_by_username('carol')
    alice_user = User.find_by_username('alice')
    BlockedUser.block!(carol_user['id'], alice_user['id'])
  end

  describe 'test user access for blocked users' do
    it "prevents bob from accessing alice's profile" do
      get '/users/alice', nil, auth_headers(@bob_token)
      expect(last_response.status).to eq(404)
    end

    it "prevents alice from accessing carol's profile" do
      get '/users/carol', nil, auth_headers(@alice_token)
      expect(last_response.status).to eq(404)
    end

    it 'allows carol to view bob' do
      get '/users/bob', nil, auth_headers(@carol_token)
      expect(last_response.status).to eq(200)
    end

    it 'lists users I blocked' do
      get '/me/blocked', nil, auth_headers(@alice_token)
      expect(last_response.status).to eq(200)
      list = JSON.parse(last_response.body)['data']
      expect(list.map { |u| u['username'] }).to include('bob')
    end

    it 'lists users who blocked me' do
      get '/me/blocked_by', nil, auth_headers(@alice_token)
      expect(last_response.status).to eq(200)
      list = JSON.parse(last_response.body)['data']
      expect(list.map { |u| u['username'] }).to include('carol')
    end
  end

  describe 'POST /me/block' do
    it 'blocks another user' do
      post '/me/block', { username: 'victim' }.to_json, auth_headers(@blocker_token)
      expect(last_response.status).to eq(200)
      json = JSON.parse(last_response.body)
      expect(json['message']).to match(/blocked/i)
      expect(json['data']['username']).to eq('victim')
    end

    it 'returns 404 if user not found' do
      post '/me/block', { username: 'ghost' }.to_json, auth_headers(@blocker_token)
      expect(last_response.status).to eq(404)
    end

    it 'returns 422 if trying to block self' do
      post '/me/block', { username: 'blocker' }.to_json, auth_headers(@blocker_token)
      expect(last_response.status).to eq(422)
      expect(JSON.parse(last_response.body)['error']).to match(/yourself/i)
    end
  end

  describe 'DELETE /me/block' do
    it 'unblocks a previously blocked user' do
      blocker = User.find_by_username('blocker')
      victim = User.find_by_username('victim')
      BlockedUser.block!(blocker['id'], victim['id'])

      # Should return 404 when blocked
      get '/users/victim', nil, auth_headers(@blocker_token)
      expect(last_response.status).to eq(404)

      # Unblock the user
      delete '/me/block', { username: 'victim' }.to_json, auth_headers(@blocker_token)
      expect(last_response.status).to eq(200)
      expect(JSON.parse(last_response.body)['message']).to match(/unblocked/i)

      # Should now be accessible again
      get '/users/victim', nil, auth_headers(@blocker_token)
      expect(last_response.status).to eq(200)
      data = JSON.parse(last_response.body)['data']
      expect(data['username']).to eq('victim')
    end

    it 'returns 404 if user not found' do
      delete '/me/block', { username: 'ghost' }.to_json, auth_headers(@blocker_token)
      expect(last_response.status).to eq(404)
    end
  end
end
