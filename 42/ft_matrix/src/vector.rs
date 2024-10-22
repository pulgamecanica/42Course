use crate::scalar::Scalar;
use crate::Matrix;
use std::fmt;

/// A struct representing a mathematical vector that is generic over type `K`.
///
/// The type `K` must implement the `Scalar` trait, which ensures that it supports
/// basic arithmetic operations like addition, subtraction, multiplication, and division.
#[derive(Debug)]
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
    pub fn reshape(&self, rows: usize, cols: usize) -> Matrix<K> {
        assert_eq!(self.data.len(), rows * cols, "Vector size does not match the dimensions of the matrix.");
        assert!(rows * cols != 0, "Reshape invalid dimensions.");

        let data = self.data
            .chunks(cols)
            .map(|chunk| chunk.to_vec())
            .collect();

        Matrix { data }
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

    /// Test using negative coefficients
    #[test]
    fn test_linear_combination_negative_coefficients() {
        let vec1 = Vector::new(vec![1, 2, 3, 4]);
        let vec2 = Vector::new(vec![-5, -15, -25, -35]);
        let coefs = vec![2, -5];

        let result = Vector::linear_combination(&[vec1, vec2], &coefs);

        assert_eq!(result.data, vec![2 * 1 - 5 * -5, 2 * 2 - 5 * -15, 2 * 3 - 5 * -25, 2 * 4 - 5 * -35]);
    }
}
