# Example Library Project

This project demonstrates how to use the CRUD framework to manage **Books** and **Libraries** with relational behavior.

## Models

### Library

- `id: String`
- `name: String`
- Implements `HasMany<Book>` → fetch all books in the library

### Book

- `id: String`
- `title: String`
- `pages: u32`
- `library_id: String`
- Implements `BelongsTo<Library>` → fetch parent library

## Features

- Interactive CLI with ANSI styling
- Create, list, and delete libraries and books
- Select library when creating books using arrow menu
- ASCII/emoji icons for even/odd pages
- Shows all books per library
- Shows the library for each book
- Storage via file-based backend (`crud-storage`)

## Usage

```bash
cargo run -p example-library
````

### Example Flow

```
Select: Create Library → Enter name
Select: Create Book → Enter title/pages → Choose library
Select: List Libraries → Displays libraries and their books
Select: List Books → Displays books and their library
Select: Delete Book → Choose book via arrow keys
```

## Extending

* Can swap `FileStorage` with any other backend implementing `Storage<M>`
* Supports Rails-style relations and future filtering/pagination
* Ideal starting point for larger relational projects in Rust
