use crate::{Storage, CrudError};

pub trait HasMany<Child> {
    fn has_many<S: Storage<Child>>(
        &self,
        storage: &S,
    ) -> Result<Vec<Child>, CrudError>;
}

pub trait BelongsTo<Parent> {
    fn belongs_to<S: Storage<Parent>>(
        &self,
        storage: &S,
    ) -> Result<Parent, CrudError>;
}
