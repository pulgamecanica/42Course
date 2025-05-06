# frozen_string_literal: true

require 'logger'

LOG_LEVELS = {
  'DEBUG' => Logger::DEBUG,
  'INFO' => Logger::INFO,
  'WARN' => Logger::WARN,
  'ERROR' => Logger::ERROR,
  'FATAL' => Logger::FATAL
}.freeze

level_str = ENV.fetch('RUBY_LOG_LEVEL', 'INFO').upcase
level     = LOG_LEVELS[level_str] || Logger::INFO
LOGGER = Logger.new($stdout)
LOGGER.level = level

LOGGER.formatter = proc do |severity, datetime, _progname, msg|
  "[#{datetime}] #{severity}: #{msg}\n"
end
