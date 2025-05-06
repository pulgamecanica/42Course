# frozen_string_literal: true

require_relative '../app/models/user'
require_relative '../app/models/tag'
require_relative '../app/models/user_tag'
require_relative '../app/models/like'
require_relative '../app/models/blocked_user'
require_relative '../app/models/profile_view'
require_relative '../app/models/picture'
require_relative '../app/models/connection'
require_relative '../app/models/message'
require_relative '../app/models/date'
require_relative '../app/models/notification'

require 'faker'
require 'ruby-progressbar'
require 'time'

VERBOSE = true
LOG = Hash.new { |h, k| h[k] = [] }
TOTALUSERS = 100

CITIES = [
  { name: 'Bangkok', country: 'Thailand', latitude: 13.7563, longitude: 100.5018 },
  { name: 'Paris', country: 'France', latitude: 48.8566, longitude: 2.3522 },
  { name: 'London', country: 'United Kingdom', latitude: 51.5074, longitude: -0.1278 },
  { name: 'Dubai', country: 'United Arab Emirates', latitude: 25.2048, longitude: 55.2708 },
  { name: 'Singapore', country: 'Singapore', latitude: 1.3521, longitude: 103.8198 },
  { name: 'Kuala Lumpur', country: 'Malaysia', latitude: 3.139, longitude: 101.6869 },
  { name: 'New York', country: 'USA', latitude: 40.7128, longitude: -74.006 },
  { name: 'Istanbul', country: 'Turkey', latitude: 41.0082, longitude: 28.9784 },
  { name: 'Tokyo', country: 'Japan', latitude: 35.6895, longitude: 139.6917 },
  { name: 'Seoul', country: 'South Korea', latitude: 37.5665, longitude: 126.978 },
  { name: 'Antalya', country: 'Turkey', latitude: 36.8969, longitude: 30.7133 },
  { name: 'Osaka', country: 'Japan', latitude: 34.6937, longitude: 135.5023 },
  { name: 'Milan', country: 'Italy', latitude: 45.4642, longitude: 9.19 },
  { name: 'Barcelona', country: 'Spain', latitude: 41.3851, longitude: 2.1734 },
  { name: 'Hong Kong', country: 'China', latitude: 22.3193, longitude: 114.1694 },
  { name: 'Mecca', country: 'Saudi Arabia', latitude: 21.3891, longitude: 39.8579 },
  { name: 'Pattaya', country: 'Thailand', latitude: 12.9236, longitude: 100.8825 },
  { name: 'Vienna', country: 'Austria', latitude: 48.2082, longitude: 16.3738 },
  { name: 'Rome', country: 'Italy', latitude: 41.9028, longitude: 12.4964 },
  { name: 'Shanghai', country: 'China', latitude: 31.2304, longitude: 121.4737 },
  { name: 'Los Angeles', country: 'USA', latitude: 34.0522, longitude: -118.2437 },
  { name: 'Las Vegas', country: 'USA', latitude: 36.1699, longitude: -115.1398 },
  { name: 'Prague', country: 'Czech Republic', latitude: 50.0755, longitude: 14.4378 },
  { name: 'Miami', country: 'USA', latitude: 25.7617, longitude: -80.1918 },
  { name: 'Madrid', country: 'Spain', latitude: 40.4168, longitude: -3.7038 },
  { name: 'Amsterdam', country: 'Netherlands', latitude: 52.3676, longitude: 4.9041 },
  { name: 'Macau', country: 'China', latitude: 22.1987, longitude: 113.5439 },
  { name: 'Ho Chi Minh City', country: 'Vietnam', latitude: 10.7769, longitude: 106.7009 },
  { name: 'Cancún', country: 'Mexico', latitude: 21.1619, longitude: -86.8515 },
  { name: 'Berlin', country: 'Germany', latitude: 52.52, longitude: 13.405 },
  { name: 'San Francisco', country: 'USA', latitude: 37.7749, longitude: -122.4194 },
  { name: 'Chicago', country: 'USA', latitude: 41.8781, longitude: -87.6298 },
  { name: 'Moscow', country: 'Russia', latitude: 55.7558, longitude: 37.6173 },
  { name: 'Lisbon', country: 'Portugal', latitude: 38.7169, longitude: -9.1399 },
  { name: 'Toronto', country: 'Canada', latitude: 43.6532, longitude: -79.3832 },
  { name: 'Beijing', country: 'China', latitude: 39.9042, longitude: 116.4074 },
  { name: 'Athens', country: 'Greece', latitude: 37.9838, longitude: 23.7275 },
  { name: 'Hanoi', country: 'Vietnam', latitude: 21.0278, longitude: 105.8342 },
  { name: 'Florence', country: 'Italy', latitude: 43.7696, longitude: 11.2558 },
  { name: 'Sydney', country: 'Australia', latitude: -33.8688, longitude: 151.2093 },
  { name: 'Munich', country: 'Germany', latitude: 48.1351, longitude: 11.582 },
  { name: 'Brussels', country: 'Belgium', latitude: 50.8503, longitude: 4.3517 },
  { name: 'Dublin', country: 'Ireland', latitude: 53.3498, longitude: -6.2603 },
  { name: 'Riyadh', country: 'Saudi Arabia', latitude: 24.7136, longitude: 46.6753 },
  { name: 'Ha Long', country: 'Vietnam', latitude: 20.951, longitude: 107.0448 },
  { name: 'Buenos Aires', country: 'Argentina', latitude: -34.6037, longitude: -58.3816 },
  { name: 'Sao Paulo', country: 'Brazil', latitude: -23.5505, longitude: -46.6333 },
  { name: 'Cape Town', country: 'South Africa', latitude: -33.9249, longitude: 18.4241 },
  { name: 'Johannesburg', country: 'South Africa', latitude: -26.2041, longitude: 28.0473 },
  { name: 'Jakarta', country: 'Indonesia', latitude: -6.2088, longitude: 106.8456 }
].freeze

puts '🌱 Seeding database...'

summary = {
  tags: [],
  users: [],
  links: [],
  pictures: [],
  likes: [],
  views: [],
  blocks: []
}

# ---------------------------
# Tags
# ---------------------------
puts '📌 Creating Tags...'
tag_names = %w[music hiking gaming vegan yoga books travel art dogs cats cooking dancing]
tag_bar = ProgressBar.create(title: 'Tags', total: tag_names.size)
tags = tag_names.map do |name|
  tag_bar.increment
  tag = Tag.find_by_name(name) || Tag.create(name)
  summary[:tags] << tag['name']
  LOG[:tags] << "➕ Tag: #{name}"
  tag
end

# ---------------------------
# Fake Users
# ---------------------------
puts '👥 Creating fake users...'
users = []
usernames = TOTALUSERS.times.map { Faker::Internet.unique.username(specifier: 5..10) }
user_bar = ProgressBar.create(title: 'Users', total: usernames.size)

usernames.each do |username|
  user_bar.increment

  user = User.find_by_username(username) || User.create({
                                                          username: username,
                                                          email: Faker::Internet.email(name: username),
                                                          password: username,
                                                          first_name: Faker::Name.first_name,
                                                          last_name: Faker::Name.last_name,
                                                          gender: %w[male female other].sample,
                                                          sexual_preferences: %w[male female non_binary everyone].sample,
                                                          birth_year: Faker::Number.between(from: 1970, to: 2006)
                                                        })
  User.confirm!(user['username'])

  city = CITIES.sample
  # Add small noise within ±0.1° for realism (~11km latitude, ~8km longitude)
  latitude = city[:latitude] + rand(-0.1..0.1)
  longitude = city[:longitude] + rand(-0.1..0.1)

  User.update(user['id'], {
                latitude: latitude.round(6),
                longitude: longitude.round(6),
                country: city[:country],
                city: city[:name]
              })

  users << user
  summary[:users] << username
  LOG[:users] << "👤 Created user: #{username}"

  sample_tags = tags.sample(rand(1..3))
  sample_tags.each do |tag|
    UserTag.add_tag(user['id'], tag['id'])
    summary[:links] << [username, [tag['name']]]
    LOG[:links] << "   🔗 #{username} tagged with #{tag['name']}"
  end

  pic_url = Faker::Avatar.image(slug: username)
  # pic_url = "https://i.pravatar.cc/300?u=#{username}"
  pic = Picture.create(user['id'], pic_url, is_profile: true)
  User.update(user['id'], { "profile_picture_id": pic['id'] })
  summary[:pictures] << username
  LOG[:pictures] << "   🖼️ #{username} profile picture added"
end

# ---------------------------
# Blocked Users
# ---------------------------
puts '🚫 Blocking users...'
combos = users.combination(2).to_a
blocked_map = Hash.new { |h, k| h[k] = Set.new }
block_bar = ProgressBar.create(title: 'Blocks', total: combos.size)

combos.each do |u1, u2|
  block_bar.increment
  next unless rand < 0.002

  BlockedUser.block!(u1['id'], u2['id'])
  blocked_map[u1['id']] << u2['id']
  summary[:blocks] << "#{u1['username']} ⛔ #{u2['username']}"
  LOG[:blocks] << "🚫 #{u1['username']} blocked #{u2['username']}"
end

# ---------------------------
# Discoverable Users Cache
# ---------------------------
puts '🔍 Caching discoverable users...'
discover_map = {}
discover_bar = ProgressBar.create(title: 'Discoverables', total: users.size)

users.each do |user|
  discover_bar.increment
  discover_map[user['id']] = User.discover(user).map { |u| u[:user]['id'] }
end

# ---------------------------
# Likes and Views
# ---------------------------
puts '💘 Creating interactions (likes, views)...'
likes = {}
interaction_bar = ProgressBar.create(title: 'Interactions', total: combos.size)

combos.each do |u1, u2|
  interaction_bar.increment

  next if blocked_map[u1['id']].include?(u2['id']) || blocked_map[u2['id']].include?(u1['id'])
  next unless discover_map[u1['id']].include?(u2['id']) && discover_map[u2['id']].include?(u1['id'])

  if rand < 0.4
    Like.like!(u1['id'], u2['id'])
    (likes[u1['id']] ||= Set.new) << u2['id']
    summary[:likes] << "#{u1['username']} → #{u2['username']}"
    LOG[:likes] << "❤️ #{u1['username']} liked #{u2['username']}"
    Notification.create(u2['id'], "#{u1['username']} liked your profile", u1['id'], 'like')
    LOG[:notifications] << "🔔 #{u1['username']} → #{u2['username']} (like)"
  end

  if rand < 0.3
    Like.like!(u2['id'], u1['id'])
    (likes[u2['id']] ||= Set.new) << u1['id']
    summary[:likes] << "#{u2['username']} → #{u1['username']}"
    LOG[:likes] << "❤️ #{u2['username']} liked #{u1['username']}"
    Notification.create(u1['id'], "#{u2['username']} liked your profile", u2['id'], 'like')
    LOG[:notifications] << "🔔 #{u2['username']} → #{u1['username']} (like)"
  end

  if rand < 0.5
    ProfileView.record(u1['id'], u2['id'])
    summary[:views] << "#{u1['username']} → #{u2['username']}"
    LOG[:views] << "👀 #{u1['username']} viewed #{u2['username']}"
    Notification.create(u2['id'], "#{u1['username']} viewed your profile", u1['id'], 'like')
    LOG[:notifications] << "🔔 #{u1['username']} → #{u2['username']} (view)"
  end

  next unless rand < 0.5

  ProfileView.record(u2['id'], u1['id'])
  summary[:views] << "#{u2['username']} → #{u1['username']}"
  LOG[:views] << "👀 #{u2['username']} viewed #{u1['username']}"
  Notification.create(u1['id'], "#{u2['username']} viewed your profile", u2['id'], 'like')
  LOG[:notifications] << "🔔 #{u1['username']} → #{u2['username']} (view)"
end

# ---------------------------
# Connections
# ---------------------------
puts '🔗 Creating connections for matches...'
connection_bar = ProgressBar.create(title: 'Connections', total: users.size)

users.each do |u1|
  connection_bar.increment
  (likes[u1['id']] || []).each do |liked_id|
    next unless likes[liked_id]&.include?(u1['id'])

    conn = Connection.create(u1['id'], liked_id)
    if conn
      u2 = users.find { |u| u['id'] == liked_id }
      LOG[:connections] << "🔗 #{u1['username']} ↔ #{u2['username']}"
    end
  end
end

# ---------------------------
# Messages
# ---------------------------
puts '✉️ Generating messages...'
message_bar = ProgressBar.create(title: 'Messages', total: users.size)

users.each do |user|
  message_bar.increment
  User.connections(user['id']).each do |partner|
    conn = Connection.find_between(user['id'], partner['id'])
    next unless conn

    rand(1..3).times do
      content = Faker::Lorem.sentence(word_count: rand(4..10))
      Message.create(conn['id'], user['id'], content)
      LOG[:messages] << "✉️ #{user['username']} → #{partner['username']}: #{content}"
      Notification.create(partner['id'], "#{user['username']} sent you a message", user['id'], 'message')
      LOG[:notifications] << "🔔 #{user['username']} → #{partner['username']} (message)"
    end
  end
end

# ---------------------------
# Dates
# ---------------------------
puts '📅 Scheduling dates...'
date_count = users.sum { |user| User.connections(user['id']).size }
date_bar = ProgressBar.create(title: 'Dates', total: date_count)

users.each do |user|
  User.connections(user['id']).each do |partner|
    date_bar.increment
    conn = Connection.find_between(user['id'], partner['id'])
    next unless conn
    next unless rand < 0.02

    time = Faker::Time.forward(days: rand(1..30), period: :evening).iso8601
    location = Faker::Address.city
    description = Faker::Lorem.sentence(word_count: 5)

    Date.create(conn['id'], user['id'], location, time, description)
    parsed_time = Time.parse(time.to_s)
    LOG[:dates] << "📅 #{user['username']} scheduled a date with #{partner['username']} at #{location} on #{parsed_time.strftime('%F %H:%M')}" # rubocop:disable Layout/LineLength
  end
end

# ---------------------------
# Final Summary
# ---------------------------
puts "\n✅ Done seeding!\n\n"

puts "👤 Users created: (#{summary[:users].size})"
puts "🏷️ Tags created: (#{summary[:tags].size})"
puts "🖼️ Pictures added: (#{summary[:pictures].size})"
puts "🔗 User-Tag links: (#{summary[:links].size})"
puts "❤️ Likes: (#{summary[:likes].size})"
puts "👀 Views: (#{summary[:views].size})"
puts "🚫 Blocks: (#{summary[:blocks].size})"
puts "🔗 Connections: (#{LOG[:connections].size})"
puts "✉️ Messages: (#{LOG[:messages].size})"
puts "📅 Dates: (#{LOG[:dates].size})"
puts "🔔 Notifications: (#{LOG[:notifications].size})"

if VERBOSE
  puts "\n📘 Detailed Log:\n"
  LOG.each do |section, lines|
    puts "\n🔹 #{section.capitalize} (#{lines.size})"
    lines.each { |line| puts "  #{line}" }
  end
end
