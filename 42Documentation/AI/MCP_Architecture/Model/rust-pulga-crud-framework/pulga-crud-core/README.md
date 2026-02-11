# CRUD Core

The `crud-core` crate provides the foundation for defining models and abstract CRUD behavior in a storage-agnostic way.

## Modules

- `model` – Defines the `Model` trait.
- `storage` – Defines the `Storage<M>` trait.
- `naming` – Utilities for singular/plural conversion and model discovery.
- `relations` – Traits for defining relationships (`has_many`, `belongs_to`).

## Key Traits

### Model

```rust
pub trait Model: Serialize + for<'de> Deserialize<'de> {
    fn id(&self) -> String;
}
````

All models must implement `Serialize`, `Deserialize`, and provide a unique ID.

### Storage

```rust
pub trait Storage<M> {
    fn create(&self, model: &M) -> Result<(), CrudError>;
    fn read(&self, id: &str) -> Result<M, CrudError>;
    fn update(&self, model: &M) -> Result<(), CrudError>;
    fn delete(&self, id: &str) -> Result<(), CrudError>;
    fn list(&self, page: usize, per_page: usize) -> Result<Vec<M>, CrudError>;
}
```

* Provides a unified interface for any backend.

### Relations

* `HasMany<Child>` – Implemented by parent models.
* `BelongsTo<Parent>` – Implemented by child models.
* Supports Rails-style `has_many_through` and `has_one_as` concepts.

## Naming

* Singular struct names → file: `book.rs`
* Plural directories → collection: `books/`
* Files ending `.ignore.rs` are not considered models.

## Example

```rust
use crud_core::pluralize;

assert_eq!(pluralize("Book"), "books");
```