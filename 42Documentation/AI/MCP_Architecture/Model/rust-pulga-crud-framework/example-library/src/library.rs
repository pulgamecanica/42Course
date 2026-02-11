use serde::{Serialize, Deserialize};
use pulga_crud_core::{Model, HasMany, Storage, CrudError};
use uuid::Uuid;

use crate::book::Book;

#[derive(Serialize, Deserialize, Clone)]
pub struct Library {
    pub id: String,
    pub name: String,
}

impl Library {
    pub fn new(name: String) -> Self {
        Self {
            id: Uuid::new_v4().to_string(),
            name,
        }
    }
}

impl Model for Library {
    fn id(&self) -> String {
        self.id.clone()
    }
}

impl HasMany<Book> for Library {
    fn has_many<S: Storage<Book>>(
        &self,
        storage: &S,
    ) -> Result<Vec<Book>, CrudError> {
        let all = storage.list(0, 10_000)?;
        Ok(all
            .into_iter()
            .filter(|b| b.library_id == self.id)
            .collect())
    }
}
