use std::fmt::Debug;
use std::ops::{Add, Sub, Mul, Div, AddAssign, SubAssign, MulAssign, DivAssign};
use num::Zero;

/// A trait for types that act as scalars in mathematical operations, like real numbers or complex numbers.
/// I concider a scalar, any struct which contains all the traits listed bellow.
/// f32 is an example if the Scalar implementation.
/// In the future a Complex Number struct implementation could potentially fit and allign with all the traits listed bellow.
pub trait Scalar: Copy + Clone + Debug + Zero
    + Add<Output = Self> 
    + Sub<Output = Self> 
    + Mul<Output = Self> 
    + Div<Output = Self>
    + AddAssign
    + SubAssign
    + MulAssign
    + DivAssign
    + PartialOrd
    + PartialEq
{
    /// Fused Multiply-Add (FMA): Default implementation that computes `a * b + c`.
    fn fma(a: Self, b: Self, c: Self) -> Self {
        a * b + c
    }


    /// Fused Multiply-Sub (FMS): Default implementation that computes `a * b - c`.
    fn fms(a: Self, b: Self, c: Self) -> Self {
        a * b - c
    }

    fn to_f32(self) -> f32;

    fn from_f32(value: f32) -> Self;
}

// Specialize for f32 using SIMD FMA intrinsics
#[cfg(target_arch = "x86_64")]
use core::arch::x86_64::{
    _mm_fmadd_ps, _mm_set1_ps, _mm_cvtss_f32
};

impl Scalar for f32 {
    fn fma(a: f32, b: f32, c: f32) -> f32 {
        unsafe {
            // Use SIMD registers to do FMA: a * b + c
            let vec_a = _mm_set1_ps(a);
            let vec_b = _mm_set1_ps(b);
            let vec_c = _mm_set1_ps(c);
            let result = _mm_fmadd_ps(vec_a, vec_b, vec_c);
            _mm_cvtss_f32(result) // Return the first element of the SIMD result
        }
    }

    fn fms(a: f32, b: f32, c: f32) -> f32 {
        unsafe {
            // Use SIMD registers to do FMA: a * b + c
            let vec_a = _mm_set1_ps(a);
            let vec_b = _mm_set1_ps(b);
            let vec_c = _mm_set1_ps(-c);
            let result = _mm_fmadd_ps(vec_a, vec_b, vec_c);
            _mm_cvtss_f32(result) // Return the first element of the SIMD result
        }
    }

    fn from_f32(value: f32) -> Self {
        value
    }

    fn to_f32(self) -> f32 {
        self as f32
    }
}

// Default behavior (no specialization) for `f64` `i32`, `i64`, `u32`, `u64`, etc.
impl Scalar for f64 {
    fn from_f32(value: f32) -> Self {
        value as f64
    }

    fn to_f32(self) -> f32 {
        self as f32
    }
}

impl Scalar for i32 {
    fn from_f32(value: f32) -> Self {
        value as i32
    }

    fn to_f32(self) -> f32 {
        self as f32
    }
}

impl Scalar for i64 {
    fn from_f32(value: f32) -> Self {
        value as i64
    }

    fn to_f32(self) -> f32 {
        self as f32
    }
}

impl Scalar for u32 {
    fn from_f32(value: f32) -> Self {
        value as u32
    }

    fn to_f32(self) -> f32 {
        self as f32
    }
}

impl Scalar for u64 {
    fn from_f32(value: f32) -> Self {
        value as u64
    }

    fn to_f32(self) -> f32 {
        self as f32
    }
}