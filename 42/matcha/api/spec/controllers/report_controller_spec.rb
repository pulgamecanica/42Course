# frozen_string_literal: true

require 'spec_helper'

describe 'ReportsController' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  def auth_headers(token)
    headers.merge('HTTP_AUTHORIZATION' => "Bearer #{token}")
  end

  before do
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

  it 'allows a user to report another user successfully' do
    post '/me/report', { username: 'bob', reason: 'scam' }.to_json, auth_headers(@alice_token)

    expect(last_response.status).to eq(200)
    body = JSON.parse(last_response.body)
    expect(body['message']).to eq('User reported successfully')
    expect(body['data']['username']).to eq('bob')
  end

  it 'does not allow reporting a non-existing user' do
    post '/me/report', { username: 'nonexistent', reason: 'fake_account' }.to_json, auth_headers(@alice_token)

    expect(last_response.status).to eq(404)
    body = JSON.parse(last_response.body)
    expect(body['error']).to eq('User not found')
  end

  it 'does not allow a user to report themselves' do
    post '/me/report', { username: 'alice', reason: 'scam' }.to_json, auth_headers(@alice_token)

    expect(last_response.status).to eq(422)
    body = JSON.parse(last_response.body)
    expect(body['error']).to eq('You cannot report yourself')
  end

  it 'validates the reason field (invalid enum)' do
    post '/me/report', { username: 'bob', reason: 'invalid_reason' }.to_json, auth_headers(@alice_token)

    expect(last_response.status).to eq(422)
    body = JSON.parse(last_response.body)
    expect(body['error']).to include('Validation failed')
  end

  it 'validates missing fields (missing username)' do
    post '/me/report', { reason: 'scam' }.to_json, auth_headers(@alice_token)

    expect(last_response.status).to eq(422)
    body = JSON.parse(last_response.body)
    expect(body['error']).to include('Validation failed')
  end
end
