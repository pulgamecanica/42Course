# frozen_string_literal: true

require 'spec_helper'

describe 'NotificationsController' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  def auth_headers(token)
    headers.merge('HTTP_AUTHORIZATION' => "Bearer #{token}")
  end

  before do
    @alice = User.create({
                           username: 'alice', email: 'alice@example.com', password: 'password',
                           first_name: 'Alice', last_name: 'A', gender: 'female', sexual_preferences: 'male'
                         })
    @bob = User.create({
                         username: 'bob', email: 'bob@example.com', password: 'password',
                         first_name: 'Bob', last_name: 'B', gender: 'male', sexual_preferences: 'female'
                       })

    User.confirm!('alice')
    User.confirm!('bob')

    @alice_token = SessionToken.generate(@alice['id'])
    @bob_token = SessionToken.generate(@bob['id'])
  end

  it 'returns a list of notifications for the current user' do
    Notification.create(@alice['id'], 'You have a new like!', from_user_id: @bob['id'])

    get '/me/notifications', nil, auth_headers(@alice_token)
    expect(last_response.status).to eq(200)
    data = JSON.parse(last_response.body)['data']
    expect(data.last['message']).to include('like')
    Notification.create(@alice['id'], 'Someone viewed your profile')
    get '/me/notifications', nil, auth_headers(@alice_token)
    data = JSON.parse(last_response.body)['data']
    expect(data.size).to eq(2)
  end

  it 'returns an empty array if no notifications exist' do
    get '/me/notifications', nil, auth_headers(@bob_token)
    expect(last_response.status).to eq(200)
    expect(JSON.parse(last_response.body)['data']).to eq([])
  end

  it 'returns 401 if user is not authenticated' do
    get '/me/notifications'
    expect(last_response.status).to eq(401)
  end
end
