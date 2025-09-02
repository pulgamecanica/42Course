# frozen_string_literal: true

require 'pg'
require 'uri'
require 'connection_pool'

module Database
  def self.pool
    @pool ||= ConnectionPool.new(size: 5, timeout: 5) do
      uri = URI.parse(ENV['DATABASE_URL'])
      PG.connect(
        host: uri.host,
        user: uri.user,
        password: uri.password,
        dbname: uri.path[1..]
      )
    end
  end

  def self.with_conn(max_retries: 3)
    attempts = 0

    begin
      pool.with do |conn|
        raise PG::ConnectionBad, 'Stale PG connection' if conn.finished?

        yield conn
      end
    rescue PG::ConnectionBad => e
      attempts += 1
      puts "[DB] Reconnecting due to: #{e.message} (attempt #{attempts}/#{max_retries})"
      @pool = nil
      sleep 0.05 * attempts
      retry if attempts < max_retries
      raise
    end
  end

  def self.with_open_conn(&block)
    pool ||= ConnectionPool.new(size: 5, timeout: 5) do
      uri = URI.parse(ENV['DATABASE_URL'])
      PG.connect(
        host: uri.host,
        user: uri.user,
        password: uri.password,
        dbname: uri.path[1..]
      )
    end
    pool.with(&block)
  end
end
