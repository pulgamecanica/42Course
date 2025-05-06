# frozen_string_literal: true

module UserSerializer
  def self.public_view(user)
    return nil unless user

    {
      'id' => user['id'],
      'username' => user['username'],
      'first_name' => user['first_name'],
      'last_name' => user['last_name'],
      'biography' => user['biography'],
      'gender' => user['gender'],
      'sexual_preferences' => user['sexual_preferences'],
      'birth_year' => user['birth_year'],
      'fame_rating' => user['fame_rating'],
      'latitude' => user['latitude'].to_f.round(6),
      'longitude' => user['longitude'].to_f.round(6),
      'country' => user['country'],
      'city' => user['city'],
      'profile_picture_id' => user['profile_picture_id'],
      'online_status' => user['online_status'] == 't',
      'last_seen_at' => user['last_seen_at']
    }
  end
end
