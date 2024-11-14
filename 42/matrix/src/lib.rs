//! # Vector and Matrix Library
//! 
//! This library provides functionality for working with vectors and matrices. 
//! It includes utility functions for operations like calculating size, reshaping, and printing.


/// The scalar module provides a trait generic for scalars.
pub mod scalar;

/// The vector module provides a generic `Vector` struct and related methods.
pub mod vector;

/// The matrix module provides a generic `Matrix` struct and related methods.
pub mod matrix;

/// The complex module provides a representation of Complex Numbers which implements Scalar.
pub mod complex;

/// The linear interpolation module provides a generic function implementation to apply an interpolation.
pub mod linear_interpolation;

/// The projection matrix module provides a function to generate projection matrices.
pub mod projection_matrix;

// Optionally, re-export types to make them easier to access from the top level.
pub use vector::Vector;
pub use matrix::Matrix;
pub use scalar::Scalar;
pub use complex::Complex;
pub use linear_interpolation::lerp;
pub use projection_matrix::projection;

