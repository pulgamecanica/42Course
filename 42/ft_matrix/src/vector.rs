use crate::scalar::Scalar;
use crate::Matrix;
use std::fmt;

/// A struct representing a mathematical vector that is generic over type `K`.
///
/// The type `K` must implement the `Scalar` trait, which ensures that it supports
/// basic arithmetic operations like addition, subtraction, multiplication, and division.
#[derive(Debug, Clone)]
pub struct Vector<K: Scalar> {
    // The underlying data of the vector stored as a `Vec<K>`.
    pub data: Vec<K>,
}

impl<K: Scalar, const N: usize> From<[K; N]> for Vector<K> {
    /// The From trait is implemented to convert an array [K; N] to a `Vector<K>`.
    /// Inside the function, we use `array.to_vec()` to convert the array into a `Vec<K>`,
    /// which is then passed to the `Vector::new()` function to create a `Vector<K>`.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    ///
    /// println!("{}", Vector::from([42.0, 42.0]));
    /// ```
    fn from(array: [K; N]) -> Self {
        Vector::new(array.to_vec())
    }
}

impl<K: Scalar + fmt::Display> fmt::Display for Vector<K> {
    /// Implement fmt to display a vector whenever you want
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    ///
    /// let v = Vector::from([2., 3.]);
    /// println!("{}", v);
    /// // [2.0, 3.0]
    /// ```
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "[")?;
        let mut first = true;
        for val in &self.data {
            if !first {
                write!(f, ", ")?;
            }
            write!(f, "{}", val)?;
            first = false;
        }
        write!(f, "]")
    }
}

impl<K: Scalar> Vector<K> {
    /// Creates a new `Vector<K>` from a vector of `K` values.
    ///
    /// # Arguments
    ///
    /// * `data` - A `Vec<K>` representing the elements of the vector.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    ///
    /// let vec = Vector::new(vec![1.0, 2.0, 3.0]);
    /// ```
    pub fn new(data: Vec<K>) -> Self {
        Self { data }
    }

    /// Returns the size (i.e., the number of elements) of the `Vector`.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    /// 
    /// let vec = Vector::new(vec![1.0, 2.0, 3.0]);
    /// assert_eq!(vec.size(), 3);
    /// ```
    ///
    /// # Returns
    ///
    /// The size of the `Vector<K>`
    pub fn size(&self) -> usize {
        self.data.len()
    }

    /// Prints the contents of the `Vector` to the standard output.
    ///
    /// The `Vector` will be printed in a single line format, like:
    /// 
    /// ```text
    /// [1.0, 2.0, 3.0]
    /// ```
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    ///
    /// let vec = Vector::new(vec![1.0, 2.0, 3.0]);
    /// vec.print();
    /// ```
    pub fn print(&self) {
        println!("{:?}", self.data);
    }

    /// Reshapes the `Vector` into a matrix with the specified number of rows and columns.
    ///
    /// # Arguments
    ///
    /// * `rows` - The number of rows in the resulting matrix.
    /// * `cols` - The number of columns in the resulting matrix.
    ///
    /// # Panics
    ///
    /// This function will panic if the size of the `Vector` does not match the
    /// requested dimensions (i.e., `rows * cols` must equal `self.size()`).
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    ///
    /// let vec = Vector::new(vec![1.0, 2.0, 3.0, 4.0]);
    /// let mat = vec.reshape(2, 2);
    /// assert_eq!(mat.size(), (2, 2));
    /// ```
    ///
    /// # Returns
    ///
    /// A `Matrix<K>` from the `Vector<K>`
    pub fn reshape(&self, rows: usize, cols: usize) -> Matrix<K> {
        assert_eq!(self.data.len(), rows * cols, "Vector size does not match the dimensions of the matrix.");
        assert!(rows * cols != 0, "Reshape invalid dimensions.");

        let data = self.data
            .chunks(cols)
            .map(|chunk| chunk.to_vec())
            .collect();

        Matrix { data }
    }

    /// Computes the linear combination of a set of vectors using Fused Multiply-Add (FMA).
    ///
    /// This implementation uses SIMD intrinsics for performance when the type `K` allows it.
    ///
    /// # Panics
    ///
    /// This function will panic if the length of `u` and `coefs` does not match,
    /// or if the vectors in `u` are not all of the same size.
    ///
    /// # Arguments
    ///
    /// * `u` - A vector of `Vector`s of type `K` to be combined.
    /// * `coefs` - A vector coefficients of type `K`, each corresponding to a vector in `u`.
    ///
    /// # Returns
    ///
    /// A `Vector<K>` with all the linear combinations
    pub fn linear_combination(u: &[Vector<K>], coefs: &[K]) -> Vector<K> {
        assert_eq!(u.len(), coefs.len(), "Vectors and coefficients must have the same length.");
        if u.is_empty() {
            return Vector::new(vec![]);
        }
        
        let size = u[0].size();
        for vector in u.iter() {
            assert_eq!(vector.size(), size, "All vectors must have the same size.");
        }
        
        let mut result = Vector::new(vec![K::zero(); size]);
        for (i, vector) in u.iter().enumerate() {
            let coef = coefs[i];
            for (j, elem) in vector.data.iter().enumerate() {
                // result.data[j] += *elem * coef; // 
                result.data[j] = K::fma(*elem, coef, result.data[j]);
            }
        }
        result
    }

    /// Computes the dot product with another `Vector<K>`
    ///
    /// # Arguments
    ///
    /// * `v` - A reference to the other `Vector<K>`.
    ///
    /// # Panics
    ///
    /// This function will panic if the vectors are not of the same size.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    ///
    /// let vec1 = Vector::new(vec![42.0, 4.2]);
    /// let vec2 = Vector::new(vec![-42.0, 4.2]);
    /// assert_eq!(vec1.dot(&vec2), -1746.36);
    /// ```
    ///
    /// # Returns
    ///
    /// The result of the dot product of type `K`
    pub fn dot(&self, v: &Vector::<K>) -> K {
        assert_eq!(self.size(), v.size());
        let mut result = K::zero();

        for (a, b) in self.data.iter().zip(v.data.iter()) {
            result = K::fma(*a, *b, result);
        }
        result
    }

    /// Computes the 1-norm (Manhattan norm) of the vector.
    /// 
    /// The 1-norm is defined as the sum of the absolute values of each element in the vector.
    /// 
    /// <div>
    /// <h3> Formula </h3>
    /// <img src="https://github.com/user-attachments/assets/aa97364a-0260-41ea-a723-2250d156565a" alt="Manhattan norm"/>
    /// </div>
    /// <hr>
    ///
    /// # Example
    /// 
    /// ```
    /// use ft_matrix::Vector;
    /// 
    /// let vec = Vector::new(vec![3, -4, 5]);
    /// assert_eq!(vec.norm_1(), 12.0);
    /// ```
    /// 
    /// # Returns
    /// 
    /// The computed 1-norm as type `K`, where `K` implements `Scalar`.
    pub fn norm_1(&self) -> f32 {
        self.data.iter().map(|&x| x.to_f32().abs()).sum()
    }

    /// Computes the 2-norm (Euclidean norm) of the vector.
    /// 
    /// The 2-norm is defined as the square root of the sum of the squares of each element in the vector.
    /// 
    /// <div>
    /// <h3> Formula </h3>
    /// <img src="https://github.com/user-attachments/assets/4e796701-2c65-4ecb-b189-d041ce48551e" alt="Euclidean norm"/>
    /// </div>
    /// <hr>
    ///
    /// # Example
    /// 
    /// ```
    /// use ft_matrix::Vector;
    /// 
    /// let vec = Vector::new(vec![3.0, -4.0, 5.0]);
    /// assert_eq!(vec.norm(), (3.0_f32.powi(2) + 4.0_f32.powi(2) + 5.0_f32.powi(2)).sqrt());
    /// ```
    /// 
    /// # Returns
    /// 
    /// The computed 2-norm as an `f32`.
    /// 
    /// # Notes
    /// 
    /// If available, this function uses `fma` (fused multiply-add) to improve numerical accuracy.
    pub fn norm(&self) -> f32 {
        let mut sum_of_squares = 0.0;
        for &x in &self.data {
            sum_of_squares = K::fma(x, x, K::from_f32(sum_of_squares)).to_f32();
        }
        sum_of_squares.sqrt()
    }

    /// Computes the ∞-norm (supremum or maximum norm) of the vector.
    /// 
    /// The ∞-norm is defined as the maximum absolute value of the elements in the vector.
    /// 
    /// <div>
    /// <h3> Formula </h3>
    /// <img src="https://github.com/user-attachments/assets/622e614c-e17e-4733-9352-31f4d65977de" alt="Supremum/maximum norm"/>
    /// </div>
    /// <hr>
    ///
    /// # Example
    /// 
    /// ```
    /// use ft_matrix::Vector;
    /// 
    /// let vec = Vector::new(vec![3.0, -4.0, 5.0]);
    /// assert_eq!(vec.norm_inf(), 5.0);
    /// ```
    /// 
    /// # Returns
    /// 
    /// The computed ∞-norm as an `f32`.
    pub fn norm_inf(&self) -> f32 {
        self.data.iter().map(|&x| x.to_f32().abs()).fold(0.0, f32::max)
    }

    /// Adds another `Vector<K>` to the calling `Vector<K>`.
    ///
    /// # Arguments
    ///
    /// * `v` - A reference to the other `Vector<K>` to add.
    ///
    /// # Panics
    ///
    /// This function will panic if the vectors are not of the same size.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    ///
    /// let mut vec1 = Vector::new(vec![42.0, 4.2]);
    /// let vec2 = Vector::new(vec![-42.0, 4.2]);
    /// vec1.add(&vec2);
    /// assert_eq!(vec1.data, vec![0.0, 8.4]);
    /// ```
    pub fn add(&mut self, v: &Vector<K>) {
        assert_eq!(self.size(), v.size(), "Vectors must be the same size for addition.");

        for (i, val) in v.data.iter().enumerate() {
            self.data[i] += *val;
        }
    }

    /// Substracts another `Vector<K>` to the calling `Vector<K>`.
    ///
    /// # Arguments
    ///
    /// * `v` - A reference to the other `Vector<K>` to subtract.
    ///
    /// # Panics
    ///
    /// This function will panic if the `Vector`s are not of the same size.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    ///
    /// let mut vec1 = Vector::new(vec![42.0, 4.2]);
    /// let vec2 = Vector::new(vec![-42.0, 4.2]);
    /// vec1.sub(&vec2);
    /// assert_eq!(vec1.data, vec![84.0, 0.0]);
    /// ```
    pub fn sub(&mut self, v: &Vector<K>) {
        assert_eq!(self.size(), v.size(), "Vectors must be the same size for subtraction.");

        for (i, val) in v.data.iter().enumerate() {
            self.data[i] -= *val;
        }
    }

    /// Scales the calling `Vector<K>` by a factor `a`.
    ///
    /// Each element in the `Vector` is multiplied by the scalar `a`.
    ///
    /// # Arguments
    ///
    /// * `a` - The scaling factor.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    ///
    /// let mut vec1 = Vector::new(vec![42.0, 4.2]);
    /// vec1.scl(2.0);
    /// assert_eq!(vec1.data, vec![84.0, 8.4]);
    /// ```
    pub fn scl(&mut self, a: K) {
        for elem in &mut self.data {
            *elem *= a;
        }
    }

    /// Calculates the cosine of the angle between two `Vector`s, `u` and `v`.
    ///
    /// This function computes the cosine of the angle θ between two vectors `u` and `v`
    /// using the formula:
    ///
    /// ```text
    /// cos(θ) = (u ⋅ v) / (‖u‖ * ‖v‖)
    /// ```
    ///
    /// where `u ⋅ v` is the dot product of `u` and `v`, and `‖u‖` and `‖v‖` are the norms (magnitudes) of the vectors `u` and `v`.
    ///
    /// The cosine value returned is between -1.0 and 1.0, where:
    /// - 1.0 indicates that the vectors are parallel and pointing in the same direction,
    /// - -1.0 indicates that the vectors are parallel and pointing in opposite directions,
    /// - 0.0 indicates that the vectors are perpendicular.
    ///
    /// # Arguments
    ///
    /// * `u` - The first `Vector`.
    /// * `v` - The second `Vector`.
    ///
    /// # Panics
    ///
    /// This function will panic if the `Vector`s are not the same size or if either vector has zero magnitude (norm).
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    ///
    /// let vec1 = Vector::new(vec![0, 1]);
    /// let vec2 = Vector::new(vec![0, -1]);
    /// assert_eq!(Vector::angle_cos(&vec1, &vec2), -1.0);
    /// ```
    ///
    /// # Returns
    ///
    /// Returns a `f32` value between -1.0 and 1.0, representing the cosine of the angle between `u` and `v`.
    pub fn angle_cos(u: &Vector<K>, v: &Vector<K>) -> f32 {
        assert_eq!(u.size(), v.size(), "Vectors must be the same size.");

        let u_norm = u.norm();
        let v_norm = v.norm();

        if u_norm == 0.0 || v_norm == 0.0 {
            panic!("Cannot compute angle between zero vectors.");
        }

        let cos_theta = Scalar::to_f32(Vector::dot(u, v)) / (u_norm * v_norm);
        cos_theta.clamp(-1.0, 1.0)
    }

}

use std::ops::{AddAssign, SubAssign, MulAssign};

impl<K: Scalar> AddAssign for Vector<K> {
    fn add_assign(&mut self, other: Self) {
        self.add(&other)
    }
}

impl<K: Scalar> SubAssign for Vector<K> {
    fn sub_assign(&mut self, other: Self) {
        self.sub(&other)
    }
}

impl<K: Scalar> MulAssign<K> for Vector<K> {
    fn mul_assign(&mut self, scalar: K) {
        self.scl(scalar)
    }
}

// Unit Tests

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_vector_size() {
        let vec = Vector::new(vec![1.0, 2.0, 3.0]);
        assert_eq!(vec.size(), 3);
    }

    #[test]
    fn test_vector_size_zero() {
        let vec: Vector<f32> = Vector::new(vec![]);
        assert_eq!(vec.size(), 0);
    }

    #[test]
    fn test_vector_reshape_square() {
        let vec = Vector::new(vec![1.0, 2.0, 3.0, 4.0]);
        let mat = vec.reshape(2, 2);
        assert_eq!(mat.size(), (2, 2));
    }

    #[test]
    fn test_vector_reshape() {
        let vec = Vector::new(vec![1.0, 2.0, 3.0]);
        let mat = vec.reshape(1, 3);
        assert_eq!(mat.size(), (1, 3));
    }

    #[test]
    #[should_panic(expected = "Vector size does not match the dimensions of the matrix.")]
    fn test_vector_reshape_few_elements() {
        let vec = Vector::new(vec![1.0, 2.0, 3.0]);
        let mat = vec.reshape(2, 3);
        assert_eq!(mat.size(), (2, 3));
    }

    #[test]
    #[should_panic(expected = "Vector size does not match the dimensions of the matrix.")]
    fn test_vector_reshape_lots_of_elements() {
        let vec = Vector::new(vec![1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0]);
        let mat = vec.reshape(2, 3);
        assert_eq!(mat.size(), (2, 3));
    }

    #[test]
    #[should_panic(expected = "Reshape invalid dimensions.")]
    fn test_vector_reshape_invalid_dimensions() {
        let vec: Vector<f32> = Vector::new(vec![]);
        let mat = vec.reshape(0, 0);
        assert_eq!(mat.size(), (0, 0));
    }

    #[test]
    fn test_vector_add_with_zero_vector() {
        let mut vec1 = Vector::new(vec![10.0, 20.0, 30.0]);
        let vec2 = Vector::new(vec![0.0, 0.0, 0.0]);
        vec1.add(&vec2);

        assert_eq!(vec1.data, vec![10.0, 20.0, 30.0]);
    }

    #[test]
    fn test_vector_add_negative_numbers() {
        let mut vec1 = Vector::new(vec![10.0, -5.0, 30.0]);
        let vec2 = Vector::new(vec![-10.0, -5.0, -30.0]);
        vec1.add(&vec2);

        assert_eq!(vec1.data, vec![0.0, -10.0, 0.0]);
    }

    #[test]
    fn test_vector_add_large_numbers() {
        let mut vec1 = Vector::new(vec![1e6, 2e6, 3e6]);
        let vec2 = Vector::new(vec![1e6, 2e6, 3e6]);
        vec1.add(&vec2);

        assert_eq!(vec1.data, vec![2e6, 4e6, 6e6]);
    }

    #[test]
    #[should_panic(expected = "Vectors must be the same size for addition.")]
    fn test_vector_add_panic_on_different_sizes() {
        let mut vec1 = Vector::new(vec![10.0, 20.0]);
        let vec2 = Vector::new(vec![1.0, 2.0, 3.0]);

        vec1.add(&vec2);
    }

    #[test]
    fn test_vector_add_empty_vectors() {
        let mut vec1 : Vector<f32> = Vector::new(vec![]);
        let vec2: Vector<f32> = Vector::new(vec![]);
        vec1.add(&vec2);

        assert_eq!(vec1.data, vec![]);
    }

    #[test]
    fn test_vector_sub_with_zero_vector() {
        let mut vec1 = Vector::new(vec![10.0, 20.0, 30.0]);
        let vec2 = Vector::new(vec![0.0, 0.0, 0.0]);
        vec1.sub(&vec2);

        assert_eq!(vec1.data, vec![10.0, 20.0, 30.0]);
    }

    #[test]
    fn test_vector_sub_negative_numbers() {
        let mut vec1 = Vector::new(vec![10.0, -5.0, 30.0]);
        let vec2 = Vector::new(vec![-10.0, -5.0, -30.0]);
        vec1.sub(&vec2);

        assert_eq!(vec1.data, vec![20.0, 0.0, 60.0]);
    }

    #[test]
    fn test_vector_sub_large_numbers() {
        let mut vec1 = Vector::new(vec![1e6, -2e6, 3e6]);
        let vec2 = Vector::new(vec![1e6, 2e6, -3e6]);
        vec1.sub(&vec2);

        assert_eq!(vec1.data, vec![0.0, -4e6, 6e6]);
    }

    #[test]
    #[should_panic(expected = "Vectors must be the same size for subtraction.")]
    fn test_vector_sub_panic_on_different_sizes() {
        let mut vec1 = Vector::new(vec![10.0, 20.0]);
        let vec2 = Vector::new(vec![3.0]);

        vec1.sub(&vec2);
    }

    #[test]
    fn test_vector_sub_empty_vectors() {
        let mut vec1 : Vector<f32> = Vector::new(vec![]);
        let vec2: Vector<f32> = Vector::new(vec![]);
        vec1.add(&vec2);

        assert_eq!(vec1.data, vec![]);
    }

    #[test]
    fn test_vector_scaling_basic() {
        let mut vec1 = Vector::new(vec![42.0, 4.2]);
        vec1.scl(2.0);

        assert_eq!(vec1.data, vec![84.0, 8.4]);
    }


    #[test]
    fn test_vector_scaling_basic_2() {
        let mut vec1: Vector<f64> = Vector::new(vec![42.0, 4.2]);
        let scalar: f64 = 2.0;
        vec1 *= scalar;

        assert_eq!(vec1.data, vec![84.0, 8.4]);
    }

    #[test]
    fn test_vector_scaling_by_zero() {
        let mut vec1 = Vector::new(vec![10.0, 20.0, 30.0]);
        vec1.scl(0.0);

        assert_eq!(vec1.data, vec![0.0, 0.0, 0.0]);
    }

    #[test]
    fn test_vector_scaling_by_negative() {
        let mut vec1 = Vector::new(vec![10.0, -5.0, 30.0]);
        vec1.scl(-1.0);

        assert_eq!(vec1.data, vec![-10.0, 5.0, -30.0]);
    }

    #[test]
    fn test_vector_scaling_with_large_factor() {
        let mut vec1 = Vector::new(vec![1.0, 2.0, 3.0]);
        vec1.scl(1e6);

        assert_eq!(vec1.data, vec![1e6, 2e6, 3e6]);
    }

    #[test]
    fn test_vector_scaling_empty_vector() {
        let mut vec1: Vector<f32> = Vector::new(vec![]);
        vec1.scl(5.0);

        assert_eq!(vec1.data, vec![]);
    }

    /// Test linear combination using f32 with FMA
    #[test]
    fn test_linear_combination_fma_f32() {
        let vec1 = Vector::new(vec![1.0, 2.0, 3.0, 4.0]);
        let vec2 = Vector::new(vec![0.5, 1.5, 2.5, 3.5]);
        let coefs = vec![2.0, 0.5];

        let result = Vector::linear_combination(&[vec1, vec2], &coefs);

        assert_eq!(result.data, vec![2.0 * 1.0 + 0.5 * 0.5, 2.0 * 2.0 + 0.5 * 1.5, 2.0 * 3.0 + 0.5 * 2.5, 2.0 * 4.0 + 0.5 * 3.5]);
    }

    /// Test linear combination using i32 (without FMA, should fallback to a * b + c)
    #[test]
    fn test_linear_combination_i32() {
        let vec1 = Vector::new(vec![1, 2, 3, 4]);
        let vec2 = Vector::new(vec![1, 2, 3, 4]);
        let coefs = vec![2, 1];

        let result = Vector::linear_combination(&[vec1, vec2], &coefs);

        assert_eq!(result.data, vec![3, 6, 9, 12]);
    }

    #[test]
    fn test_linear_combination_empty() {
        let vec1: Vector<f32> = Vector::new(vec![]);
        let vec2: Vector<f32> = Vector::new(vec![]);
        let coefs = vec![2.0, 0.5];

        let result = Vector::linear_combination(&[vec1, vec2], &coefs);

        assert_eq!(result.data, vec![]);
    }

    #[test]
    #[should_panic(expected = "Vectors and coefficients must have the same length.")]
    fn test_linear_combination_mismatched_sizes() {
        let vec1 = Vector::new(vec![1.0, 2.0, 3.0]);
        let vec2 = Vector::new(vec![1.0, 2.0, 3.0]);
        let coefs = vec![2.0];  // Coefficient list is too short (1 coef for 2 vectors)

        // This should panic because the number of vectors and coefficients don't match
        let _ = Vector::linear_combination(&[vec1, vec2], &coefs);
    }

    #[test]
    fn test_linear_combination_negative_coefficients() {
        let vec1 = Vector::new(vec![1, 2, 3, 4]);
        let vec2 = Vector::new(vec![-5, -15, -25, -35]);
        let coefs = vec![2, -5];

        let result = Vector::linear_combination(&[vec1, vec2], &coefs);

        assert_eq!(result.data, vec![2 * 1 - 5 * -5, 2 * 2 - 5 * -15, 2 * 3 - 5 * -25, 2 * 4 - 5 * -35]);
    }


    #[test]
    fn test_dot_product_vec_i32() {
        let vec1: Vector<i32> = Vector::new(vec![1, 1]);
        let vec2: Vector<i32> = Vector::new(vec![1, 1]);

        assert_eq!(vec1.dot(&vec2), 2);
    }

    #[test]
    fn test_dot_product_vec_f32() {
        let vec1 = Vector::new(vec![3.0, 5.5]);
        let vec2 = Vector::new(vec![-1.0, 2.0]);

        assert_eq!(vec1.dot(&vec2), 8.0);
    }


    #[test]
    fn test_norm_1_f32() {
        let vec = Vector::new(vec![3.0, -4.0, 5.0]);
        let result = vec.norm_1();
        assert_eq!(result, 12.0);
    }

    #[test]
    fn test_norm_1_f64() {
        let vec: Vector<f64> = Vector::new(vec![3.0, -4.5, 5.0]);
        let result = vec.norm_1();
        assert_eq!(result, 12.5);
    }

    #[test]
    fn test_norm_1_u32() {
        let vec = Vector::new(vec![3, 4, 5]);
        let result = vec.norm_1();
        assert_eq!(result, 12.0);
    }

    #[test]
    fn test_norm_f32() {
        let vec: Vector<f32> = Vector::new(vec![3.0, 4.0]);
        let result = vec.norm();
        assert_eq!(result, 5.0);
    }

    #[test]
    fn test_norm_f64() {
        let vec: Vector<f64> = Vector::new(vec![3.0, 4.0]);
        let result = vec.norm();
        assert_eq!(result, 5.0);
    }


    #[test]
    fn test_norm_i32() {
        let vec: Vector<i32> = Vector::new(vec![6, 8]);
        let result = vec.norm();
        assert_eq!(result, 10.0);
    }

    #[test]
    fn test_norm_u32() {
        let vec: Vector<u32> = Vector::new(vec![6, 8]);
        let result = vec.norm();
        assert_eq!(result, 10.0);
    }

    #[test]
    fn test_norm_inf_f32() {
        let vec: Vector<f32> = Vector::new(vec![3.0, -7.0, 5.0]);
        let result = vec.norm_inf();
        assert_eq!(result, 7.0);
    }

    #[test]
    fn test_norm_inf_f64() {
        let vec: Vector<f64> = Vector::new(vec![3.0, -7.0, 5.0]);
        let result = vec.norm_inf();
        assert_eq!(result, 7.0);
    }

    #[test]
    fn test_norm_inf_u32() {
        let vec: Vector<u32> = Vector::new(vec![2, 9, 5]);
        let result = vec.norm_inf();
        assert_eq!(result, 9.0);
    }

    #[test]
    fn test_angle_cos_parallel_vectors() {
        let vec1 = Vector::new(vec![1.0, 0.0]);
        let vec2 = Vector::new(vec![2.0, 0.0]);
        assert_eq!(Vector::angle_cos(&vec1, &vec2), 1.0);
    }

    #[test]
    fn test_angle_cos_opposite_vectors() {
        let vec1 = Vector::new(vec![1.0, 0.0]);
        let vec2 = Vector::new(vec![-1.0, 0.0]);
        assert_eq!(Vector::angle_cos(&vec1, &vec2), -1.0);
    }

    #[test]
    fn test_angle_cos_perpendicular_vectors() {
        let vec1 = Vector::new(vec![1.0, 0.0]);
        let vec2 = Vector::new(vec![0.0, 1.0]);
        assert_eq!(Vector::angle_cos(&vec1, &vec2), 0.0);
    }

    #[test]
    fn test_angle_cos_with_f32() {
        let vec1 = Vector::new(vec![1.0f32, 2.0, 3.0]);
        let vec2 = Vector::new(vec![1.0f32, 2.0, 3.0]);
        let result = Vector::angle_cos(&vec1, &vec2); 
        println!("{result}");
        assert!(result > 0.999);
    }

    #[test]
    fn test_angle_cos_with_f64() {
        let vec1 = Vector::new(vec![1.0f64, 2.0, 3.0]);
        let vec2 = Vector::new(vec![4.0f64, -5.0, 6.0]);
        let result = Vector::angle_cos(&vec1, &vec2);
        println!("{result}");
        assert!(result > 0.365 && result < 0.366);
    }

    #[test]
    fn test_angle_cos_different_sizes() {
        let vec1 = Vector::new(vec![1.0, 2.0, 3.0]);
        let vec2 = Vector::new(vec![4.0, 5.0]);
        let result = std::panic::catch_unwind(|| Vector::angle_cos(&vec1, &vec2));
        assert!(result.is_err());
    }

    #[test]
    fn test_angle_cos_with_zero_vector() {
        let vec1 = Vector::new(vec![1.0, 2.0, 3.0]);
        let vec2 = Vector::new(vec![0.0, 0.0, 0.0]);
        let result = std::panic::catch_unwind(|| Vector::angle_cos(&vec1, &vec2));
        assert!(result.is_err());
    }

    #[test]
    fn test_angle_cos_with_integer_vectors() {
        let vec1 = Vector::new(vec![1, 2, 3]);
        let vec2 = Vector::new(vec![4, -5, 6]);
        let result = Vector::angle_cos(&vec1, &vec2);
        assert!(result > 0.365 && result < 0.366);
    }

    #[test]
    fn test_angle_cos_with_large_vectors() {
        let vec1 = Vector::new(vec![1000.0, 2000.0, 3000.0]);
        let vec2 = Vector::new(vec![4000.0, -5000.0, 6000.0]);
        let result = Vector::angle_cos(&vec1, &vec2);
        assert!(result > 0.365 && result < 0.366);
    }

    #[test]
    fn test_angle_cos_identical_vectors() {
        let vec1 = Vector::new(vec![3.0, 4.0, 5.0]);
        let vec2 = Vector::new(vec![3.0, 4.0, 5.0]);
        assert_eq!(Vector::angle_cos(&vec1, &vec2), 1.0);
    }

    #[test]
    fn test_angle_cos_zero_length_vectors() {
        let vec1 = Vector::new(vec![0.0, 0.0, 0.0]);
        let vec2 = Vector::new(vec![0.0, 0.0, 0.0]);
        let result = std::panic::catch_unwind(|| Vector::angle_cos(&vec1, &vec2));
        assert!(result.is_err());
    }
}
