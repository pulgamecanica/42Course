use std::fmt;
use crate::Scalar;
use std::ops::{Add, Sub, Mul, Div, AddAssign, SubAssign, MulAssign, DivAssign};
use num::Zero;

/// A struct representing a complex number with `f32` real and imaginary components.
#[derive(Copy, Clone, Debug, PartialEq, PartialOrd)]
pub struct Complex {
    pub real: f32,
    pub imag: f32,
}

impl Complex {
    /// Creates a new `Complex` number given a real and imaginary part.
    ///
    /// # Arguments
    ///
    /// * `real` - The real component of the complex number.
    /// * `imag` - The imaginary component of the complex number.
    ///
    /// # Returns
    ///
    /// A new `Complex` number with the specified real and imaginary components.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Complex;
    ///
    /// let complex_num = Complex::new(3.0, 4.0);
    /// assert_eq!(complex_num.real, 3.0);
    /// assert_eq!(complex_num.imag, 4.0);
    /// ```
    pub fn new(real: f32, imag: f32) -> Self {
        Self { real, imag }
    }

    /// Returns the conjugate of the complex number.
    ///
    /// The conjugate of a complex number `a + bi` is given by `a - bi`.
    ///
    /// <img src="https://github.com/user-attachments/assets/0c6c385f-7bf4-4892-9e9a-ca8bc4aea0b2" alt="complex numbers magnitude formula" />
    ///
    /// # Returns
    ///
    /// A new `Complex` number with the same real component and a negated imaginary component.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Complex;
    ///
    /// let complex_num = Complex::new(3.0, 4.0);
    /// let conjugate = complex_num.conjugate();
    /// assert_eq!(conjugate, Complex::new(3.0, -4.0));
    /// ```
    pub fn conjugate(self) -> Self {
        Self { real: self.real, imag: -self.imag }
    }

    /// Computes the magnitude (or modulus) of the complex number.
    ///
    /// The magnitude of a complex number \( z = a + bi \) is calculated as:
    ///
    /// <img src="https://github.com/user-attachments/assets/628a3bbc-43d6-4503-863b-25d885084c6f" alt="complex numbers magnitude formula" />
    ///
    /// where `a` is the real part and `b` is the imaginary part of the complex number.
    ///
    /// # Returns
    ///
    /// A `f64` value representing the magnitude of the complex number.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Complex;
    ///
    /// let complex_num = Complex::new(3.0, 4.0);
    /// assert_eq!(complex_num.magnitude(), 5.0);
    /// ```
    pub fn magnitude(&self) -> f32 {
        (self.real * self.real + self.imag * self.imag).sqrt()
    }
}

impl fmt::Display for Complex {
    /// Formats the `Complex` for display.
    ///
    /// # Parameters
    /// - `f`: A mutable reference to a `fmt::Formatter` for formatting.
    ///
    /// # Returns
    /// - `fmt::Result`: Indicates success or failure of the formatting.
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "({} + {}i)", self.real, self.imag)
    }
}

impl Add for Complex {
    type Output = Self;

    /// Adds two `Complex` numbers.
    ///
    /// Given two complex numbers `a + bi` and `c + di`, the sum is:
    ///
    /// <img src="https://github.com/user-attachments/assets/00316ce6-d1f2-4078-b8b5-d489659e538f" alt="complex nums addition" />
    ///
    /// # Arguments
    ///
    /// * `self` - The first `Complex` number.
    /// * `other` - The second `Complex` number to add.
    ///
    /// # Returns
    ///
    /// A new `Complex` number which is the sum of `self` and `other`.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Complex;
    ///
    /// let a = Complex::new(3.0, 4.0);
    /// let b = Complex::new(1.0, 2.0);
    /// assert_eq!(a + b, Complex::new(4.0, 6.0));
    /// ```
    fn add(self, other: Self) -> Self::Output {
        Self {
            real: self.real + other.real,
            imag: self.imag + other.imag,
        }
    }
}

impl Sub for Complex {
    type Output = Self;

    /// Subtracts one `Complex` number from another.
    ///
    /// Given two complex numbers `a + bi` and `c + di`, the difference is:
    ///
    /// <img src="https://github.com/user-attachments/assets/f8a0b1a3-a4f7-4917-a72c-0bd43f4f6d43" alt="complex nums addition" />
    ///
    /// # Arguments
    ///
    /// * `self` - The `Complex` number to be subtracted from.
    /// * `other` - The `Complex` number to subtract.
    ///
    /// # Returns
    ///
    /// A new `Complex` number which is the difference of `self` and `other`.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Complex;
    ///
    /// let a = Complex::new(3.0, 4.0);
    /// let b = Complex::new(1.0, 2.0);
    /// assert_eq!(a - b, Complex::new(2.0, 2.0));
    /// ```
    fn sub(self, other: Self) -> Self::Output {
        Self {
            real: self.real - other.real,
            imag: self.imag - other.imag,
        }
    }
}

impl Mul for Complex {
    type Output = Self;

    /// Multiplies two `Complex` numbers.
    ///
    /// Given two complex numbers `a + bi` and `c + di`, the product is:
    ///
    /// <img src="https://github.com/user-attachments/assets/16e7ccd6-f4e5-4a4f-a3d0-31e2dc8d5f32" alt="complex nums multiplication formula" />
    ///
    /// # Arguments
    ///
    /// * `self` - The first `Complex` number.
    /// * `other` - The second `Complex` number to multiply.
    ///
    /// # Returns
    ///
    /// A new `Complex` number which is the product of `self` and `other`.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Complex;
    ///
    /// let a = Complex::new(3.0, 4.0);
    /// let b = Complex::new(1.0, 2.0);
    /// assert_eq!(a * b, Complex::new(-5.0, 10.0));
    /// ```
    fn mul(self, other: Self) -> Self::Output {
        Self {
            real: self.real * other.real - self.imag * other.imag,
            imag: self.real * other.imag + self.imag * other.real,
        }
    }
}

impl Div for Complex {
    type Output = Self;

    /// Divides one `Complex` number by another.
    ///
    /// Given two complex numbers `a + bi` and `c + di`, the quotient is:
    ///
    /// <img src="https://github.com/user-attachments/assets/5b5c6ec8-9ac3-4644-b832-7426f9998a37" alt="complex nums multiplication formula" />
    ///
    /// # Arguments
    ///
    /// * `self` - The `Complex` number to be divided.
    /// * `other` - The `Complex` number to divide by.
    ///
    /// # Returns
    ///
    /// A new `Complex` number which is the quotient of `self` and `other`.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Complex;
    ///
    /// let a = Complex::new(3.0, 2.0);
    /// let b = Complex::new(1.0, -1.0);
    /// let result = a / b;
    /// assert!((result.real - 0.5).abs() < 1e-10);
    /// assert!((result.imag - 2.5).abs() < 1e-10);
    /// ```
    fn div(self, other: Self) -> Self::Output {
        let denominator = other.real * other.real + other.imag * other.imag;
        Self {
            real: (self.real * other.real + self.imag * other.imag) / denominator,
            imag: (self.imag * other.real - self.real * other.imag) / denominator,
        }
    }
}

impl AddAssign for Complex {
    /// Adds another `Complex` number to `self`.
    fn add_assign(&mut self, other: Self) {
        *self = *self + other;
    }
}

impl SubAssign for Complex {
    /// Subtracts another `Complex` number from `self`.
    fn sub_assign(&mut self, other: Self) {
        *self = *self - other;
    }
}

impl MulAssign for Complex {
    /// Multiplies `self` by another `Complex` number.
    fn mul_assign(&mut self, other: Self) {
        *self = *self * other;
    }
}

impl DivAssign for Complex {
    /// Divides `self` by another `Complex` number.
    fn div_assign(&mut self, other: Self) {
        *self = *self / other;
    }
}

impl Zero for Complex {
    /// Returns the additive identity of `Complex`, which is 0 + 0i.
    fn zero() -> Self {
        Self { real: 0.0, imag: 0.0 }
    }

    /// Checks if the `Complex` number is zero (both real and imaginary parts are zero).
    fn is_zero(&self) -> bool {
        self.real == 0.0 && self.imag == 0.0
    }
}

impl Scalar for Complex {
    /// Fused Multiply-Add (FMA): Computes `a * b + c` for `Complex` numbers.
    fn fma(a: Self, b: Self, c: Self) -> Self {
        a * b + c
    }

    /// Fused Multiply-Subtract (FMS): Computes `a * b - c` for `Complex` numbers.
    fn fms(a: Self, b: Self, c: Self) -> Self {
        a * b - c
    }

    /// Converts an `f32` value to a `Complex` number with a zero imaginary part.
    ///
    /// # Arguments
    ///
    /// * `value` - The `f32` value to convert.
    ///
    /// # Returns
    ///
    /// A new `Complex` number with the real part set to `value` and imaginary part set to 0.
    fn from_f32(value: f32) -> Self {
        Self {
            real: value,
            imag: 0.0,
        }
    }

    /// Converts the real part of the `Complex` number to `f32`, ignoring the imaginary part.
    ///
    /// # Returns
    ///
    /// The real part of `self` as an `f32` value.
    fn to_f32(self) -> f32 {
        self.magnitude()
    }
}


#[cfg(test)]
mod tests {
    use crate::Matrix;
    use crate::Vector;
    use super::*;

    #[test]
    fn test_complex_magnitude() {
        let complex_num = Complex::new(3.0, 4.0);
        assert_eq!(complex_num.magnitude(), 5.0);
    }

    #[test]
    fn test_vector_creation_and_size() {
        let vec = Vector::new(vec![Complex::new(1.0, 2.0), Complex::new(3.0, 4.0)]);
        assert_eq!(vec.size(), 2);
    }

    #[test]
    fn test_vector_linear_combination() {
        let v1 = Vector::new(vec![Complex::new(1.0, 2.0)]);
        let v2 = Vector::new(vec![Complex::new(3.0, 4.0)]);
        let coefs = vec![Complex::new(2.0, 0.0), Complex::new(-1.0, 0.0)];
        let result = Vector::linear_combination(&[v1, v2], &coefs);
        assert_eq!(result, Vector::new(vec![Complex::new(-1.0, 0.0)]));
    }

    #[test]
    fn test_vector_norms() {
        let v = Vector::new(vec![Complex::new(3.0, 4.0)]);
        assert_eq!(v.norm_1(), 5.0);
        assert_eq!(v.norm(), 5.0);
        assert_eq!(v.norm_inf(), 5.0);
    }

    #[test]
    fn test_vector_addition_and_subtraction() {
        let mut v1 = Vector::new(vec![Complex::new(1.0, 2.0)]);
        let v2 = Vector::new(vec![Complex::new(3.0, 4.0)]);
        v1.add(&v2);
        assert_eq!(v1, Vector::new(vec![Complex::new(4.0, 6.0)]));

        v1.sub(&v2);
        assert_eq!(v1, Vector::new(vec![Complex::new(1.0, 2.0)]));
    }

    #[test]
    fn test_vector_scaling() {
        let mut v = Vector::new(vec![Complex::new(1.0, 2.0)]);
        v.scl(Complex::new(2.0, 0.0));
        assert_eq!(v, Vector::new(vec![Complex::new(2.0, 4.0)]));
    }

    #[test]
    fn test_matrix_creation_and_size() {
        let mat = Matrix::new(vec![
            vec![Complex::new(1.0, 2.0), Complex::new(3.0, 4.0)],
            vec![Complex::new(5.0, 6.0), Complex::new(7.0, 8.0)]
        ]);
        assert_eq!(mat.size(), (2, 2));
    }

    #[test]
    fn test_matrix_addition_and_subtraction() {
        let mut m1 = Matrix::new(vec![
            vec![Complex::new(1.0, 2.0), Complex::new(3.0, 4.0)],
        ]);
        let m2 = Matrix::new(vec![
            vec![Complex::new(5.0, 6.0), Complex::new(7.0, 8.0)],
        ]);
        m1.add(&m2);
        assert_eq!(m1, Matrix::new(vec![
            vec![Complex::new(6.0, 8.0), Complex::new(10.0, 12.0)],
        ]));

        m1.sub(&m2);
        assert_eq!(m1, Matrix::new(vec![
            vec![Complex::new(1.0, 2.0), Complex::new(3.0, 4.0)],
        ]));
    }

    #[test]
    fn test_matrix_transpose() {
        let mat = Matrix::new(vec![
            vec![Complex::new(1.0, 2.0), Complex::new(3.0, 4.0)],
            vec![Complex::new(5.0, 6.0), Complex::new(7.0, 8.0)]
        ]);
        let transposed = mat.transpose();
        assert_eq!(transposed, Matrix::new(vec![
            vec![Complex::new(1.0, 2.0), Complex::new(5.0, 6.0)],
            vec![Complex::new(3.0, 4.0), Complex::new(7.0, 8.0)]
        ]));
    }

    #[test]
    fn test_matrix_trace() {
        let mat = Matrix::new(vec![
            vec![Complex::new(1.0, 0.0), Complex::new(0.0, 0.0)],
            vec![Complex::new(0.0, 0.0), Complex::new(2.0, 0.0)],
        ]);
        assert_eq!(mat.trace(), Complex::new(3.0, 0.0));
    }

    #[test]
    fn test_matrix_determinant() {
        let mat = Matrix::new(vec![
            vec![Complex::new(4.0, 0.0), Complex::new(7.0, 0.0)],
            vec![Complex::new(2.0, 0.0), Complex::new(6.0, 0.0)],
        ]);
        assert_eq!(mat.determinant(), Complex::new(10.0, 0.0));
    }

    #[test]
    fn test_matrix_inverse() {
        let mat = Matrix::new(vec![
            vec![Complex::new(4.0, 0.0), Complex::new(7.0, 0.0)],
            vec![Complex::new(2.0, 0.0), Complex::new(6.0, 0.0)],
        ]);
        let inv = mat.inverse().unwrap();
        let identity = mat.mul_mat(&inv);
        assert!(identity.data[0][1].to_f32() == 0.0 && identity.data[1][0].to_f32() == 0.0 && identity.data[0][0].to_f32() >= 1.0 && identity.data[0][0].to_f32() < 1.1 && identity.data[1][1].to_f32() >= 1.0 && identity.data[1][1].to_f32() < 1.1);
    }

    #[test]
    fn test_matrix_rank() {
        let mat = Matrix::new(vec![
            vec![Complex::new(1.0, 0.0), Complex::new(2.0, 0.0)],
            vec![Complex::new(2.0, 0.0), Complex::new(4.0, 0.0)],
        ]);
        assert_eq!(mat.rank(), 1);
    }
}
