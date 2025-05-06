# frozen_string_literal: true

require_relative './base_controller'
require_relative '../models/user'

class ProfileViewsController < BaseController
  # ---------------------------
  # WHO VISITED ME
  # ---------------------------
  api_doc '/me/visits', method: :get do
    tags 'User', 'ProfileView'
    description 'See who has viewed your profile'
    response 200, 'List of users who viewed you', example: {
      data: [
        { username: 'janedoe', first_name: 'Jane', last_name: 'Doe', viewed_at: '2025-04-12T10:00:00Z' },
        { username: 'bobsmith', first_name: 'Bob', last_name: 'Smith', viewed_at: '2025-04-11T16:40:00Z' }
      ]
    }
  end

  get '/me/visits' do
    visitors = User.visitors_for(@current_user['id'])
    { data: visitors }.to_json
  end

  # ---------------------------
  # WHO I VISITED
  # ---------------------------
  api_doc '/me/views', method: :get do
    tags 'User', 'ProfileView'
    description 'See which users you have viewed'
    response 200, 'List of profiles you viewed', example: {
      data: [
        { username: 'alicewonder', first_name: 'Alice', last_name: 'Wonder', viewed_at: '2025-04-12T09:00:00Z' }
      ]
    }
  end

  get '/me/views' do
    visited = User.views(@current_user['id'])
    { data: visited }.to_json
  end
end
