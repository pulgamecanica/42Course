# frozen_string_literal: true

require 'spec_helper'

describe 'PicturesController' do
  let(:headers) { { 'CONTENT_TYPE' => 'application/json' } }

  def auth_headers(token)
    headers.merge('HTTP_AUTHORIZATION' => "Bearer #{token}")
  end

  before(:each) do
    @user = User.create({
                          username: 'picuser', email: 'picuser@example.com', password: 'pass',
                          first_name: 'Pic', last_name: 'User', gender: 'other', sexual_preferences: 'everyone'
                        })
    User.confirm!('picuser')
    @token = SessionToken.generate(@user['id'])
  end

  describe 'POST /me/pictures' do
    it 'uploads a picture' do
      post '/me/pictures', { url: 'https://dummyimage.com/600x400/000/fff.png&text=img' }.to_json, auth_headers(@token)
      expect(last_response.status).to eq(201)

      json = JSON.parse(last_response.body)
      expect(json['data']['url']).to eq('https://dummyimage.com/600x400/000/fff.png&text=img')
    end

    it 'sets a picture as profile' do
      post '/me/pictures', {
        url: 'https://dummyimage.com/600x400/000/fff.png&text=new',
        is_profile: true
      }.to_json, auth_headers(@token)
      expect(last_response.status).to eq(201)

      user = User.find_by_id(@user['id'])
      expect(user['profile_picture_id']).not_to be_nil
    end

    it 'fails with invalid URL' do
      post '/me/pictures', { url: 'invalid-url' }.to_json, auth_headers(@token)
      expect(last_response.status).to eq(422)
    end
  end

  describe 'GET /me/pictures' do
    it "returns the user's pictures" do
      Picture.create(@user['id'], 'https://dummyimage.com/600x400/000/fff.png&text=view')
      get '/me/pictures', nil, auth_headers(@token)
      expect(last_response.status).to eq(200)
      json = JSON.parse(last_response.body)['data']
      expect(json).not_to be_empty
    end
  end

  describe 'PATCH /me/pictures/:id' do
    it "updates a picture's URL" do
      picture = Picture.create(@user['id'], 'https://dummyimage.com/600x400/000/fff.png&text=old')
      patch "/me/pictures/#{picture['id']}", {
        url: 'https://dummyimage.com/600x400/000/fff.png&text=new'
      }.to_json, auth_headers(@token)

      expect(last_response.status).to eq(200)
      updated = Picture.find_by_id(picture['id'])
      expect(updated['url']).to eq('https://dummyimage.com/600x400/000/fff.png&text=new')
    end

    it 'sets the picture as profile' do
      picture = Picture.create(@user['id'], 'https://dummyimage.com/600x400/000/fff.png&text=bar')
      patch "/me/pictures/#{picture['id']}", {
        is_profile: true
      }.to_json, auth_headers(@token)

      user = User.find_by_id(@user['id'])
      expect(user['profile_picture_id']).to eq(picture['id'])
    end
  end

  describe 'DELETE /me/pictures/:id' do
    it 'deletes a picture' do
      picture = Picture.create(@user['id'], 'https://dummyimage.com/600x400/000/fff.png&text=delete')
      delete "/me/pictures/#{picture['id']}", nil, auth_headers(@token)
      expect(last_response.status).to eq(200)

      result = Picture.find_by_id(picture['id'])
      expect(result).to be_nil
    end

    it 'unsets profile_picture_id if profile is deleted' do
      picture = Picture.create(@user['id'], 'https://dummyimage.com/600x400/000/fff.png&text=bye', is_profile: true)
      User.update(@user['id'], { profile_picture_id: picture['id'] })

      delete "/me/pictures/#{picture['id']}", nil, auth_headers(@token)
      user = User.find_by_id(@user['id'])
      expect(user['profile_picture_id']).to be_nil
    end
  end
end
