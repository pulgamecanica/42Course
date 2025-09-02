# frozen_string_literal: true

module APIDoc
  def self.included(base)
    base.extend(ClassMethods)
  end

  module ClassMethods
    def api_doc(path, method:, &block)
      @docs ||= {}
      dsl = DSL.new
      dsl.instance_eval(&block)
      @docs[[method.to_s.upcase, path]] = dsl.data
    end

    def docs
      @docs || {}
    end
  end

  class DSL
    attr_reader :data

    def initialize
      @data = {
        params: [],
        responses: [],
        tags: [],
        auth_required: true
      }
    end

    def description(text)
      @data[:description] = text
    end

    def tag(name)
      tags(name)
    end

    def tags(*names)
      @data[:tags].concat(names.map(&:to_s)).uniq!
    end

    def auth_required(value: true)
      @data[:auth_required] = value
    end

    def param(name, type, required: false, desc: nil)
      @data[:params] << { name:, type:, required:, desc: }
    end

    def response(code, desc, example: nil)
      @data[:responses] << { code:, desc:, example: }
    end
  end

  def self.export_markdown(path:, title: nil, include_auth_notice: false)
    controllers = ObjectSpace.each_object(Class).select { |cls| cls < Sinatra::Base }

    File.open(path, 'w') do |file|
      write_header(file, title, include_auth_notice)

      controllers.each do |ctrl|
        next unless ctrl.respond_to?(:docs)

        ctrl.docs.each do |(method, route), doc|
          write_endpoint_doc(file, method, route, doc)
        end
      end
    end
  end

  def self.write_header(file, title, include_auth_notice)
    file.puts "# #{title}\n\n" if title
    return unless include_auth_notice

    file.puts "> **Note:** All authenticated endpoints require a valid token via `Authorization: Bearer <token>`.\n\n"
  end

  def self.write_endpoint_doc(file, method, route, doc)
    file.puts "## `#{method}` #{route}"
    file.puts "**Description**: #{doc[:description]}"
    file.puts "**Auth required**: #{doc[:auth_required] ? 'Yes' : 'No'}"
    file.puts "**Tags**: #{doc[:tags].join(', ')}" if doc[:tags]&.any?

    write_params(file, doc[:params]) if doc[:params].any?
    write_responses(file, doc[:responses]) if doc[:responses].any?

    file.puts "\n---\n"
  end

  def self.write_params(file, params)
    file.puts "\n### Parameters"
    params.each do |p|
      type = p[:type].to_s.gsub(/^.*::/, '')
      required = p[:required] ? '**(required)**' : ''
      file.puts "- `#{p[:name]}` (#{type}) #{required} - #{p[:desc]}"
    end
  end

  def self.write_responses(file, responses)
    file.puts "\n### Responses"
    responses.each do |r|
      file.puts "- `#{r[:code]}`: #{r[:desc]}"
      file.puts "```json\n#{JSON.pretty_generate(r[:example])}\n```" if r[:example]
    end
  end

  def self.export_json(path:)
    controllers = ObjectSpace.each_object(Class).select { |cls| cls < Sinatra::Base }
    all_routes = []
    controllers.each do |ctrl|
      next unless ctrl.respond_to?(:docs)

      ctrl.docs.each do |(method, route), doc|
        all_routes << {
          method: method,
          path: route,
          description: doc[:description],
          auth_required: doc[:auth_required],
          tags: doc[:tags] || [],
          params: doc[:params],
          responses: doc[:responses]
        }
      end
    end
    File.write(path, JSON.pretty_generate(all_routes))
  end

  private_class_method :write_header, :write_endpoint_doc, :write_params, :write_responses
end
