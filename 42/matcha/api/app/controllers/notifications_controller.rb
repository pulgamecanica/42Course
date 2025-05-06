# frozen_string_literal: true

require_relative './base_controller'
require_relative '../models/notification'
require_relative '../models/user'

class NotificationsController < BaseController
  # ---------------------------
  # GET /me/notifications
  # ---------------------------
  api_doc '/me/notifications', method: :get do
    tags 'User', 'Notifications'
    description 'List notifications for the current user'
    response 200, 'List of notifications', example: {
      data: [
        {
          id: 1,
          message: 'You have a new like!',
          read: false,
          from_username: 'bob',
          created_at: '2025-04-13T10:00:00Z'
        }
      ]
    }
  end

  get '/me/notifications' do
    { data: User.notifications(@current_user['id']) }.to_json
  end

  # ---------------------------
  # PATCH /me/notifications/:id
  # ---------------------------
  api_doc '/me/notifications/:id', method: :patch do
    tags 'User', 'Notifications'
    description 'Mark a notification as read'
    param :id, Integer, required: true, desc: 'Notification ID'
    response 200, 'Notification marked as read', example: {
      message: 'Notification marked as read'
    }
    response 404, 'Notification not found', example: { error: 'Notification not found' }
  end

  patch '/me/notifications/:id' do
    notification = User.notifications(@current_user['id']).find { |n| n['id'].to_i == params[:id].to_i }
    halt 404, { error: 'Notification not found' }.to_json unless notification

    Notification.mark_as_read(params[:id])
    { message: 'Notification marked as read' }.to_json
  end

  # ---------------------------
  # DELETE /me/notifications/:id
  # ---------------------------
  api_doc '/me/notifications/:id', method: :delete do
    tags 'User', 'Notifications'
    description 'Delete a notification'
    param :id, Integer, required: true, desc: 'Notification ID'
    response 200, 'Notification deleted', example: {
      message: 'Notification deleted'
    }
    response 404, 'Notification not found', example: { error: 'Notification not found' }
  end

  delete '/me/notifications/:id' do
    notification = User.notifications(@current_user['id']).find { |n| n['id'].to_i == params[:id].to_i }
    halt 404, { error: 'Notification not found' }.to_json unless notification

    Notification.delete(params[:id])
    { message: 'Notification deleted' }.to_json
  end
end
