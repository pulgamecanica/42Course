# 🧪 Matcha API — Love & Ruby 💘

[![Sinatra](https://img.shields.io/badge/Made%20with-Sinatra-ff69b4?logo=sinatra)](http://sinatrarb.com/)
[![PostgreSQL](https://img.shields.io/badge/Database-PostgreSQL-336791?logo=postgresql)](https://www.postgresql.org/)
[![Dockerized](https://img.shields.io/badge/Dockerized-Yes-blue?logo=docker)](https://www.docker.com/)
[![Tested with RSpec](https://img.shields.io/badge/Tested%20with-RSpec-ff4155?logo=ruby)](https://rspec.info/)
[![TDD](https://img.shields.io/badge/TDD-Driven-%23cc0066?logo=testing-library)]()
[![Custom DSL](https://img.shields.io/badge/DSL-api_doc-%23bada55)]()

> The only thing better than a Matcha latte is a **match made in Ruby**.  
> This is a fully-documented, TDD-driven Sinatra API for the Matcha dating platform. It’s modular, dockerized, and optimized for love at first request 💘

---

## 📦 Tech Stack

| Layer         | Tech                    |
|---------------|-------------------------|
| Framework     | Sinatra (modular style) |
| Language      | Ruby 3.2                |
| Database      | PostgreSQL 14           |
| Persistence   | Raw SQL + SQLHelper     |
| Auth          | JWT (handrolled)        |
| Container     | Docker + Compose        |
| Testing       | RSpec + Rack::Test      |
| Docs          | `api_doc` DSL (custom)  |
| Console       | IRB via bin/console     |
| Tasks         | Rake + Makefile         |

---

## ⚙️ Features

- 🔐 **Authentication**: Email/password & social (Google, Facebook, Snapchat)
- 🧪 **TDD-first** with RSpec specs for everything
- 🧼 **Clean architecture**: Helpers, controllers, and models separated
- 🔒 **JWT-based sessions** (no gem dependencies)
- 🧠 **Robust validation** with reusable DSL-based `Validator`
- 🌍 **RESTful routes**: `/auth`, `/me`, `/users/:username`, etc.
- 🚫 **Ban & Confirm logic**: No banned or unconfirmed user can access the API
- 💾 **Smart SQL helper**: `SQLHelper.create`, `update`, `find_by`, etc.
- 🧾 **API docs**: Exportable via `make docs`
- 🐳 **Fully dockerized**
- 💬 Friendly, readable logs

---

## 🗂️ Project Structure

```
.
├── app/
│   ├── controllers/     # Modular Sinatra apps (AuthController, UsersController)
│   ├── helpers/         # Validators, SQLHelper, Request parsing, Auth, Database
│   ├── models/          # Models (User, ...)
│   ├── lib/             # Shared error classes, doc
├── config/
│   └── database.yml     # Database settings
│   └── environment.rb   # Loads everything
├── db/
│   ├── migrate/         # DB migrations
│   └── seeds.rb         # (optional)
├── spec/                # RSpec suite
├── docker-compose.yml
├── Rakefile
├── Makefile
├── .env
├── config.ru
└── README.md
```

---

## 🚀 Getting Started

---

### 📘 Postman Collection

> Visit the [Postman Colleciton](https://pulgamecanica.postman.co/workspace/pulgamecanica~fddcef76-1724-4c9d-8f90-c24cfe79c2b9/collection/13231501-422c8247-c0ee-4bec-834b-ede0215c4e05?action=share&creator=13231501) to run all the API endpoints.

---

### 🔧 Local Dev (Dockerized)

```bash
git clone https://github.com/pulgamecanica/matcha
cd matcha

# Build containers
make build
```

---

## 🧪 Testing

RSpec tests live in `spec/`, with:
- Integration tests for endpoints
- Unit tests for helpers
- Full TDD on auth, validation, sessions, and core models

```bash
make test
```

---

## 🛠️ Developer Shortcuts (Makefile)

```bash
make create   # db:create
make migrate  # db:migrate
make seed     # db:seed
make test     # run all specs (you can provide a file)
make console  # open IRB console
make logs     # see the docker logs
make docs     # export route documentation
make re_db    # recreate database (empty)
make seed     # generate database and seed data
```

---

## 🔐 Authentication

Stateless JWT (no libraries!) via `SessionToken`.

```rb
SessionToken.generate(user_id)  # => "encoded.jwt.token"
SessionToken.decode(token)      # => { "user_id" => 42, ... }
```

---

## 📘 API Documentation

Each route is documented inline with the `api_doc` DSL.  
To export to markdown:

```bash
make docs
```

➡ Output: `docs/exported.md`

Example:

```ruby
api_doc "/auth/register", method: :post do
  description "Register a new user"
  param :email, String, required: true
  param :username, String, required: true
  param :password, String, required: true
  response 201, "User created"
end
```

---

## 🚀 Endpoints (Implemented)

> Visit the [API Colleciton](https://pulgamecanica.github.io/matcha-docs/) to see all the API endpoints.

---

## 🎯 Roadmap

- [x] JWT session system
- [x] Login, Register, Social Auth
- [x] Patch & Delete `/me`
- [x] Ban, confirm & protect endpoints
- [x] Public profiles `/users/:username`
- [x] API Docs via DSL
- [x] Validation system
- [x] SQLHelper abstraction
- [x] Tag system
- [x] Connections, Likes
- [X] Notifications
- [X] Commonly used English words should not be accepted as passwords.
- [X] If user unlikes, make sure to delete connection. (cascades all connection relationships such as messages).
- [X] Limit up to 5 pictures.
- [X] Real-time messaging (WebSocket or polling)
- [X] Limit actions if user does not have a profile picture set: [connection is not possible]
- [X] Notification when "matching" for the other user
- [X] Add city and country to the user
- [X] Email acitons (confirm register / reset password)
- [ ] VideoCalls
- [X] Report account as fake
- [ ] Deploy with NGINX

---

## 💡 Philosophy

> 📜 Everything documented  
> 🧪 Everything tested  
> 🚫 No unhandled JSON  
> 💥 No silent failures  
> 💎 Code should read like Ruby poetry  

---