# frozen_string_literal: true

require 'spec_helper'

describe 'POST /auth/register' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  it 'creates a user' do
    post '/auth/register', {
      username: 'alice',
      email: 'alice@example.com',
      password: 'secret',
      first_name: 'Alice',
      last_name: 'Liddell'
    }.to_json, headers

    expect(last_response.status).to eq(201)
  end

  it 'returns 422 when fields are missing' do
    post '/auth/register', { email: 'x@y.com' }.to_json, headers
    expect(last_response.status).to eq(422)
  end

  context 'when duplicate username is used' do
    let(:valid_payload) do
      {
        username: 'duplicate_me',
        email: 'original@example.com',
        password: 'secure123',
        first_name: 'First',
        last_name: 'User'
      }
    end

    it 'fails with 422 if username is taken' do
      post '/auth/register', valid_payload.to_json, headers
      expect(last_response.status).to eq(201)

      post '/auth/register', valid_payload.merge(email: 'other@email.com').to_json, headers
      expect(last_response.status).to eq(422)
      expect(JSON.parse(last_response.body)['error']).to match(/username/i)
    end
  end
end
