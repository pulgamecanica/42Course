# frozen_string_literal: true

require 'spec_helper'
require_relative '../../app/helpers/sql_helper'

describe SQLHelper do
  let(:table) { :users }

  let(:valid_data) do
    {
      username: 'helper_tester',
      email: 'sql_helper@example.com',
      password_digest: 'hashedpassword',
      first_name: 'SQL',
      last_name: 'Helper',
      gender: 'male',
      sexual_preferences: 'female'
    }
  end

  let(:allowed_fields) do
    %w[username email password_digest first_name last_name gender sexual_preferences]
  end

  describe '.create and .find_by_id' do
    it 'inserts and retrieves the record' do
      user = SQLHelper.create(table, valid_data, allowed_fields)
      expect(user['username']).to eq('helper_tester')

      found = SQLHelper.find_by_id(table, user['id'])
      expect(found['email']).to eq('sql_helper@example.com')
    end
  end

  describe '.find_by' do
    it 'finds the record by username' do
      SQLHelper.create(table, valid_data, allowed_fields)
      result = SQLHelper.find_by(table, :username, 'helper_tester')
      expect(result['email']).to eq('sql_helper@example.com')
    end
  end

  describe '.update' do
    it 'updates the user’s first name' do
      user = SQLHelper.create(table, valid_data, allowed_fields)

      updated = SQLHelper.update(table, user['id'], { 'first_name' => 'Updated' }, %w[first_name])
      expect(updated['first_name']).to eq('Updated')
    end
  end

  describe '.update_column' do
    it 'updates a single column using conditions' do
      SQLHelper.create(table, valid_data, allowed_fields)

      SQLHelper.update_column(table, :is_banned, true, { username: 'helper_tester' })
      result = SQLHelper.find_by(table, :username, 'helper_tester')
      expect(result['is_banned']).to eq('t')
    end
  end

  describe '.delete' do
    it 'deletes the user' do
      user = SQLHelper.create(table, valid_data, allowed_fields)
      deleted = SQLHelper.delete(table, user['id'])
      expect(deleted.cmd_tuples).to eq(1)

      result = SQLHelper.find_by_id(table, user['id'])
      expect(result).to be_nil
    end
  end
end
