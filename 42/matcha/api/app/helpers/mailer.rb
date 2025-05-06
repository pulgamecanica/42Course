# frozen_string_literal: true

require 'logger'
require 'mail'

class Mailer
  FROM_EMAIL = 'pulgamecanica@matcha.com'
  DOMAIN = ENV.fetch('APP_ENV', 'development') == 'development' ? 'http://localhost:5173' : 'http://localhost:4173'

  def self.setup
    Mail.defaults do
      delivery_method :smtp, {
        address: ENV.fetch('SMTP_ADDRESS', 'smtp.example.com'),
        port: ENV.fetch('SMTP_PORT', 1025),
        user_name: ENV.fetch('SMTP_USERNAME', 'username'),
        password: ENV.fetch('SMTP_PASSWORD', 'password'),
        authentication: :login,
        enable_starttls_auto: true
      }
    end
  end

  def self.send_email(to, subject, html_content)
    setup
    Mail.deliver do
      from     FROM_EMAIL
      to       to
      subject  subject

      html_part do
        content_type 'text/html; charset=UTF-8'
        body html_content
      end
    end
  rescue StandardError => e
    LOGGER.error("Failed to send confirmation email to #{to}[#{subject}]: #{e.class} - #{e.message}")
  end

  def self.send_confirmation_email(user_email, code)
    html = confirmation_template(code)
    send_email(user_email, 'Confirm your Matcha account', html)
  end

  def self.send_password_reset_email(user_email, code)
    html = reset_password_template(code)
    send_email(user_email, 'Reset your Matcha password', html)
  end

  def self.confirmation_template(code)
    <<~HTML
      <!DOCTYPE html>
      <html lang="en">
      <body style="font-family: Arial, sans-serif; background-color: #f9fafb; margin: 0; padding: 0;">
        <div style="max-width: 600px; margin: auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0px 0px 10px rgba(0,0,0,0.1);">
          <div style="text-align: center;">
            <!--<img src="https://via.placeholder.com/150x80?text=Matcha+Logo" alt="Matcha Logo" style="margin-bottom: 20px;" />-->
            <h1 style="color: #4f46e5;">Welcome to Matcha! 🌸</h1>
            <p style="font-size: 16px; color: #4b5563;">Thanks for joining <b>Matcha</b>!</p>
            <p style="font-size: 16px; color: #4b5563;">Click the button to confirm your account:</p>
            <a href="#{DOMAIN}/confirm?code=#{code}"
              style="display: inline-block; margin-top: 20px; background-color: #10b981; color: white; padding: 14px 24px; border-radius: 6px; text-decoration: none; font-size: 16px;">
              Confirm my account
            </a>
          </div>
        </div>
        <p style="text-align: center; font-size: 10px; color: #9ca3af; margin-top: 20px;">
          Sent with ❤️ from Matcha | pulgamecanica@matcha.com
        </p>
      </body>
      </html>
    HTML
  end

  def self.reset_password_template(code)
    <<~HTML
      <!DOCTYPE html>
      <html lang="en">
      <body style="font-family: Arial, sans-serif; background-color: #f9fafb; margin: 0; padding: 0;">
        <div style="max-width: 600px; margin: auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0px 0px 10px rgba(0,0,0,0.1);">
          <div style="text-align: center;">
            <!--<img src="https://via.placeholder.com/150x80?text=Matcha+Logo" alt="Matcha Logo" style="margin-bottom: 20px;" />-->
            <h1 style="color: #ef4444;">Forgot your password?</h1>
            <p style="font-size: 16px; color: #4b5563;">No worries! Click below to reset it:</p>
            <a href="#{DOMAIN}/reset-password?code=#{code}"
              style="display: inline-block; margin-top: 20px; background-color: #ef4444; color: white; padding: 14px 24px; border-radius: 6px; text-decoration: none; font-size: 16px;">
              Reset Password
            </a>
          </div>
        </div>
        <p style="text-align: center; font-size: 10px; color: #9ca3af; margin-top: 20px;">
          Sent with ❤️ from Matcha | pulgamecanica@matcha.com
        </p>
      </body>
      </html>
    HTML
  end
end
