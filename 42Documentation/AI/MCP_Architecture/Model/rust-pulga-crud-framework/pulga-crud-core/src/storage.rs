use crate::error::CrudError;

pub trait Storage<M> {
    fn create(&self, model: &M) -> Result<(), CrudError>;
    fn read(&self, id: &str) -> Result<M, CrudError>;
    fn update(&self, model: &M) -> Result<(), CrudError>;
    fn delete(&self, id: &str) -> Result<(), CrudError>;
    fn list(&self, page: usize, per_page: usize) -> Result<Vec<M>, CrudError>;
}
