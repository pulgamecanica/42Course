# frozen_string_literal: true

require 'spec_helper'

describe 'MessagesController' do
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

    Like.like!(@alice['id'], @bob['id'])
    Like.like!(@bob['id'], @alice['id'])
    @conn = Connection.create(@alice['id'], @bob['id'])

    @alice_token = SessionToken.generate(@alice['id'])
    @bob_token = SessionToken.generate(@bob['id'])
  end

  describe 'POST /me/messages' do
    it 'sends a message to a connected user' do
      payload = { username: 'bob', content: 'Hey there!' }.to_json
      post '/me/messages', payload, auth_headers(@alice_token)
      expect(last_response.status).to eq(200)
      json = JSON.parse(last_response.body)
      expect(json['message']).to match(/sent/i)
    end

    it 'does not send a message to unconnected user' do
      User.create({
                    username: 'stranger', email: 'stranger@example.com', password: 'pass',
                    first_name: 'Strange', last_name: 'Guy', gender: 'male', sexual_preferences: 'everyone'
                  })
      payload = { username: 'stranger', content: 'Nope' }.to_json
      post '/me/messages', payload, auth_headers(@alice_token)
      expect(last_response.status).to eq(404)
    end
  end

  describe 'GET /me/messages/:username' do
    it 'retrieves conversation between two users' do
      Message.create(@conn['id'], @bob['id'], 'Hi Bob!')
      Message.create(@conn['id'], @alice['id'], 'Hey Alice!')

      get '/me/messages/bob', nil, auth_headers(@alice_token)
      expect(last_response.status).to eq(200)
      data = JSON.parse(last_response.body)['data']
      expect(data).to have_key('user')
      expect(data).to have_key('messages')
      messages = data['messages']
      expect(messages).to be_an(Array)
      expect(messages.map { |m| m['content'] }).to include('Hi Bob!', 'Hey Alice!')
    end
  end

  describe 'GET /me/messages' do
    it 'lists all conversations grouped by user' do
      Message.create(@conn['id'], @bob['id'], 'Hello again!')
      Message.create(@conn['id'], @alice['id'], 'Hey again!')

      get '/me/messages', nil, auth_headers(@alice_token)
      expect(last_response.status).to eq(200)
      json = JSON.parse(last_response.body)['data']
      expect(json).to be_an(Array)
      expect(json.first).to have_key('user')
      expect(json.first).to have_key('messages')
    end
  end
end
