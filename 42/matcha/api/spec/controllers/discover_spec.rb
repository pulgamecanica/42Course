# frozen_string_literal: true

require 'spec_helper'

describe 'UserController -> Discover' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  def auth_headers(token)
    headers.merge('HTTP_AUTHORIZATION' => "Bearer #{token}")
  end

  before do
    @alice = User.create({
                           username: 'alice',
                           email: 'alice@example.com',
                           password: 'pass',
                           first_name: 'Alice',
                           last_name: 'A',
                           gender: 'female',
                           sexual_preferences: 'male',
                           birth_year: 2000
                         })
    User.confirm!('alice')
    @alice_token = SessionToken.generate(@alice['id'])

    @bob = User.create({
                         username: 'bob',
                         email: 'bob@example.com',
                         password: 'pass',
                         first_name: 'Bob',
                         last_name: 'B',
                         gender: 'male',
                         sexual_preferences: 'female',
                         birth_year: 1995,
                         latitude: 19.43,
                         longitude: -99.13
                       })
    User.confirm!('bob')
    @bob_token = SessionToken.generate(@bob['id'])

    UserTag.add_tag(@alice['id'], Tag.create('music')['id'])
    UserTag.add_tag(@bob['id'], Tag.find_by_name('music')['id'])
  end

  it 'returns a discovered user with matching gender and preference' do
    post '/me/discover', { tags: ['music'] }.to_json, auth_headers(@alice_token)
    expect(last_response.status).to eq(200)
    data = JSON.parse(last_response.body)['data']
    expect(data.first['user']['username']).to eq('bob')
    expect(data.first['score']['total']).to be > 0
  end

  it 'filters by age correctly' do
    post '/me/discover', {
      min_age: 28,
      max_age: 32
    }.to_json, auth_headers(@alice_token)

    expect(last_response.status).to eq(200)
    data = JSON.parse(last_response.body)['data']
    expect(data.map { |u| u['user']['username'] }).to include('bob')
  end

  it 'filters out users outside age range' do
    post '/me/discover', {
      min_age: 40,
      max_age: 45
    }.to_json, auth_headers(@alice_token)

    expect(last_response.status).to eq(200)
    data = JSON.parse(last_response.body)['data']
    expect(data).to be_empty
  end

  it 'prioritizes location if provided' do
    post '/me/location', {
      latitude: 19.43,
      longitude: -99.13,
      city: 'Paris',
      country: 'France'
    }.to_json, auth_headers(@alice_token)
    post '/me/location', {
      latitude: 19.43,
      longitude: -99.13,
      city: 'Paris',
      country: 'France'
    }.to_json, auth_headers(@bob_token)
    post '/me/discover', {
      location: {
        latitude: 19.43,
        longitude: -99.13,
        max_distance_km: 10
      }
    }.to_json, auth_headers(@alice_token)

    expect(last_response.status).to eq(200)
    data = JSON.parse(last_response.body)['data']
    expect(data.first['user']['username']).to eq('bob')
    expect(data.first['score']['total']).to be > 33
  end
end
