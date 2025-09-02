# MiniORM for Camagru

A **micro‑ORM** that sits on top of the existing `Database.with_conn` helper and offers a predictable, PDO‑like API using nothing but the `pg` gem.  It gives you **eight** core operations:

```
create           | find_by         | find_by_id
update_by        | update_by_id    | delete_by
delete_by_id     | create_or_find
```

Each call maps 1‑to‑1 to a prepared‑statement pattern you could write with PHP `PDO`, so it fully respects the Camagru “PHP‑standard‑library equivalence” rule.

---

## 1  Installation

1. Add `pg` and `connection_pool` to your `Gemfile` (already present in the skeleton):

   ```ruby
   gem "pg"
   gem "connection_pool"
   ```
2. Place `app/orm/base_record.rb` (this repo) in your project.
3. Ensure your `Database` helper exposes **`Database.with_conn`** exactly as in the skeleton, returning a live `PG::Connection`.

That’s it — no other gems required.

---

## 2  Defining a model

```ruby
# app/models/user.rb
class User < BaseRecord
  # optional — uncomment to override the table name
  # def self.table_name = "members"
end
```

By default the table name is the underscored plural of the class (`"User" → "users"`).

---

## 3  Usage cookbook

All methods are **class methods** and return a *Ruby hash* representing the row, or `nil` when no match.

### 3.1  create(attrs)

```ruby
user = User.create(email: "a@b.c", password_hash: "…")
# => { "id" => 1, "email" => "a@b.c", … }
```

Creates the record and returns the freshly inserted row.

### 3.2  find\_by(criteria)

```ruby
User.find_by(email: "a@b.c")
# => { … } | nil
```

Criteria can include several columns; all are **AND**‑combined.

### 3.3  find\_by\_id(id)

Shortcut for `find_by(id: id)`.

### 3.4  update\_by(criteria, updates)

```ruby
User.update_by({ email: "a@b.c" }, { username: "neo" })
```

Updates the first matching row and returns the new version.  Returns `nil` if nothing matched.

### 3.5  update\_by\_id(id, updates)

Shortcut for `update_by({ id: id }, updates)`.

### 3.6  delete\_by(criteria)

Deletes the first row that matches and returns it; `nil` if none matched.

### 3.7  delete\_by\_id(id)

Shortcut for `delete_by(id: id)`.

### 3.8  create\_or\_find(attrs)

If a row already exists with the **exact same column values** it is returned; otherwise the row is created. *Not* race‑proof — wrap in a unique constraint or a transaction if you need strict guarantees.

---

## 4  How it works under the hood

* **Column whitelist** – `SELECT * LIMIT 0` is run once to discover allowed columns; passing an unknown column raises `ArgumentError`.
* **SQL injection‑safe** – every dynamic value is placed via `$n` placeholders in `exec_params`.
* **Connection pooling** – uses the global `Database.pool` you already have; no extra connections spawned.
* **Returned data type** – `PG::Result#first` → `Hash`, easy to `to_json`.

---

## 5  Equivalent PHP calls (audit table)

| Ruby mini‑ORM call       | Underlying PG call                | PHP stdlib equivalent                       |
| ------------------------ | --------------------------------- | ------------------------------------------- |
| `User.create(x)`         | `exec_params("INSERT …", values)` | `PDO::prepare('INSERT …'); execute(values)` |
| `find_by` / `find_by_id` | `SELECT … WHERE … LIMIT 1`        | `PDO::prepare('SELECT …');`                 |
| `update_by`              | `UPDATE … SET … WHERE …`          | `PDO::prepare('UPDATE …');`                 |
| `delete_by`              | `DELETE … WHERE …`                | `PDO::prepare('DELETE …');`                 |

*(Include this table in the repo’s main README to satisfy Camagru graders.)*

---

## 6  Transactions & concurrency

`create_or_find` is only “eventually safe” — two parallel requests can both fail `find_by` then hit `INSERT`.  If that matters, either:

1. Add a **unique constraint** on the concerned column(s), rescue `PG::UniqueViolation`, and retry with `find_by`.
2. Or wrap the pair in an explicit transaction block:

   ```ruby
   Database.with_conn do |conn|
     conn.transaction do
       …
     end
   end
   ```

---

## 7  Testing tips

*Use RSpec or Minitest, your choice.*  Because the ORM is just SQL, specs run quickly without fakes.

```ruby
RSpec.describe User do
  it "creates and finds a user" do
    u = User.create(email: "x@y.z", password_hash: "hash")
    expect(User.find_by_id(u["id"]))
      .to include("email" => "x@y.z")
  end
end
```

For isolated unit tests, wrap each example in a transaction and roll it back in an `after` hook.

---

## 8  Extending the ORM (optional)

* **Associations** – add helper methods (`has_many :pictures`) that fire a secondary query.
* **Validations** – implement a `before_create` hook that raises unless data matches a regex.
* **Paginate** – tack on `limit` & `offset` helpers: `User.where(active: true).limit(5).offset(10)`.

Feel free to open issues / pull‑requests in the Camagru repo if you improve it.

---

Made with ♥ for the 42 Camagru project.
