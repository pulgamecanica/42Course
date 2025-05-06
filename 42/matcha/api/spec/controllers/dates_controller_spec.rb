# frozen_string_literal: true

require 'spec_helper'

describe 'DatesController' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  def auth_headers(token)
    headers.merge('HTTP_AUTHORIZATION' => "Bearer #{token}")
  end

  before do
    @alice = User.create({
                           username: 'alice', email: 'alice@example.com', password: 'secret',
                           first_name: 'Alice', last_name: 'Wonder', gender: 'female', sexual_preferences: 'male'
                         })
    @bob = User.create({
                         username: 'bob', email: 'bob@example.com', password: 'secret',
                         first_name: 'Bob', last_name: 'Builder', gender: 'male', sexual_preferences: 'female'
                       })
    User.confirm!(@alice['username'])
    User.confirm!(@bob['username'])

    @alice_token = SessionToken.generate(@alice['id'])
    @bob_token = SessionToken.generate(@bob['id'])

    Like.like!(@alice['id'], @bob['id'])
    Like.like!(@bob['id'], @alice['id'])
    @conn = Connection.create(@alice['id'], @bob['id'])
  end

  it 'creates a date request' do
    payload = {
      username: 'bob',
      scheduled_at: (Time.now + 3600).to_s,
      location: 'Central Park',
      note: 'Wanna hang?'
    }

    post '/me/dates', payload.to_json, auth_headers(@alice_token)
    expect(last_response.status).to eq(201)
    body = JSON.parse(last_response.body)
    expect(body['message']).to match(/date scheduled/i)
    expect(body['data']['location']).to eq('Central Park')
  end

  it 'fails to create a date if not connected' do
    User.create({
                  username: 'eve', email: 'eve@example.com', password: 'secret',
                  first_name: 'Eve', last_name: 'Smith', gender: 'female', sexual_preferences: 'male'
                })
    User.confirm!('eve')

    payload = {
      username: 'eve',
      scheduled_at: (Time.now + 3600).iso8601,
      location: 'Park',
      message: 'Hey'
    }

    post '/me/dates', payload.to_json, auth_headers(@alice_token)
    expect(last_response.status).to eq(404)
  end

  it 'lists all dates for current user' do
    Date.create(@conn['id'], @alice['id'], 'Coffee Shop', (Time.now + 7200).iso8601, 'Coffee?')

    get '/me/dates', nil, auth_headers(@alice_token)
    expect(last_response.status).to eq(200)
    body = JSON.parse(last_response.body)
    expect(body['data']).not_to be_empty
    expect(body['data'].first['location']).to eq('Coffee Shop')
  end

  it 'allows sender to delete a date' do
    date = Date.create(@conn['id'], @bob['id'], 'Somewhere', (Time.now + 7200).iso8601, 'See you')

    delete "/me/dates/#{date['id']}", nil, auth_headers(@alice_token)
    expect(last_response.status).to eq(200)
    expect(JSON.parse(last_response.body)['message']).to match(/deleted/i)
  end

  it 'prevents unrelated user from deleting a date' do
    charlie = User.create({
                            username: 'charlie', email: 'charlie@example.com', password: 'secret',
                            first_name: 'Charlie', last_name: 'Brown', gender: 'male', sexual_preferences: 'female'
                          })
    User.confirm!('charlie')
    charlie_token = SessionToken.generate(charlie['id'])

    date = Date.create(@conn['id'], @alice['id'], 'Coffee Shop', (Time.now + 7200).iso8601, 'Coffee?')

    delete "/me/dates/#{date['id']}", nil, auth_headers(charlie_token)
    expect(last_response.status).to eq(403)
  end
end
