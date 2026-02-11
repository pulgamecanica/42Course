use serde::{Serialize, Deserialize};

/// Marker trait for models.
/// Every model must:
/// - Be serializable
/// - Be deserializable
/// - Have an ID
pub trait Model: Serialize + for<'de> Deserialize<'de> {
    fn id(&self) -> String;
}
