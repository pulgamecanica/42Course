//! # CRUD Core
//!
//! Core abstractions for model definition, naming conventions,
//! serialization, pluralization, and CRUD trait definitions.
//!
//! This crate is storage-agnostic.

pub mod naming;
pub mod model;
pub mod storage;
pub mod error;
pub mod relations;


pub use naming::*;
pub use model::*;
pub use storage::*;
pub use error::*;
pub use relations::*;
