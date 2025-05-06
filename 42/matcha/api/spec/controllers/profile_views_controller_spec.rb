# frozen_string_literal: true

require 'spec_helper'

describe 'ProfileViewsController' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  def auth_headers(token)
    headers.merge('HTTP_AUTHORIZATION' => "Bearer #{token}")
  end

  before(:each) do
    @alice = User.create({
                           username: 'alice', email: 'alice@example.com', password: 'pass',
                           first_name: 'Alice', last_name: 'A', gender: 'female', sexual_preferences: 'male'
                         })
    @bob = User.create({
                         username: 'bob', email: 'bob@example.com', password: 'pass',
                         first_name: 'Bob', last_name: 'B', gender: 'male', sexual_preferences: 'female'
                       })

    User.confirm!('alice')
    User.confirm!('bob')

    @alice_token = SessionToken.generate(@alice['id'])
    @bob_token   = SessionToken.generate(@bob['id'])
  end

  it 'records a profile view when Bob visits Alice' do
    get '/users/alice', nil, auth_headers(@bob_token)
    expect(last_response.status).to eq(200)
    visitors = User.views(@alice['id'])
    expect(visitors).not_to be_empty
    expect(visitors.first['username']).to eq('bob')
  end

  it 'shows who I visited' do
    get '/users/alice', nil, auth_headers(@bob_token)

    get '/me/visits', nil, auth_headers(@bob_token)
    expect(last_response.status).to eq(200)
    users = JSON.parse(last_response.body)['data']
    expect(users).not_to be_empty
    expect(users.first['username']).to eq('alice')
  end

  it 'shows who visits my profile' do
    get '/users/bob', nil, auth_headers(@alice_token)

    get '/me/views', nil, auth_headers(@bob_token)
    expect(last_response.status).to eq(200)
    users = JSON.parse(last_response.body)['data']
    expect(users).not_to be_empty
    expect(users.first['username']).to eq('alice')
  end
end
