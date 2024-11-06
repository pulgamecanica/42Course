use crate::scalar::Scalar;
use crate::Vector;
use std::fmt;

/// A struct representing a mathematical matrix.
/// 
/// The `Matrix` struct is generic over type `K` that implements the `Scalar`
/// type, that which ensures that it supports basic arithmetic operations
/// like addition, subtraction, multiplication, and division.
#[derive(Debug, Clone)]
pub struct Matrix<K: Scalar> {
    // The underlying data of the matrix stored as a `Vec<Vec<K>>`.
    // First Vec represents the "row", each element of Vec<Vec> represents the "col"
    pub data: Vec<Vec<K>>,
}

impl<K: Scalar, const N: usize> From<[K; N]> for Matrix<K> {
    /// The From trait is implemented to convert an array [K; N] to a square `Matrix<K>`.
    /// Inside the function, we use `array.to_vec()` to convert the array into a `Vec<K>`,
    /// which is then passed to the `Matrix::new()` function to create a `Matrix<K>`.
    ///
    /// # Panics
    ///
    /// The function will panic if the matrix conversion makes no sense (no square)
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// let v = Matrix::from([
    /// 7., 4.,
    /// -2., 2.
    /// ]);
    /// ```
    fn from(array: [K; N]) -> Self {
        Vector::new(array.to_vec()).reshape(f32::sqrt(N as f32) as usize, f32::sqrt(N as f32) as usize)
    }
}

impl<K: Scalar + fmt::Display> fmt::Display for Matrix<K> {
    /// Implement fmt to display a matrix whenever you want
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// let m = Matrix::from([
    ///     2., 3.,
    ///     4., 5.
    /// ]);
    /// println!("{}", m);
    /// // [2.0, 3.0]
    /// // [4.0, 5.0]
    /// ```
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for row in &self.data {
            write!(f, "[")?;
            let mut first = true;
            for col in row {
                if !first {
                    write!(f, ", ")?;
                }
                write!(f, "{}", col)?;
                first = false;
            }
            write!(f, "]\n")?;
        }
        Ok(())
    }
}

impl<K: Scalar> Matrix<K> {
    /// Creates a new `Matrix<K>` from a matrix of matrices (rows of the matrix).
    ///
    /// # Arguments
    ///
    /// * `data` - A 2D matrix of `K` values representing the rows of the matrix.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![vec![1.0, 2.0], vec![3.0, 4.0]]);
    /// ```
    pub fn new(data: Vec<Vec<K>>) -> Self {
        Self { data }
    }

    /// Returns the size (i.e., number of rows and columns) of the matrix.
    ///
    /// # Returns
    ///
    /// A tuple `(rows, cols)` representing the dimensions of the matrix.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![vec![1.0, 2.0], vec![3.0, 4.0]]);
    /// assert_eq!(mat.size(), (2, 2));
    /// ```
    pub fn size(&self) -> (usize, usize) {
        let rows = self.data.len();
        let cols = if rows > 0 { self.data[0].len() } else { 0 };
        (rows, cols)
    }

    /// Returns the number of rows in the matrix
    fn rows(&self) -> usize {
        self.size().0
    }

    /// Returns the number of columns in the matrix
    fn cols(&self) -> usize {
        self.size().1
    }

    /// Returns whether the matrix is square.
    ///
    /// A matrix is square if the number of rows equals the number of columns.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![vec![1.0, 2.0], vec![3.0, 4.0]]);
    /// assert!(mat.is_square());
    /// ```
    pub fn is_square(&self) -> bool {
        let (rows, cols) = self.size();
        rows == cols
    }

    /// Prints the contents of the matrix to the standard output.
    ///
    /// The matrix will be printed row by row, with each row on a new line, like:
    /// 
    /// ```text
    /// [1.0, 2.0]
    /// [3.0, 4.0]
    /// ```
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![vec![1.0, 2.0], vec![3.0, 4.0]]);
    /// mat.print();
    /// ```
    pub fn print(&self) {
        for row in &self.data {
            println!("{:?}", row);
        }
    }

    /// Reshapes the `Martix` into a `Vector` by flattening its rows.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![vec![1.0, 2.0], vec![3.0, 4.0]]);
    /// let vec = mat.flatten();
    /// assert_eq!(vec.size(), 4);
    /// ```
    pub fn flatten(&self) -> Vector<K> {
        let data = self.data.iter().flat_map(|row| row.clone()).collect();
        Vector { data }
    }

    /// Adds another `Matrix<K>` to the calling `Matrix<K>`.
    ///
    /// # Arguments
    ///
    /// * `v` - A reference to the other `Matrix<K>` to add.
    ///
    /// # Panics
    ///
    /// This function will panic if the matrixes are not of the same size.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// let mut mat1 = Matrix::new(vec![
    ///     vec![42.0, 4.2],
    ///     vec![1.0, 2.0]
    /// ]);
    /// let mat2 = Matrix::new(vec![
    ///     vec![-42.0, 4.2],
    ///     vec![1.0, 2.0]
    /// ]);
    ///
    /// mat1.add(&mat2);
    ///
    /// assert_eq!(mat1.data, vec![
    ///     vec![0.0, 8.4],
    ///     vec![2.0, 4.0]
    /// ]);
    /// ```
    pub fn add(&mut self, v: &Matrix<K>) {
        assert_eq!(self.size(), v.size(), "Matrices must be the same size for addition.");

        for (row_id, row) in self.data.iter_mut().enumerate() {
            for (col_id, elem) in row.iter_mut().enumerate() {
                *elem += v.data[row_id][col_id];
            }
        }
    }

    /// Substracts another `Matrix<K>` to the calling `Matrix<K>`.
    ///
    /// # Arguments
    ///
    /// * `v` - A reference to the other `Matrix<K>` to subtract.
    ///
    /// # Panics
    ///
    /// This function will panic if the `Matrix`s are not of the same size.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// let mut mat1 = Matrix::new(vec![
    ///     vec![42.0, 4.2],
    ///     vec![1.0, 2.0]
    /// ]);
    /// let mat2 = Matrix::new(vec![
    ///     vec![1.0, 2.0],
    ///     vec![-42.0, 4.2]
    /// ]);
    ///
    /// mat1.sub(&mat2);
    ///
    /// assert_eq!(mat1.data, vec![
    ///     vec![42.0 - 1.0, 4.2 - 2.0],
    ///     vec![1.0 - -42.0, 2.0 - 4.2]
    /// ]);
    /// ```
    pub fn sub(&mut self, v: &Matrix<K>) {
        assert_eq!(self.size(), v.size(), "Matrices must be the same size for subtraction.");

        for (row_id, row) in self.data.iter_mut().enumerate() {
            for (col_id, elem) in row.iter_mut().enumerate() {
                *elem -= v.data[row_id][col_id];
            }
        }
    }

    /// Scales the calling `Matrix<K>` by a factor `a`.
    ///
    /// Each element in the `Matrix` is multiplied by the scalar `a`.
    ///
    /// # Arguments
    ///
    /// * `a` - The scaling factor.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// let mut mat = Matrix::new(vec![
    ///     vec![1.0, 2.0],
    ///     vec![3.0, 4.0]
    /// ]);
    ///
    /// mat.scl(2.0); // Scale by 2
    ///
    /// assert_eq!(mat.data, vec![
    ///     vec![2.0, 4.0],
    ///     vec![6.0, 8.0]
    /// ]);
    /// ```
    pub fn scl(&mut self, a: K) {
        for row in &mut self.data {
            for elem in row {
                *elem *= a;
            }
        }
    }

    /// Multiplies the matrix `self` by a vector `vec`, producing a new vector as the result.
    ///
    /// This function performs a matrix-vector multiplication, where `self` is an `(m x n)` matrix
    /// and `vec` is a vector of size `(n)`. The resulting vector will have a size of `(m)`.
    ///
    /// Given a matrix `(A)` and a vector `(u)`, the product `(v = A x u)` is calculated as follows:
    ///
    /// ```text
    /// v[i] = sum(A[i][j] * u[j] for j in 0..n)
    /// ```
    ///
    /// where:
    /// - `i` iterates over the rows of the matrix `A` (from `0` to `m-1`),
    /// - `j` iterates over the columns of `A` and the elements of the vector `u` (from `0` to `n-1`).
    ///
    /// Each element `v[i]` in the resulting vector is the dot product of the `i`-th row of `A`
    /// with the vector `u`.
    ///
    /// # Arguments
    ///
    /// * `vec` - A `Vector<K>` representing the vector to multiply with the matrix.
    ///
    /// # Panics
    ///
    /// This function will panic if the number of columns in the matrix does not match the vector's size,
    /// as the multiplication would be undefined in that case.
    ///
    /// # Returns
    ///
    /// A `Vector<K>` that is the result of the matrix-vector multiplication, with a length equal
    /// to the number of rows in `self`.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::{Matrix, Vector};
    ///
    /// // Define a 2x3 matrix
    /// let mat = Matrix::new(vec![
    ///     vec![1.0, 2.0, 3.0],
    ///     vec![4.0, 5.0, 6.0],
    /// ]);
    ///
    /// // Define a vector of size 3
    /// let vec = Vector::new(vec![7.0, 8.0, 9.0]);
    ///
    /// // Multiply the matrix by the vector
    /// let result = mat.mul_vec(&vec);
    ///
    /// // Expected result is a vector of size 2:
    /// // [50.0, 122.0]
    /// assert_eq!(result.data, vec![50.0, 122.0]);
    /// ```
    pub fn mul_vec(&self, vec: &Vector<K>) -> Vector<K> {
        assert_eq!(self.cols(), vec.size(), "Incompatible dimensions for multiplication.");
        
        let mut result_data = vec![K::zero(); self.rows()];
        
        for i in 0..self.rows() {
            for j in 0..self.cols() {
                result_data[i] += self.data[i][j] * vec.data[j];
            }
        }
        
        Vector::new(result_data)
    }

    /// Multiplies `self` with another matrix `other`.
    ///
    /// This function performs matrix multiplication on two matrices, `self` and `other`, and returns a new matrix.
    /// The resulting matrix will have dimensions `m x p`, where:
    /// - `m` is the number of rows in `self`.
    /// - `p` is the number of columns in `other`.
    ///
    /// Given two matrices, `A` and `B`, the product `C = A * B` is calculated as follows:
    ///
    /// ```text
    /// C[i][j] = sum(A[i][k] * B[k][j] for k in 0..n)
    /// ```
    ///
    /// where:
    /// - `i` is the row index of `A` (from `0` to `m-1`)
    /// - `j` is the column index of `B` (from `0` to `p-1`)
    /// - `k` iterates over the shared dimension (from `0` to `n-1`).
    ///
    /// Each element `C[i][j]` in the resulting matrix is the dot product of the `i`-th row of `A`
    /// and the `j`-th column of `B`.
    ///
    /// # Arguments
    ///
    /// * `other` - A reference to another `Matrix<K>` to multiply with `self`.
    ///
    /// # Panics
    ///
    /// This function will panic if the number of columns in `self` does not match the number of rows in `other`,
    /// as this makes matrix multiplication undefined.
    ///
    /// # Returns
    ///
    /// A new `Matrix<K>` containing the product of `self` and `other`, with dimensions `m x p`.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// // Define a 2x3 matrix
    /// let mat1 = Matrix::new(vec![
    ///     vec![1.0, 2.0, 3.0],
    ///     vec![4.0, 5.0, 6.0],
    /// ]);
    ///
    /// // Define a 3x2 matrix
    /// let mat2 = Matrix::new(vec![
    ///     vec![7.0, 8.0],
    ///     vec![9.0, 10.0],
    ///     vec![11.0, 12.0],
    /// ]);
    ///
    /// // Multiply the matrices
    /// let result = mat1.mul_mat(&mat2);
    ///
    /// // Expected result is a 2x2 matrix:
    /// // [[58.0, 64.0],
    /// //  [139.0, 154.0]]
    /// assert_eq!(result.data, vec![
    ///     vec![58.0, 64.0],
    ///     vec![139.0, 154.0],
    /// ]);
    /// ```
    pub fn mul_mat(&self, other: &Matrix<K>) -> Matrix<K> {
        assert_eq!(self.cols(), other.rows(), "Incompatible dimensions for matrix multiplication.");

        let m = self.rows();
        let n = self.cols();
        let p = other.cols();

        // Initialize a matrix of zeros with dimensions (m, p)
        let mut result_data = vec![vec![K::zero(); p]; m];

        // Perform matrix multiplication
        for i in 0..m {
            for j in 0..p {
                let mut sum = K::zero();
                for k in 0..n {
                    sum += self.data[i][k] * other.data[k][j];
                }
                result_data[i][j] = sum;
            }
        }

        Matrix { data: result_data }
    }

    /// Computes the trace of a square matrix, which is the sum of its diagonal elements.
    ///
    /// For a square matrix `A` of size `n x n`, the trace is defined as the sum of all elements
    /// along the diagonal, where each diagonal element is represented as `A[i][i]`.
    ///
    /// For a matrix `A`:
    ///
    /// ```text
    /// trace(A) = A[0][0] + A[1][1] + ... + A[n-1][n-1]
    /// ```
    ///
    /// - Time Complexity: `O(n)`, where `n` is the number of rows (or columns) in the matrix.
    ///   The function iterates only through the diagonal elements.
    /// - Space Complexity: `O(1)`, since only a single accumulator variable is used.
    ///
    /// # Returns
    ///
    /// Returns a value of type `K`, representing the trace of the matrix.
    ///
    /// # Panics
    ///
    /// This function will panic if the matrix is not square, i.e., if the number of rows does not
    /// equal the number of columns. The trace is only defined for square matrices.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![
    ///     vec![1.0_f32, 2.0, 3.0],
    ///     vec![4.0, 5.0, 6.0],
    ///     vec![7.0, 8.0, 9.0],
    /// ]);
    /// assert_eq!(mat.trace(), 15.0); // 1.0 + 5.0 + 9.0
    /// ```
    pub fn trace(&self) -> K {
        assert_eq!(self.rows(), self.cols(), "Matrix must be square to compute trace.");

        let mut sum = K::zero();
        for i in 0..self.rows() {
            sum += self.data[i][i];
        }
        sum
    }

}

use std::ops::{AddAssign, SubAssign, MulAssign};

impl<K: Scalar> AddAssign for Matrix<K> {
    fn add_assign(&mut self, other: Self) {
        self.add(&other)
    }
}

impl<K: Scalar> SubAssign for Matrix<K> {
    fn sub_assign(&mut self, other: Self) {
        self.sub(&other)
    }
}

impl<K: Scalar> MulAssign<K> for Matrix<K> {
    fn mul_assign(&mut self, scalar: K) {
        self.scl(scalar)
    }
}

// Unit Tests

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_matrix_size() {
        let mat = Matrix::new(vec![vec![1.0, 2.0], vec![3.0, 4.0]]);
        assert_eq!(mat.size(), (2, 2));
    }

    #[test]
    fn test_is_square() {
        let mat = Matrix::new(vec![vec![1.0, 2.0], vec![3.0, 4.0]]);
        assert!(mat.is_square());
    }

    #[test]
    fn test_matrix_print() {
        let mat = Matrix { data: vec![vec![1.0, 2.0], vec![3.0, 4.0]] };
        println!("{mat:?}");
    }

    #[test]
    fn test_matrix_add_basic() {
        let mut mat1 = Matrix::new(vec![
            vec![1.0, 2.0],
            vec![3.0, 4.0]
        ]);
        let mat2 = Matrix::new(vec![
            vec![1.0, 1.0],
            vec![1.0, 1.0]
        ]);

        mat1.add(&mat2);

        assert_eq!(mat1.data, vec![
            vec![2.0, 3.0],
            vec![4.0, 5.0]
        ]);
    }

    #[test]
    #[should_panic(expected = "Matrices must be the same size for addition.")]
    fn test_matrix_add_size_mismatch() {
        let mut mat1 = Matrix::new(vec![
            vec![1.0, 2.0]
        ]);
        let mat2 = Matrix::new(vec![
            vec![1.0, 1.0],
            vec![1.0, 1.0]
        ]);

        mat1.add(&mat2);
    }

    #[test]
    fn test_matrix_add_negative_values() {
        let mut mat1 = Matrix::new(vec![
            vec![1.0, -2.0],
            vec![3.0, -4.0]
        ]);
        let mat2 = Matrix::new(vec![
            vec![5.0, 6.0],
            vec![-7.0, 8.0]
        ]);

        mat1.add(&mat2);

        assert_eq!(mat1.data, vec![
            vec![6.0, 4.0],
            vec![-4.0, 4.0]
        ]);
    }

    #[test]
    fn test_matrix_add_empty_matrix() {
        let mut mat1: Matrix<f32> = Matrix::new(vec![]);
        let mat2: Matrix<f32> = Matrix::new(vec![]);

        mat1.add(&mat2);

        assert_eq!(mat1.data, vec![] as Vec<Vec<f32>>);
    }

    #[test]
    fn test_matrix_sub_basic() {
        let mut mat1 = Matrix::new(vec![
            vec![5.0, 6.0],
            vec![7.0, 8.0]
        ]);
        let mat2 = Matrix::new(vec![
            vec![1.0, 2.0],
            vec![3.0, 4.0]
        ]);

        mat1.sub(&mat2);

        assert_eq!(mat1.data, vec![
            vec![4.0, 4.0],
            vec![4.0, 4.0]
        ]);
    }

    #[test]
    #[should_panic(expected = "Matrices must be the same size for subtraction.")]
    fn test_matrix_sub_size_mismatch() {
        let mut mat1 = Matrix::new(vec![
            vec![1.0, 2.0]
        ]);
        let mat2 = Matrix::new(vec![
            vec![1.0, 1.0],
            vec![1.0, 1.0]
        ]);

        mat1.sub(&mat2);
    }

    #[test]
    fn test_matrix_sub_negative_values() {
        let mut mat1 = Matrix::new(vec![
            vec![10.0, -5.0],
            vec![-3.0, 6.0]
        ]);
        let mat2 = Matrix::new(vec![
            vec![1.0, 2.0],
            vec![3.0, 4.0]
        ]);

        mat1.sub(&mat2);

        assert_eq!(mat1.data, vec![
            vec![9.0, -7.0],
            vec![-6.0, 2.0]
        ]);
    }

    #[test]
    fn test_matrix_sub_empty_matrix() {
        let mut mat1: Matrix<f32> = Matrix::new(vec![]);
        let mat2: Matrix<f32> = Matrix::new(vec![]);

        mat1.sub(&mat2);

        assert_eq!(mat1.data, vec![] as Vec<Vec<f32>>);
    }

    #[test]
    fn test_matrix_scl_basic() {
        let mut mat = Matrix::new(vec![
            vec![1.0, 2.0],
            vec![3.0, 4.0]
        ]);

        mat.scl(2.0);

        assert_eq!(mat.data, vec![
            vec![2.0, 4.0],
            vec![6.0, 8.0]
        ]);
    }

    #[test]
    fn test_matrix_scl_zero() {
        let mut mat = Matrix::new(vec![
            vec![1.0, 2.0],
            vec![3.0, 4.0]
        ]);

        mat.scl(0.0);

        assert_eq!(mat.data, vec![
            vec![0.0, 0.0],
            vec![0.0, 0.0]
        ]);
    }

    #[test]
    fn test_matrix_scl_negative() {
        let mut mat = Matrix::new(vec![
            vec![1.0, -2.0],
            vec![3.0, -4.0]
        ]);

        mat.scl(-2.0);

        assert_eq!(mat.data, vec![
            vec![-2.0, 4.0],
            vec![-6.0, 8.0]
        ]);
    }

    #[test]
    fn test_matrix_scl_empty_matrix() {
        let mut mat: Matrix<f32> = Matrix::new(vec![]);
        mat.scl(5.0);

        assert_eq!(mat.data, vec![] as Vec<Vec<f32>>);
    }

    #[test]
    fn test_mul_vec_f32() {
        // Simple 2x3 matrix and 3-element vector multiplication
        let mat = Matrix::new(vec![
            vec![1.0_f32, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
        ]);
        let vec = Vector::new(vec![7.0_f32, 8.0, 9.0]);

        let result = mat.mul_vec(&vec);
        assert_eq!(result.data, vec![50.0, 122.0]);
    }

    #[test]
    fn test_mul_vec_f64() {
        let mat: Matrix<f64> = Matrix::new(vec![
            vec![1.0, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
        ]);
        let vec = Vector::new(vec![7.0_f64, 8.0, 9.0]);

        let result = mat.mul_vec(&vec);
        assert_eq!(result.data, vec![50.0, 122.0]);
    }

    #[test]
    fn test_mul_vec_i32() {
        let mat: Matrix<i32> = Matrix::new(vec![
            vec![1, 2, 3],
            vec![4, 5, 6],
        ]);
        let vec: Vector<i32> = Vector::new(vec![7, 8, 9]);

        let result = mat.mul_vec(&vec);
        assert_eq!(result.data, vec![50, 122]);
    }

    #[test]
    #[should_panic(expected = "Incompatible dimensions for multiplication.")]
    fn test_mul_vec_panic_on_incompatible_dimensions() {
        // Test with incompatible dimensions (2x2 matrix and 3-element vector)
        let mat = Matrix::new(vec![
            vec![1.0_f32, 2.0],
            vec![3.0, 4.0],
        ]);
        let vec = Vector::new(vec![5.0_f32, 6.0, 7.0]);

        mat.mul_vec(&vec);
    }

    #[test]
    fn test_mul_vec_zero_vector() {
        let mat = Matrix::new(vec![
            vec![1.0_f32, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
        ]);
        let vec = Vector::new(vec![0.0_f32, 0.0, 0.0]);

        let result = mat.mul_vec(&vec);
        assert_eq!(result.data, vec![0.0, 0.0]);
    }

    #[test]
    fn test_mul_mat_f32() {
        // Simple 2x3 * 3x2 matrix multiplication
        let mat_a: Matrix<f32> = Matrix::new(vec![
            vec![1.0, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
        ]);
        let mat_b: Matrix<f32> = Matrix::new(vec![
            vec![7.0, 8.0],
            vec![9.0, 10.0],
            vec![11.0, 12.0],
        ]);

        let result = mat_a.mul_mat(&mat_b);
        assert_eq!(result.data, vec![
            vec![58.0, 64.0],
            vec![139.0, 154.0]
        ]);
    }

    #[test]
    fn test_mul_mat_f64() {
        let mat_a: Matrix<f64> = Matrix::new(vec![
            vec![1.0, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
        ]);
        let mat_b: Matrix<f64> = Matrix::new(vec![
            vec![7.0, 8.0],
            vec![9.0, 10.0],
            vec![11.0, 12.0],
        ]);

        let result: Matrix<f64> = mat_a.mul_mat(&mat_b);
        assert_eq!(result.data, vec![
            vec![58.0, 64.0],
            vec![139.0, 154.0]
        ]);
    }

    #[test]
    fn test_mul_mat_i32() {
        let mat_a = Matrix::new(vec![
            vec![1, 2, 3],
            vec![4, 5, 6],
        ]);
        let mat_b = Matrix::new(vec![
            vec![7, 8],
            vec![9, 10],
            vec![11, 12],
        ]);

        let result: Matrix<i32> = mat_a.mul_mat(&mat_b);
        assert_eq!(result.data, vec![
            vec![58, 64],
            vec![139, 154]
        ]);
    }

    #[test]
    #[should_panic(expected = "Incompatible dimensions for matrix multiplication.")]
    fn test_mul_mat_panic_on_incompatible_dimensions() {
        // (2x1 matrix and 2x3 matrix)
        let mat_a: Matrix<f32> = Matrix::new(vec![
            vec![1.0],
            vec![3.0],
        ]);
        let mat_b: Matrix<f32> = Matrix::new(vec![
            vec![5.0, 6.0, 7.0],
            vec![8.0, 9.0, 10.0],
        ]);

        mat_a.mul_mat(&mat_b);
    }

    #[test]
    fn test_mul_mat_zero_matrix() {
        let mat_a = Matrix::new(vec![
            vec![1.0_f32, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
        ]);
        let mat_b = Matrix::new(vec![
            vec![0.0_f32, 0.0],
            vec![0.0, 0.0],
            vec![0.0, 0.0],
        ]);

        let result = mat_a.mul_mat(&mat_b);
        assert_eq!(result.data, vec![
            vec![0.0, 0.0],
            vec![0.0, 0.0]
        ]);
    }

    #[test]
    fn test_trace_2x2() {
        let mat = Matrix::new(vec![
            vec![1.0, 2.0],
            vec![3.0, 4.0],
        ]);
        assert_eq!(mat.trace(), 5.0);
    }

    #[test]
    fn test_trace_3x3() {
        let mat = Matrix::new(vec![
            vec![1.0, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
            vec![7.0, 8.0, 9.0],
        ]);
        assert_eq!(mat.trace(), 15.0);
    }

    #[test]
    fn test_trace_4x4_f64() {
        let mat = Matrix::new(vec![
            vec![1.0_f64, 2.0, 3.0, 4.0],
            vec![5.0, 6.0, 7.0, 8.0],
            vec![9.0, 10.0, 11.0, 12.0],
            vec![13.0, 14.0, 15.0, 16.0],
        ]);
        assert_eq!(mat.trace(), 34.0);
    }

    #[test]
    fn test_trace_1x1() {
        let mat = Matrix::new(vec![
            vec![7.0_f32],
        ]);
        assert_eq!(mat.trace(), 7.0);
    }

    #[test]
    fn test_trace_large_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0, 0.0, 0.0, 0.0, 0.0],
            vec![0.0, 2.0, 0.0, 0.0, 0.0],
            vec![0.0, 0.0, 3.0, 0.0, 0.0],
            vec![0.0, 0.0, 0.0, 4.0, 0.0],
            vec![0.0, 0.0, 0.0, 0.0, 5.0],
        ]);
        assert_eq!(mat.trace(), 15.0);
    }

    #[test]
    #[should_panic(expected = "Matrix must be square to compute trace.")]
    fn test_trace_panic_non_square() {
        let mat = Matrix::new(vec![
            vec![1.0, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
        ]);
        let result = mat.trace();
        println!("{result}");
    }

    #[test]
    fn test_trace_integer_elements() {
        let mat = Matrix::new(vec![
            vec![1, 2],
            vec![3, 4],
        ]);
        assert_eq!(mat.trace(), 5);
    }
}
