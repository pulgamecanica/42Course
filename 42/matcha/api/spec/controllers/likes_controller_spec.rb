# frozen_string_literal: true

require 'spec_helper'

describe 'LikesController' do
  def parse_data(body)
    json = JSON.parse(body)
    expect(json).to have_key('data')
    json['data']
  end

  def auth_headers(user)
    token = SessionToken.generate(user['id'])
    { 'CONTENT_TYPE' => 'application/json', 'HTTP_AUTHORIZATION' => "Bearer #{token}" }
  end

  before(:each) do
    User.create({
                  username: 'liker',
                  email: 'liker@example.com',
                  password: 'pass',
                  first_name: 'Liker',
                  last_name: 'User',
                  gender: 'other',
                  sexual_preferences: 'everyone'
                })

    User.create({
                  username: 'liked',
                  email: 'liked@example.com',
                  password: 'pass',
                  first_name: 'Liked',
                  last_name: 'Person',
                  gender: 'female',
                  sexual_preferences: 'male'
                })

    User.confirm!('liker')
    User.confirm!('liked')

    @liker = User.find_by_username('liker')
    @liked = User.find_by_username('liked')
  end

  describe 'POST /me/like' do
    it 'likes another user' do
      post '/me/like', { username: 'liked' }.to_json, auth_headers(@liker)
      expect(last_response.status).to eq(200)
      expect(JSON.parse(last_response.body)['message']).to match(/liked/i)
    end

    it 'returns error if user not found' do
      post '/me/like', { username: 'ghost' }.to_json, auth_headers(@liker)
      expect(last_response.status).to eq(404)
    end

    it 'does not like same user twice' do
      post '/me/like', { username: 'liked' }.to_json, auth_headers(@liker)
      post '/me/like', { username: 'liked' }.to_json, auth_headers(@liker)
      expect(last_response.status).to eq(200)
    end

    it 'does not allow a user to like themselves' do
      post '/me/like', { username: 'liker' }.to_json, auth_headers(@liker)
      expect(last_response.status).to eq(422)
      body = JSON.parse(last_response.body)
      expect(body['error']).to match(/cannot like yourself/i)
    end
  end

  describe 'GET /me/likes' do
    it 'returns users I liked' do
      Like.like!(@liker['id'], @liked['id'])

      get '/me/likes', nil, auth_headers(@liker)
      expect(last_response.status).to eq(200)
      data = parse_data(last_response.body)
      expect(data.map { |u| u['username'] }).to include('liked')
    end
  end

  describe 'GET /me/liked_by' do
    it 'returns users who liked me' do
      Like.like!(@liked['id'], @liker['id'])

      get '/me/liked_by', nil, auth_headers(@liker)
      expect(last_response.status).to eq(200)
      data = parse_data(last_response.body)
      expect(data.map { |u| u['username'] }).to include('liked')
    end
  end

  describe 'GET /me/matches' do
    it 'returns mutual likes (matches)' do
      Like.like!(@liker['id'], @liked['id'])
      Like.like!(@liked['id'], @liker['id'])

      get '/me/matches', nil, auth_headers(@liker)
      expect(last_response.status).to eq(200)
      data = parse_data(last_response.body)
      expect(data.map { |u| u['username'] }).to include('liked')
    end
  end
end
