use std::fmt::Debug;
use std::ops::{Add, Sub, Mul, Div, AddAssign, SubAssign, MulAssign, DivAssign};

/// A trait for types that act as scalars in mathematical operations, like real numbers or complex numbers.
/// I concider a scalar, any struct which contains all the traits listed bellow.
/// f32 is an example if the Scalar implementation.
/// In the future a Complex Number struct implementation could potentially fit and allign with all the traits listed bellow.
pub trait Scalar: Copy + Clone + Debug
    + Add<Output = Self> 
    + Sub<Output = Self> 
    + Mul<Output = Self> 
    + Div<Output = Self>
    + AddAssign
    + SubAssign
    + MulAssign
    + DivAssign {}

impl Scalar for f32 {
    // For now, we implement `Scalar` for f32, which represents real numbers.
    // Later, I can add the Complex numbers implementation, which should not change so much
}
