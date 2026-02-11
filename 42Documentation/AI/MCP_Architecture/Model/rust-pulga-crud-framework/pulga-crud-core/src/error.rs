use thiserror::Error;

#[derive(Error, Debug)]
pub enum CrudError {
    #[error("Not found")]
    NotFound,

    #[error("Storage error")]
    StorageError,

    #[error("Serialization error")]
    SerializationError,
}
