use serde::{Serialize, Deserialize};
use pulga_crud_core::{Model, BelongsTo};
use uuid::Uuid;

use crate::library::Library;

#[derive(Serialize, Deserialize, Clone)]
pub struct Book {
    pub id: String,
    pub title: String,
    pub pages: u32,
    pub library_id: String,
}

impl Book {
    pub fn new(title: String, pages: u32, library_id: String) -> Self {
        Self {
            id: Uuid::new_v4().to_string(),
            title,
            pages,
            library_id,
        }
    }
}

impl Model for Book {
    fn id(&self) -> String {
        self.id.clone()
    }
}

impl BelongsTo<Library> for Book {
    fn belongs_to<S: pulga_crud_core::Storage<Library>>(
        &self,
        storage: &S,
    ) -> Result<Library, pulga_crud_core::CrudError> {
        storage.read(&self.library_id)
    }
}
