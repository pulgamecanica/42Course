//! Default raw file-based storage.
//!
//! Layout:
//!
//! data/
//!   books/
//!     1.json
//!     2.json
//!
//! Supports pagination via directory reading.

use std::fs;
use std::io::Read;
use std::marker::PhantomData;
use std::path::PathBuf;

use pulga_crud_core::{Storage, Model, CrudError, pluralize};
use serde::de::DeserializeOwned;

pub struct FileStorage<M> {
    base: PathBuf,
    _marker: PhantomData<M>,
}

impl<M> FileStorage<M>
where
    M: Model + DeserializeOwned,
{
    pub fn new(base: PathBuf) -> Self {
        fs::create_dir_all(&base).ok();
        Self {
            base,
            _marker: PhantomData,
        }
    }

    fn collection_path(&self) -> PathBuf {
        let type_name = std::any::type_name::<M>()
            .split("::")
            .last()
            .unwrap();

        let mut p = self.base.clone();
        p.push(pluralize(type_name));
        fs::create_dir_all(&p).ok();
        p
    }

    fn file_path(&self, id: &str) -> PathBuf {
        let mut p = self.collection_path();
        p.push(format!("{}.json", id));
        p
    }
}

impl<M> Storage<M> for FileStorage<M>
where
    M: Model + DeserializeOwned,
{
    fn create(&self, model: &M) -> Result<(), CrudError> {
        let json = serde_json::to_string_pretty(model)
            .map_err(|_| CrudError::SerializationError)?;
        fs::write(self.file_path(&model.id()), json)
            .map_err(|_| CrudError::StorageError)
    }

    fn read(&self, id: &str) -> Result<M, CrudError> {
        let mut file = fs::File::open(self.file_path(id))
            .map_err(|_| CrudError::NotFound)?;

        let mut contents = String::new();
        file.read_to_string(&mut contents)
            .map_err(|_| CrudError::StorageError)?;

        serde_json::from_str(&contents)
            .map_err(|_| CrudError::SerializationError)
    }

    fn update(&self, model: &M) -> Result<(), CrudError> {
        self.create(model)
    }

    fn delete(&self, id: &str) -> Result<(), CrudError> {
        fs::remove_file(self.file_path(id))
            .map_err(|_| CrudError::NotFound)
    }

    fn list(&self, page: usize, per_page: usize) -> Result<Vec<M>, CrudError> {
        let entries = fs::read_dir(self.collection_path())
            .map_err(|_| CrudError::StorageError)?;

        let mut items = vec![];

        for entry in entries.flatten() {
            let mut file = fs::File::open(entry.path())
                .map_err(|_| CrudError::StorageError)?;

            let mut contents = String::new();
            file.read_to_string(&mut contents)
                .map_err(|_| CrudError::StorageError)?;

            let obj: M = serde_json::from_str(&contents)
                .map_err(|_| CrudError::SerializationError)?;

            items.push(obj);
        }

        let start = page * per_page;

        Ok(items.into_iter().skip(start).take(per_page).collect())
    }
}
