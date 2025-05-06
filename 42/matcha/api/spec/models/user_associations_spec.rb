# frozen_string_literal: true

require 'spec_helper'

describe 'User associations' do
  before do
    @alice = User.create({
                           username: 'alice',
                           email: 'alice@example.com',
                           password: 'pass123',
                           first_name: 'Alice',
                           last_name: 'A',
                           gender: 'female',
                           sexual_preferences: 'male'
                         })

    @bob = User.create({
                         username: 'bob',
                         email: 'bob@example.com',
                         password: 'pass123',
                         first_name: 'Bob',
                         last_name: 'B',
                         gender: 'male',
                         sexual_preferences: 'female'
                       })

    User.confirm!(@alice['username'])
    User.confirm!(@bob['username'])
  end

  describe '.tags' do
    it 'returns user tags' do
      tag = Tag.create('curious')
      UserTag.add_tag(@alice['id'], tag['id'])

      tags = User.tags(@alice['id'])
      expect(tags.map { |t| t['name'] }).to include('curious')
    end
  end

  describe '.likes and .liked_by' do
    it 'returns who user liked and who liked the user' do
      Like.like!(@alice['id'], @bob['id'])
      expect(User.likes(@alice['id']).map { |u| u['id'] }).to include(@bob['id'])
      expect(User.liked_by(@bob['id']).map { |u| u['id'] }).to include(@alice['id'])
    end
  end

  describe '.matches' do
    it 'returns mutual likes as matches' do
      Like.like!(@alice['id'], @bob['id'])
      Like.like!(@bob['id'], @alice['id'])

      matches = User.matches(@alice['id'])
      expect(matches.map { |u| u['id'] }).to include(@bob['id'])
    end

    it 'returns empty if no mutual likes' do
      Like.like!(@alice['id'], @bob['id']) # Bob doesn't like back

      matches = User.matches(@alice['id'])
      expect(matches).to be_empty
    end
  end

  describe '.blocked_users and .blocked_by' do
    it 'returns blocked users and who blocked the user' do
      BlockedUser.block!(@alice['id'], @bob['id'])

      blocked = User.blocked_users(@alice['id'])
      blocked_by = User.blocked_by(@bob['id'])

      expect(blocked.map { |u| u['id'] }).to include(@bob['id'])
      expect(blocked_by.map { |u| u['id'] }).to include(@alice['id'])
    end
  end
end
