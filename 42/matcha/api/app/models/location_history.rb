# frozen_string_literal: true

require_relative '../helpers/database'
require_relative '../helpers/sql_helper'
require_relative '../lib/geolocation'

class LocationHistory
  def self.record(user_id, ip_address, user_agent, location: nil)
    location ||= Geolocation.lookup(ip_address)
    raise Errors::ValidationError, 'Geolocation service failed' unless location

    location_history = SQLHelper.create(:location_history, {
                                          user_id: user_id,
                                          latitude: location[:latitude],
                                          longitude: location[:longitude],
                                          city: location[:city],
                                          country: location[:country],
                                          ip_address: ip_address,
                                          user_agent: user_agent,
                                          created_at: Time.now
                                        }, %w[user_id latitude longitude city country ip_address user_agent created_at])
    User.update(user_id, {
                  latitude: location[:latitude],
                  longitude: location[:longitude],
                  country: location[:country],
                  city: location[:city]
                })
    location_history
  end

  def self.for_user(user_id)
    Database.with_conn do |conn|
      conn.exec_params(<<~SQL, [user_id]).to_a
        SELECT * FROM location_history
        WHERE user_id = $1
        ORDER BY created_at DESC
      SQL
    end
  end
end
