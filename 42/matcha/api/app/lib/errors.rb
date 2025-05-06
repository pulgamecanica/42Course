# frozen_string_literal: true

module Errors
  class ValidationError < StandardError
    def initialize(message = 'Validation failed', details = nil)
      super(message)
      @details = details
    end

    attr_reader :details
  end

  class JSONParsingError < StandardError
    def initialize(message = 'Parsing error', details = nil)
      super(message)
      @details = details
    end

    attr_reader :details
  end
end
