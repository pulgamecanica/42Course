# CRUD Storage

The `crud-storage` crate provides a **default file-based storage backend** using JSON files in directories.

## Layout

```
data/
books/
1.json
2.json
libraries/
a1b2c3.json
```

- Each model is serialized to a separate JSON file.
- Supports `create`, `read`, `update`, `delete`, and paginated `list`.
- Backup: Copy `data/` folder.
- Restore: Replace `data/` folder.

## Extending Storage

You can implement other storage backends by implementing `Storage<M>`:

### Examples

- **SQL Databases**
  - PostgreSQL: Use `sqlx` or `diesel`
  - MySQL: Use `sqlx` or `mysql_async`
  - SQLite: `rusqlite` or `sqlx`
- **NoSQL**
  - Redis: Store JSON or hashes
  - MongoDB: Store serialized BSON
- **Cloud**
  - AWS S3: Store each record as an object
  - Google Cloud Storage: Object-based JSON storage

### Implementation Steps

1. Create new crate or module for backend.
2. Implement `Storage<M>` for your model.
3. Implement `create`, `read`, `update`, `delete`, `list`.
4. Optionally add batch operations, migrations, or caching.
5. Plug into the CLI or example project without modifying core.
