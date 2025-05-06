# frozen_string_literal: true

require 'rake'
require 'pg'
require 'uri'
require 'fileutils'

desc 'Export all API docs'
task 'doc:export' do
  require_relative './app/lib/api_doc'
  require_relative './app'

  puts
  puts '📘 Exporting API Documentation'
  puts '────────────────────────────────────────────────'

  markdown_path = 'docs/exported.md'
  webpage_path = 'docs/routes.json'
  APIDoc.export_markdown(
    path: markdown_path,
    title: '📘 API Documentation',
    include_auth_notice: true
  )
  APIDoc.export_json(path: webpage_path)

  puts "✅ Done! Markdown exported to: #{markdown_path}"
  puts "✅ Done! Data loaded to: #{webpage_path}"
  puts '📄 You can open it in any Markdown viewer or GitHub'
  puts
end

namespace :db do
  desc 'Create the database'
  task :create do
    uri = URI.parse(ENV['DATABASE_URL'])
    dbname = uri.path[1..] || 'matcha_db'
    conn = PG.connect(dbname: 'postgres', user: uri.user, password: uri.password, host: uri.host)
    begin
      conn.exec("CREATE DATABASE #{dbname}")
      puts "✅ Database '#{dbname}' created"
    rescue PG::DuplicateDatabase
      puts "⚠️  Database already exists: #{dbname}"
    ensure
      conn.close
    end
  end

  desc 'Run all migrations'
  task :migrate do
    Dir.glob('db/migrate/*.rb').sort.each do |file|
      puts "🔧 Running #{file}"
      require_relative "./#{file}"
    end
  end

  desc 'Drop the database'
  task :drop do
    uri = URI.parse(ENV['DATABASE_URL'])
    dbname = uri.path[1..]

    # Connect to a different DB like `postgres`
    conn = PG.connect(
      dbname: 'postgres',
      host: uri.host,
      user: uri.user,
      password: uri.password
    )

    begin
      # Terminate all connections to the DB we're dropping
      conn.exec <<~SQL
        REVOKE CONNECT ON DATABASE #{dbname} FROM public;
        SELECT pg_terminate_backend(pid) FROM pg_stat_activity WHERE datname = '#{dbname}';
      SQL

      # Drop it
      conn.exec("DROP DATABASE IF EXISTS #{dbname}")
      puts "🗑️  Dropped database: #{dbname}"
    ensure
      conn.close
    end
  end

  desc 'Seed the database'
  task :seed do
    load './db/seeds.rb'
  end
end

desc 'Run the test suite'
task :test do
  sh 'bundle exec rspec'
end
