# frozen_string_literal: true

require 'spec_helper'

describe 'POST /auth/login' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  let(:valid_user_data) do
    {
      username: 'alice42',
      email: 'alice@example.com',
      password: 'secure123',
      first_name: 'Alice',
      last_name: 'Wonder',
      gender: 'female',
      sexual_preferences: 'male'
    }
  end

  before do
    # Create user manually with correct hash
    User.create(valid_user_data)
    User.confirm!(valid_user_data[:username])
  end

  it 'authenticates with correct credentials' do
    post '/auth/login', {
      username: 'alice42',
      password: 'secure123'
    }.to_json, headers

    expect(last_response.status).to eq(200)
    body = JSON.parse(last_response.body)
    expect(body).to have_key('token')
  end

  it 'fails with incorrect password' do
    post '/auth/login', {
      username: 'alice42',
      password: 'wrongpassword'
    }.to_json, headers

    expect(last_response.status).to eq(401)
    expect(JSON.parse(last_response.body)['error']).to match(/invalid/i)
  end

  it 'returns 400 for invalid JSON' do
    post '/auth/login', 'this is not json', { 'CONTENT_TYPE' => 'application/json' }

    expect(last_response.status).to eq(400)
    expect(JSON.parse(last_response.body)['error']).to match(/Invalid JSON payload/i)
  end

  it 'fails if user is not confirmed' do
    unconfirmed_data = valid_user_data.merge(username: 'unconfirmed', email: 'u@example.com')
    User.create(unconfirmed_data)

    post '/auth/login', {
      username: 'unconfirmed',
      password: 'secure123'
    }.to_json, headers

    expect(last_response.status).to eq(403)
    expect(JSON.parse(last_response.body)['error']).to match(/confirm/i)
  end

  it 'fails for non-existent user' do
    post '/auth/login', {
      username: 'ghost',
      password: 'whatever'
    }.to_json, headers

    expect(last_response.status).to eq(401)
    expect(JSON.parse(last_response.body)['error']).to match(/invalid/i)
  end
end
