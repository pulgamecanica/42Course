//! # Vector and Matrix Library
//! 
//! This library provides functionality for working with vectors and matrices. 
//! It includes utility functions for operations like calculating size, reshaping, and printing.

/// The vector module provides a generic `Vector` struct and related methods.
pub mod vector;

/// The matrix module provides a generic `Matrix` struct and related methods.
pub mod matrix;

// Optionally, re-export types to make them easier to access from the top level.
pub use vector::Vector;
pub use matrix::Matrix;
