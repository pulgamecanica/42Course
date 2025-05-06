# frozen_string_literal: true

require_relative '../lib/errors'

module Validator
  def self.validate!(params:, required: [], enums: {}, length: {}, range: {})
    errors = []

    errors.concat validate_required(params, required)
    errors.concat validate_enums(params, enums)
    errors.concat validate_length(params, length)
    errors.concat validate_range(params, range)

    raise Errors::ValidationError.new('Validation failed', errors) unless errors.empty?
  end

  def self.validate_required(params, required)
    required.each_with_object([]) do |field, errs|
      value = params[field.to_s] || params[field.to_sym]
      errs << "#{field} is required" if value.nil? || value.to_s.strip.empty?
    end
  end

  def self.validate_enums(params, enums)
    enums.each_with_object([]) do |(field, valid_values), errs|
      value = params[field.to_s] || params[field.to_sym]
      errs << "#{field} must be one of: #{valid_values.join(', ')}" if value && !valid_values.include?(value)
    end
  end

  def self.validate_length(params, length_rules)
    length_rules.each_with_object([]) do |(field, rules), errs|
      value = params[field.to_s] || params[field.to_sym]
      next unless value.respond_to?(:length)

      errs << "#{field} must be at least #{rules[:min]} characters" if rules[:min] && value.length < rules[:min]
      errs << "#{field} must be at most #{rules[:max]} characters" if rules[:max] && value.length > rules[:max]
    end
  end

  def self.validate_range(params, range_rules)
    range_rules.each_with_object([]) do |(field, rules), errs|
      value = params[field.to_s] || params[field.to_sym]
      next unless value.is_a?(Numeric)

      errs << "#{field} must be at least #{rules[:min]}" if rules[:min] && value < rules[:min]
      errs << "#{field} must be at most #{rules[:max]}" if rules[:max] && value > rules[:max]
    end
  end

  private_class_method :validate_required, :validate_enums, :validate_length, :validate_range
end
