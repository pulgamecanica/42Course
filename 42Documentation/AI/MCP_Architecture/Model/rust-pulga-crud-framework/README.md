# Rust CRUD Framework Workspace

This workspace contains a modular CRUD framework in Rust with pluggable storage backends, and an example project demonstrating relational data management.

## Workspace Structure

- `crud-core/` – Core framework with model traits, storage abstraction, and naming conventions.
- `crud-storage/` – Default file-based storage implementation.
- `example-library/` – Example project implementing `Library` and `Book` models with relational behavior.

## Features

- Fully modular CRUD framework
- Pluggable storage backend (files, SQL, Redis, AWS S3, etc.)
- Rails-like relationships (`has_many`, `belongs_to`, `has_many_through`)
- Automatic singular/plural naming conventions
- Command-line interactive tools
- ANSI-enhanced, ASCII-based output
- Backup, restore, and migration ready
- Pagination-ready list queries

## Getting Started

Build and run all projects:

```bash
cargo build --workspace
cargo run -p example-library
cargo test --workspace
cargo doc --workspace --no-deps --open
````

## Extending Storage

* Implement the `Storage<M>` trait from `crud-core`.
* Example backends:

  * SQL: PostgreSQL, MySQL, SQLite
  * NoSQL: Redis, MongoDB
  * Cloud: AWS S3, Google Cloud Storage
* Plug-in by adding new crate in workspace and implementing required CRUD methods.

## Documentation

All crates include `cargo doc` documentation:

```bash
cargo doc --workspace --no-deps --open
```

This provides details on traits, models, naming conventions, and storage.

---
