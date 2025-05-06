# frozen_string_literal: true

require 'spec_helper'
require_relative '../../app/lib/geolocation'

describe 'LocationController' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  def auth_headers(token)
    headers.merge('HTTP_AUTHORIZATION' => "Bearer #{token}")
  end

  before do
    @user = User.create({
                          username: 'geo_user',
                          email: 'geo@example.com',
                          password: 'secretpass',
                          first_name: 'Geo',
                          last_name: 'Tester',
                          gender: 'other',
                          sexual_preferences: 'everyone'
                        })
    User.confirm!(@user['username'])
    @token = SessionToken.generate(@user['id'])
  end

  describe 'POST /me/location' do
    it "updates the user's location using IP geolocation" do
      allow(Geolocation).to receive(:lookup).and_return({
                                                          latitude: 48.8566,
                                                          longitude: 2.3522,
                                                          city: 'Paris',
                                                          country: 'France'
                                                        })

      post '/me/location', {}.to_json, auth_headers(@token)
      expect(last_response.status).to eq(200)
      json = JSON.parse(last_response.body)
      expect(json['message']).to match(/location recorded/i)
      expect(json['data']['latitude'].to_s).to eq(48.8566.to_s)
      expect(json['data']['longitude'].to_s).to eq(2.3522.to_s)
    end

    it 'returns an error when geolocation fails' do
      allow(Geolocation).to receive(:lookup).and_return(nil)

      post '/me/location', {}.to_json, auth_headers(@token)
      expect(last_response.status).to eq(422)
      json = JSON.parse(last_response.body)
      expect(json['error']).to match(/geolocation service failed/i)
    end

    it 'updates the user’s latitude and longitude in the user table' do
      allow(Geolocation).to receive(:lookup).and_return({
                                                          latitude: 40.7128,
                                                          longitude: -74.0060
                                                        })

      post '/me/location', {}.to_json, auth_headers(@token)
      get '/me/location', {}.to_json, auth_headers(@token)
      expect(last_response.status).to eq(200)

      updated_user = User.find_by_id(@user['id'])
      expect(updated_user['latitude'].to_s).to eq(40.7128.to_s)
      expect(updated_user['longitude'].to_s).to eq(-74.0060.to_s)
    end
  end
end
