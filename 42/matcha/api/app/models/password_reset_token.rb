# frozen_string_literal: true

require 'securerandom'

class PasswordResetToken
  def self.generate_for(user_id)
    token = SecureRandom.hex(24)
    expires_at = Time.now + 60 * 60 # 1 hour

    Database.with_conn do |conn|
      conn.exec_params(<<~SQL, [user_id, token, expires_at])
        INSERT INTO password_reset_tokens (user_id, token, expires_at)
        VALUES ($1, $2, $3)
        RETURNING *
      SQL
    end&.first
  end

  def self.find_valid(token)
    Database.with_conn do |conn|
      conn.exec_params(<<~SQL, [token, Time.now])
        SELECT * FROM password_reset_tokens
        WHERE token = $1 AND used = FALSE AND expires_at > $2
      SQL
    end&.first
  end

  def self.mark_used(token)
    Database.with_conn do |conn|
      conn.exec_params('UPDATE password_reset_tokens SET used = TRUE WHERE token = $1', [token])
    end
  end
end
