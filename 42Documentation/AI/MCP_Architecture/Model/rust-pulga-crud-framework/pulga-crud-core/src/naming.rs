//! Naming utilities.
//!
//! Responsible for:
//! - Singularization
//! - Pluralization
//! - File naming rules
//! - Model discovery validation
//!
//! ## Naming Rules
//!
//! - Model struct: `Book`
//! - Singular file: `book.rs`
//! - Plural directory: `books/`
//! - Storage collection name: plural snake_case
//!
//! Files ending with `.ignore.rs` are NOT treated as models.
//!
//! ## Examples
//!
//! ```rust
//! use pulga_crud_core::pluralize;
//!
//! assert_eq!(pluralize("Book"), "books");
//! assert_eq!(pluralize("LibraryItem"), "library_items");
//! ```
//!
//! ## Validation
//!
//! A file is considered a model if:
//! - It ends in `.rs`
//! - It does NOT end in `.ignore.rs`

use inflector::Inflector;

pub fn singularize(name: &str) -> String {
    name.to_singular().to_snake_case()
}

pub fn pluralize(name: &str) -> String {
    name.to_plural().to_snake_case()
}

pub fn is_model_file(filename: &str) -> bool {
    filename.ends_with(".rs") && !filename.ends_with(".ignore.rs")
}
