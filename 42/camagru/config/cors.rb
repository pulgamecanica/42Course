# frozen_string_literal: true

class CORS
  def initialize(app)
    @app = app
  end

  def call(env)
    status, headers, body = if env['REQUEST_METHOD'] == 'OPTIONS'
                              [200, cors_headers, []]
                            else
                              @app.call(env)
                            end

    [status, headers.merge(cors_headers), body]
  end

  private

  def cors_headers
    {
      'access-control-allow-origin' => '*',
      'access-control-allow-methods' => 'GET, POST, PUT, PATCH, DELETE, OPTIONS',
      'access-control-allow-headers' => 'content-type, authorization',
      'access-control-max-age' => '86400'
    }
  end
end
