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

impl<K: Scalar, const N: usize, const M: usize> From<[[K; M]; N]> for Matrix<K> {
    /// The From trait is implemented to convert a 2D array `[[K; M]; N]` into a `Matrix<K>`.
    /// The function takes a 2D array (with N rows and M columns) and creates a `Matrix<K>`
    /// by converting it into a `Vec<Vec<K>>`, where each row of the matrix is a `Vec<K>`.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Matrix;
    ///
    /// let u = Matrix::from([
    ///     [8., 5., -2.],
    ///     [4., 7., 20.],
    ///     [7., 6., 1.],
    /// ]);
    /// ```
    ///
    /// ```
    /// use matrix::Matrix;
    ///
    /// let u = Matrix::from([
    ///     [2., 0., 0.],
    ///     [0., 2., 0.],
    /// ]);
    /// ```
    fn from(array: [[K; M]; N]) -> Self {
        // Convert the 2D array into a Vec<Vec<K>>
        let vec_of_vecs: Vec<Vec<K>> = array
            .iter()
            .map(|&row| row.to_vec()) // Convert each row to a Vec<K>
            .collect();

        // Create the Matrix using the vector of vectors
        Matrix::new(vec_of_vecs)
    }
}

impl<K: Scalar + fmt::Display> fmt::Display for Matrix<K> {
    /// Implement fmt to display a matrix whenever you want
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Matrix;
    ///
    /// let m = Matrix::from([
    ///     [2., 3.],
    ///     [4., 5.],
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
    /// use matrix::Matrix;
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
    /// use matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![vec![1.0, 2.0], vec![3.0, 4.0]]);
    /// assert_eq!(mat.size(), (2, 2));
    /// ```
    pub fn size(&self) -> (usize, usize) {
        let rows = self.data.len();
        let cols = if rows > 0 { self.data[0].len() } else { 0 };
        (rows, cols)
    }

    /// Returns the number of rows in the matrix.
    ///
    /// This function provides the total count of rows in the matrix `data` structure.
    /// Each row in the matrix corresponds to a nested vector within `self.data`, 
    /// so the number of rows is determined by the length of `self.data`.
    ///
    /// # Example
    /// ```
    /// use matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![
    ///     vec![1, 2, 3],
    ///     vec![4, 5, 6]
    /// ]);
    ///
    /// assert_eq!(mat.rows(), 2);
    /// ```
    ///
    /// # Returns
    /// The number of rows in the matrix as `usize`.
    pub fn rows(&self) -> usize {
        self.size().0
    }

    /// Returns the number of columns in the matrix.
    ///
    /// This function provides the count of columns in the matrix `data` structure.
    /// Each row in the matrix has the same number of elements, representing columns,
    /// so the number of columns is derived from the length of any row within `self.data`.
    ///
    /// # Example
    /// ```
    /// use matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![
    ///     vec![1, 2, 3],
    ///     vec![4, 5, 6]
    /// ]);
    ///
    /// assert_eq!(mat.cols(), 3);
    /// ```
    ///
    /// # Returns
    /// The number of columns in the matrix as `usize`.
    ///
    /// # Panics
    /// This function will panic if the matrix contains no rows.
    pub fn cols(&self) -> usize {
        self.size().1
    }

    /// Returns whether the matrix is square.
    ///
    /// A matrix is square if the number of rows equals the number of columns.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Matrix;
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
    /// use matrix::Matrix;
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
    /// use matrix::Matrix;
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
    /// use matrix::Matrix;
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
    /// use matrix::Matrix;
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
    /// use matrix::Matrix;
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
    /// use matrix::{Matrix, Vector};
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
    /// use matrix::Matrix;
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
    /// use matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![
    ///     vec![1.0_f32, 2.0, 3.0],
    ///     vec![4.0, 5.0, 6.0],
    ///     vec![7.0, 8.0, 9.0],
    /// ]);
    /// assert_eq!(mat.trace(), 15.0); // 1.0 + 5.0 + 9.0
    /// ```
    pub fn trace(&self) -> K {
        assert!(self.is_square(), "Matrix must be square to compute trace.");

        let mut sum = K::zero();
        for i in 0..self.rows() {
            sum += self.data[i][i];
        }
        sum
    }

    /// Returns the transpose of the matrix.
    ///
    /// Given a matrix `A` with dimensions `m x n`, this function produces a matrix `B` with dimensions `n x m`,
    /// where each element `B[j][i]` is equal to `A[i][j]`.
    ///
    /// # Formula
    ///
    /// For a matrix `A`:
    /// ```text
    /// B[j][i] = A[i][j]
    /// ```
    ///
    /// # Example
    /// ```
    /// use matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![
    ///     vec![1.0, 2.0, 3.0],
    ///     vec![4.0, 5.0, 6.0]
    /// ]);
    /// let transposed = mat.transpose();
    ///
    /// assert_eq!(transposed, Matrix::new(vec![
    ///     vec![1.0, 4.0],
    ///     vec![2.0, 5.0],
    ///     vec![3.0, 6.0],
    /// ]));
    /// ```
    ///
    /// # Returns
    ///
    /// A new `Matrix<K>` which is the transpose of the original matrix.
    pub fn transpose(&self) -> Matrix<K> {
        let rows = self.rows();
        let cols = self.cols();
        let mut result_data = vec![vec![K::zero(); rows]; cols];
        
        for i in 0..rows {
            for j in 0..cols {
                result_data[j][i] = self.data[i][j].clone();
            }
        }
        
        Matrix::new(result_data)
    }

    /// Converts the matrix to its reduced row-echelon form (RREF) using Gauss-Jordan elimination.
    ///
    /// # Panics
    ///
    /// This function will panic if the matrix is empty or not rectangular.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Matrix;
    ///
    /// let mut mat = Matrix::new(vec![
    ///     vec![2.0, 1.0, -1.0],
    ///     vec![-3.0, -1.0, 2.0],
    ///     vec![-2.0, 1.0, 2.0],
    /// ]);
    /// let rref = mat.row_echelon();
    /// ```
    ///
    /// # Returns
    ///
    /// A new matrix in reduced row-echelon form.
    pub fn row_echelon(&self) -> Matrix<K> {
        let rows = self.rows();
        let cols = self.cols();

        // Check if the matrix is empty
        if rows == 0 || cols == 0 {
            panic!("Matrix must have at least one row and one column.");
        }

        // Check if the matrix is rectangular
        let row_length = self.data[0].len();
        for row in &self.data {
            if row.len() != row_length {
                panic!("Matrix must be rectangular; all rows must have the same number of columns.");
            }
        }

        let mut result = self.clone();
        let mut row = 0;

        for col in 0..cols {
            if row >= rows {
                break;
            }
            // Find the row with the largest pivot in the current column
            let mut max_row = row;
            for i in (row + 1)..rows {
                if result.data[i][col].to_f32().abs() > result.data[max_row][col].to_f32().abs() {
                    max_row = i;
                }
            }

            // If the largest pivot is zero, skip this column
            if result.data[max_row][col] == K::zero() {
                continue;
            }

            // Swap to position the largest pivot at the top of the submatrix
            result.data.swap(row, max_row);

            // Normalize the pivot row to make the pivot element 1
            let pivot = result.data[row][col];
            for j in col..cols {
                result.data[row][j] /= pivot;
            }

            // Use helper to avoid borrow issues
            let pivot_row = result.data[row].clone();

            // Make all rows above and below the pivot zero in this column
            // by substracting from the pivot row and multiply from current selected col
            for i in 0..rows {
                if i != row {
                    let factor = result.data[i][col];
                    for j in col..cols {
                        result.data[i][j] -= factor * pivot_row[j];
                    }
                }
            }

            // Move to the next row for the next pivot
            row += 1;
        }

        result
    }

    /// Returns the minor matrix by removing the specified row and column.
    ///
    /// Given a square matrix, this function returns a new matrix that is one size smaller,
    /// with the specified row and column removed.
    ///
    /// # Parameters
    /// - `row`: The row index to remove.
    /// - `col`: The column index to remove.
    ///
    /// # Panics
    /// - Panics if the matrix is not square.
    /// - Panics if the matrix is 0x0 or if `row` or `col` are out of bounds.
    ///
    /// # Returns
    /// A new `Matrix<K>` that is the minor matrix of the original matrix.
    fn minor(&self, row: usize, col: usize) -> Matrix<K> {
        // Ensure the matrix is square
        let size = self.data.len();
        assert!(size > 0, "Cannot compute minor of a 0x0 matrix.");
        assert!(self.is_square(), "Matrix must be square.");
        assert!(row < size, "Row index out of bounds.");
        assert!(col < size, "Column index out of bounds.");

        if size == 1 {
            return Matrix::new(vec![]);
        }

        let mut minor_data = Vec::with_capacity(size - 1);
        for i in 0..size {
            if i == row {
                continue;
            }
            let mut minor_row = Vec::with_capacity(size - 1);
            for j in 0..size {
                if j == col {
                    continue;
                }
                minor_row.push(self.data[i][j].clone());
            }
            minor_data.push(minor_row);
        }

        Matrix::new(minor_data)
    }

    /// Computes the determinant of a 2x2 matrix.
    /// 
    /// Given a 2x2 matrix `A`, the determinant is calculated as:
    /// ```text
    /// det(A) = A[0][0] * A[1][1] - A[0][1] * A[1][0]
    /// ```
    ///
    /// # Returns
    /// 
    /// A value of type `K` representing the determinant of the matrix.
    ///
    /// # Example
    /// ```
    /// use matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![
    ///     vec![3.0, 8.0],
    ///     vec![4.0, 6.0]
    /// ]);
    /// assert_eq!(mat.determinant(), -14.0);
    /// ```
    fn determinant2x2(&self) -> K {
        K::fms(
            self.data[0][0],
            self.data[1][1],
            self.data[0][1] * self.data[1][0]
            )
    }

    /// Computes the determinant of a 3x3 matrix using cofactor expansion along the first row.
    ///
    /// This function calculates the determinant of a 3x3 matrix by expanding along the first row
    /// and using the generic `minor` function to obtain 2x2 submatrices (minors) dynamically.
    /// This approach makes the code more consistent with larger determinant calculations (e.g., for 4x4 matrices),
    /// and allows for easier extension to support matrices of other sizes.
    ///
    /// # Formula
    /// Given a 3x3 matrix `A`:
    /// ```text
    /// A = | A_11 A_12 A_13 |
    ///     | A_21 A_22 A_23 |
    ///     | A_31 A_32 A_33 |
    /// ```
    /// the determinant is calculated as:
    /// ```text
    /// det(A) = A_11 * det(M_11) - A_12 * det(M_12) + A_13 * det(M_13)
    /// ```
    /// where:
    /// - `M_11` is the minor obtained by removing the first row and first column of `A`,
    /// - `M_12` is the minor obtained by removing the first row and second column of `A`,
    /// - `M_13` is the minor obtained by removing the first row and third column of `A`,
    /// and `det(M_ij)` represents the determinant of each 2x2 minor.
    ///
    /// # Example
    /// ```
    /// use matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![
    ///     vec![3.0, 8.0, 4.0],
    ///     vec![2.0, 1.0, 7.0],
    ///     vec![6.0, 5.0, 9.0]
    /// ]);
    /// let determinant = mat.determinant();
    ///
    /// assert_eq!(determinant, 130.0);
    /// ```
    ///
    /// # Panics
    /// This function will panic if the matrix is not 3x3. Ensure that the matrix size is 3x3
    /// before calling this function.
    ///
    /// # Returns
    /// Returns the determinant of the 3x3 matrix as a scalar value of type `K`.
    fn determinant3x3(&self) -> K {
        let a11 = self.data[0][0].clone();
        let a12 = self.data[0][1].clone();
        let a13 = self.data[0][2].clone();

        a11 * self.minor(0, 0).determinant2x2()
            - a12 * self.minor(0, 1).determinant2x2()
            + a13 * self.minor(0, 2).determinant2x2()
    }

    /// Computes the determinant of a 4x4 matrix using cofactor expansion along the first row.
    ///
    /// This function calculates the determinant of a 4x4 matrix by expanding along the first row
    /// and using the generic `minor` function to obtain 3x3 submatrices (minors) dynamically.
    /// This approach leverages the cofactor expansion, which recursively calls the `determinant3x3`
    /// method on each minor.
    ///
    /// # Formula
    /// Given a 4x4 matrix `A`:
    /// ```text
    /// A = | A_11 A_12 A_13 A_14 |
    ///     | A_21 A_22 A_23 A_24 |
    ///     | A_31 A_32 A_33 A_34 |
    ///     | A_41 A_42 A_43 A_44 |
    /// ```
    /// the determinant is calculated as:
    /// ```text
    /// det(A) = A_11 * det(M_11) - A_12 * det(M_12) + A_13 * det(M_13) - A_14 * det(M_14)
    /// ```
    /// where:
    /// - `M_11` is the minor obtained by removing the first row and first column of `A`,
    /// - `M_12` is the minor obtained by removing the first row and second column of `A`,
    /// - `M_13` is the minor obtained by removing the first row and third column of `A`,
    /// - `M_14` is the minor obtained by removing the first row and fourth column of `A`,
    /// and `det(M_ij)` represents the determinant of each 3x3 minor.
    ///
    /// # Example
    /// ```
    /// use matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![
    ///     vec![1.0, 2.0, 3.0, 4.0],
    ///     vec![5.0, 6.0, 7.0, 8.0],
    ///     vec![9.0, 10.0, 11.0, 12.0],
    ///     vec![13.0, 14.0, 15.0, 16.0]
    /// ]);
    /// let determinant = mat.determinant();
    ///
    /// assert_eq!(determinant, 0.0); // Example result; in this case, the matrix is singular
    /// ```
    ///
    /// # Panics
    /// This function will panic if the matrix is not 4x4. Ensure that the matrix size is 4x4
    /// before calling this function.
    ///
    /// # Returns
    /// Returns the determinant of the 4x4 matrix as a scalar value of type `K`.
    fn determinant4x4(&self) -> K {
        let a11 = self.data[0][0].clone();
        let a12 = self.data[0][1].clone();
        let a13 = self.data[0][2].clone();
        let a14 = self.data[0][3].clone();

        a11 * self.minor(0, 0).determinant3x3()
            - a12 * self.minor(0, 1).determinant3x3()
            + a13 * self.minor(0, 2).determinant3x3()
            - a14 * self.minor(0, 3).determinant3x3()
    }

    /// Computes the determinant of the matrix, supporting dimensions from 1x1 to 4x4.
    /// 
    /// # Formula
    ///
    /// <img src="https://github.com/user-attachments/assets/e8b02a9d-8cff-4c39-9e00-b664ba9412f6" alt="determinant formula"/>
    ///
    /// For matrices with dimensions 1x1 to 4x4, the determinant is computed based on the matrix size:
    /// - 0: Returns zero.
    /// - 1x1: Returns the single element as the determinant.
    /// - 2x2: Uses the formula `A[0][0] * A[1][1] - A[0][1] * A[1][0]`.
    /// - 3x3: Uses cofactor expansion on the 3x3 matrix.
    /// - 4x4: Uses cofactor expansion along the first row on the 4x4 matrix.
    ///
    /// # Returns
    /// 
    /// A value of type `K` representing the determinant of the matrix.
    ///
    /// # Panics
    ///
    /// Panics if the matrix is not square or if its dimensions exceed 4x4.
    ///
    /// # Example
    /// ```
    /// use matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![
    ///     vec![1.0, 2.0],
    ///     vec![3.0, 4.0]
    /// ]);
    /// assert_eq!(mat.determinant(), -2.0);
    /// ```
    ///
    /// ***
    ///
    /// <details>
    /// <summary><h3>Understanding the Determinant and it's Geometric Meaning</h3></summary>
    /// 
    /// # Understanding Determinants and it's Geometric Meaning
    /// 
    /// The determinant of a square matrix `A`, denoted as `det(A)`, is a scalar value that can tell us a lot about the matrix and its associated linear transformation.
    /// To explain in detail, we need to break it down into two major concepts: **linear transformations** and **geometry**.
    ///
    /// ## Linear Transformations
    /// 
    /// A matrix `A` represents a **linear transformation** in a vector space. A linear transformation takes vectors from one vector space and maps them to another (or possibly the same) vector space, while preserving certain properties like vector addition and scalar multiplication.
    /// 
    /// Consider the matrix `A` acting on a vector `v` in a 2D or 3D space. When you multiply the matrix `A` by the vector `v`, you get a new vector `A * v`, which is the transformed version of `v`.
    /// 
    /// - In 2D space, for example, a linear transformation could rotate, scale, shear, or reflect a vector.
    /// - In 3D space, it could also stretch or compress the space, or rotate and reflect it.
    ///
    /// ## The Determinant and What It Tells Us
    /// 
    /// Now, the **determinant** of the matrix gives us some very important information about this transformation:
    /// 
    /// 1. **Scaling Factor**: The determinant tells us **how much the linear transformation changes the volume** or area (in 2D) of the space. 
    ///    - In 2D, the determinant tells us by how much the area of any shape (like a square or triangle) is scaled when the transformation is applied. If the determinant is 2, for example, the area of any shape is doubled.
    ///    - In 3D, it tells us by how much the volume is scaled.
    /// 
    /// 2. **Invertibility**: The determinant also tells us whether the transformation is **invertible** (i.e., whether you can reverse the transformation). If `det(A) = 0`, the transformation is **not invertible**. This means that the transformation "collapses" the space in some way, making it impossible to reverse the process. The vectors get "squashed" into a lower-dimensional space.
    ///    
    ///    - In 2D, for instance, if the determinant is zero, a shape like a square could be transformed into a line or even a point (a collapse from 2D to 1D or 0D).
    ///    - In 3D, if `det(A) = 0`, a solid object (like a cube) could be collapsed into a plane, a line, or even a point.
    ///
    /// 3. **Orientation**: The sign of the determinant tells us whether the transformation **preserves or reverses orientation**. If the determinant is positive, the transformation preserves orientation. If it’s negative, the transformation reverses orientation (for example, like flipping a shape over).
    ///
    /// ## What Happens When `det(A) = 0`?
    ///
    /// When `det(A) = 0`, we have a very special situation:
    ///
    /// - The linear transformation **collapses the space** into a lower-dimensional subspace. This means that if you apply this transformation to any set of vectors, the result will always lie in a smaller-dimensional subspace.
    ///     - In 2D, for instance, if the determinant is zero, the transformation might map every vector to a line or even a single point. The area of any shape becomes zero because all the points are squeezed onto a lower-dimensional object.
    ///     - In 3D, if `det(A) = 0`, the transformation could collapse a 3D object into a 2D plane, a 1D line, or a point. The volume becomes zero because the space has lost one or more dimensions.
    ///
    /// ## Geometric Explanation of `det(A) = 0` with Examples
    ///
    /// Let's take some simple examples to visualize what happens when the determinant is zero:
    ///
    /// ### 1. **2D Case:**
    /// 
    /// Suppose we have the following matrix `A` that represents a linear transformation in 2D:
    /// 
    /// ```text
    /// A = [ 2  4 ]
    ///     [ 1  2 ]
    /// ```
    /// 
    /// The determinant of `A` is:
    /// 
    /// ```text
    /// det(A) = (2 * 2) - (1 * 4) = 4 - 4 = 0
    /// ```
    ///
    /// This means the transformation represented by `A` collapses the 2D plane into a line. If we apply this matrix to a square, it will become a degenerate shape (a line), and the area of the square will become zero. 
    /// 
    /// **Why is this?** Because the columns of the matrix are linearly dependent (the second column is just a multiple of the first column). So, the transformation doesn't preserve the 2D structure and squashes the space.
    ///
    /// ### 2. **3D Case:**
    /// 
    /// Now consider a 3D example:
    /// 
    /// ```text
    /// A = [ 1  2  3 ]
    ///     [ 0  0  0 ]
    ///     [ 4  5  6 ]
    /// ```
    /// 
    /// The determinant is zero:
    /// 
    /// ```text
    /// det(A) = 1 * det([0 0] [5 6]) - 2 * det([0 0] [4 6]) + 3 * det([0 0] [4 5]) = 0
    /// ```
    ///
    /// The second row of the matrix is all zeros, which means that this matrix represents a transformation that collapses all 3D space onto a plane (since we lose a degree of freedom by squashing one of the dimensions). As a result, the volume becomes zero.
    ///
    /// **Why is this?** The rows (or columns) of the matrix are not linearly independent, meaning the transformation maps the entire 3D space onto a lower-dimensional subspace, in this case, a 2D plane.
    ///
    /// ## Summary
    ///
    /// - The **determinant of a matrix** is a scalar value that represents how much a linear transformation scales the volume or area of objects.
    /// - If `det(A) ≠ 0`, the transformation preserves the space’s dimensionality (it’s invertible).
    /// - If `det(A) = 0`, the transformation collapses the space into a lower dimension, and the transformation is not invertible. This leads to a loss of volume or area, depending on the dimension.
    /// - Geometrically, `det(A) = 0` means the linear transformation reduces the space’s dimensionality (e.g., from 3D to 2D, or from 2D to 1D).
    ///
    /// In conclusion, the determinant is a measure of how the transformation behaves geometrically, especially in terms of scaling, collapsing, and invertibility. A determinant of zero indicates a collapse of space, which makes the transformation non-reversible and non-invertible.
    /// </details>
    pub fn determinant(&self) -> K {
        assert!(self.is_square(), "Matrix must be square to compute determinant.");

        match self.rows() {
            0 => K::zero(),
            1 => self.data[0][0],
            2 => self.determinant2x2(),
            3 => self.determinant3x3(),
            4 => self.determinant4x4(),
            _ => panic!("Matrix dimensions must be between 1 and 4."),
        }
    }


    /// Computes the inverse of a square matrix, if it exists.
    ///
    /// For a given square matrix `A`, the inverse matrix `A^-1` is defined such that:
    ///
    /// <img src="https://github.com/user-attachments/assets/cf619e7a-78a7-4b06-a5e6-22b2b519461e" alt="inverse property A * A^-1 = A^-1 * A = I"/>
    /// 
    /// where `I` is the identity matrix of the same dimension as `A`. The identity matrix `I`
    /// has `1`s on the diagonal and `0`s elsewhere. Only non-singular matrices (matrices with
    /// a non-zero determinant) have an inverse. If the matrix is singular (determinant is zero),
    /// this function returns an error.
    ///
    /// # Formula
    ///
    /// For a matrix `A` of dimension `n x n`, the inverse matrix `A^-1` can be calculated using:
    ///
    /// <img src="https://github.com/user-attachments/assets/439db0d1-fb83-4e91-ad7a-2569b5014cb2" alt="inverse formula A^-1 = (1 / det(A)) * adj(A)"/>
    ///
    /// where:
    /// - `det(A)` is the determinant of `A`.
    /// - `adj(A)` is the adjugate (or adjoint) of `A`.
    ///
    /// ## Steps to Calculate the Adjugate `adj(A)`
    ///
    /// <img src="https://github.com/user-attachments/assets/98eb39bc-b4ab-4595-94d2-9371ebb349aa" alt="adjoint formula"/>
    ///
    /// The adjugate matrix `adj(A)` is obtained by:
    /// 1. **Computing the Matrix of Minors**: Each element `M[i][j]` in the matrix of minors
    ///    is the determinant of the `(n-1)x(n-1)` submatrix that remains after removing the `i`-th row
    ///    and `j`-th column of `A`.
    /// 2. **Applying Cofactor Signs**: For each element `M[i][j]`, we apply the cofactor sign:
    ///
    ///    <img src="https://github.com/user-attachments/assets/fa84bbd3-2749-47e4-abaf-be1f7050d819" alt="cofactor formula C[i][j] = (-1)^(i+j) * M[i][j]"/>
    ///
    ///    This results in the cofactor matrix `C`.
    /// 3. **Taking the Transpose**: The adjugate `adj(A)` is the transpose of the cofactor matrix `C`,
    ///    where each element `adj(A)[j][i]` is assigned the value `C[i][j]`.
    ///
    /// ## Complete Formula for Each Element of the Inverse
    ///
    /// Once the adjugate matrix `adj(A)` is calculated, we obtain the inverse `A^-1` by dividing
    /// each element in `adj(A)` by `det(A)`. Thus:
    /// ```text
    /// A^-1[i][j] = adj(A)[i][j] / det(A)
    /// ```
    ///
    /// # Errors
    ///
    /// - Returns `Err("Matrix is singular and cannot be inverted.")` if the matrix has a zero
    ///   determinant, meaning it has no inverse.
    ///
    /// # Panics
    ///
    /// - Panics if the matrix is not square, as inversion is only defined for square matrices.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![
    ///     vec![4.0, 7.0],
    ///     vec![2.0, 6.0],
    /// ]);
    ///
    /// let inverse = mat.inverse().expect("Matrix is singular and cannot be inverted.");
    ///
    /// assert_eq!(inverse, Matrix::new(vec![
    ///     vec![0.6, -0.7],
    ///     vec![-0.2, 0.4],
    /// ]));
    /// ```
    pub fn inverse(&self) -> Result<Matrix<K>, &'static str> {
        assert!(self.is_square(), "Matrix must be square for inversion.");
        
        let n = self.rows();
        let det = self.determinant();
        if det == K::zero() {
            return Err("Matrix is singular and cannot be inverted.");
        }

        let mut inverse_data = vec![vec![K::zero(); n]; n];

        // Calculate each element of the adjugate matrix, divided by the determinant
        for i in 0..n {
            for j in 0..n {
                // Get the minor matrix by excluding row `i` and column `j`
                let minor = self.minor(i, j);

                let cofactor = minor.determinant() * if (i + j) % 2 == 0 { K::from_f32(1.0) } else { K::from_f32(-1.0) };

                inverse_data[j][i] = cofactor / det.clone();
            }
        }
        Ok(Matrix::new(inverse_data))
    }


    /// Computes the rank of a matrix using row echlon method.
    ///
    /// The rank of a matrix is equal to the number of linearly independent rows in it. Hence, it cannot be more than its number of rows and columns.
    /// The best rank a matrix can have is equal to the number of rows.
    ///
    /// ## Steps to Calculate the rank
    ///
    /// 1. **Compute the reduced row echlon** matrix.
    /// 2. **Count all the linear independent rows** that is the non zero rows.
    /// 3. The number of rows counted is the rank..
    ///
    /// # Panics
    ///
    /// - Panics if the matrix is not rectangular, cannot compute row echlon of non rectangular matrices.
    ///
    /// # Example
    ///
    /// ```
    /// use matrix::Matrix;
    ///
    /// let mat = Matrix::new(vec![
    ///     vec![4.0, 7.0],
    ///     vec![2.0, 6.0],
    ///     vec![2.0, 6.0],
    /// ]);
    ///
    /// assert_eq!(mat.rank(), 2);
    /// ```
    pub fn rank(&self) -> usize {
        let echlon_mat = self.row_echelon();
        let rows = echlon_mat.rows();
        let cols = echlon_mat.cols();
        let mut rank: usize = 0;

        for i in 0..rows {
            if echlon_mat.data[i] != vec![K::zero(); cols] {
                rank += 1;
            }
        }
        rank
    }
}

impl<K: Scalar> PartialEq for Matrix<K>
{
    fn eq(&self, other: &Self) -> bool {
        if self.size() != other.size() {
            return false;
        }
        
        for i in 0..self.rows() {
            for j in 0..self.cols() {
                if other.data[i][j] != self.data[i][j] {
                    return false;
                }
            }
        }
        true
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

    #[test]
    fn test_transpose_2x3_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0_f32, 2.0, 3.0],
            vec![4.0, 5.0, 6.0]
        ]);
        let transposed = mat.transpose();
        assert_eq!(transposed, Matrix::new(vec![
            vec![1.0, 4.0],
            vec![2.0, 5.0],
            vec![3.0, 6.0]
        ]));
    }

    #[test]
    fn test_transpose_square_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0_f32, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
            vec![7.0, 8.0, 9.0]
        ]);
        let transposed = mat.transpose();
        assert_eq!(transposed, Matrix::new(vec![
            vec![1.0, 4.0, 7.0],
            vec![2.0, 5.0, 8.0],
            vec![3.0, 6.0, 9.0]
        ]));
    }

    #[test]
    fn test_transpose_1x4_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0_f32, 2.0, 3.0, 4.0]
        ]);
        let transposed = mat.transpose();
        assert_eq!(transposed, Matrix::new(vec![
            vec![1.0],
            vec![2.0],
            vec![3.0],
            vec![4.0]
        ]));
    }

    #[test]
    fn test_transpose_4x1_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0_f32],
            vec![2.0],
            vec![3.0],
            vec![4.0]
        ]);
        let transposed = mat.transpose();
        assert_eq!(transposed, Matrix::new(vec![
            vec![1.0, 2.0, 3.0, 4.0]
        ]));
    }

    #[test]
    fn test_transpose_empty_matrix() {
        let mat: Matrix<f32> = Matrix::new(vec![]);
        let transposed = mat.transpose();
        assert_eq!(transposed, Matrix::new(vec![]));
    }

    #[test]
    fn test_transpose_integer_matrix() {
        let mat = Matrix::new(vec![
            vec![1, 2, 3],
            vec![4, 5, 6]
        ]);
        let transposed = mat.transpose();
        assert_eq!(transposed, Matrix::new(vec![
            vec![1, 4],
            vec![2, 5],
            vec![3, 6]
        ]));
    }

    #[test]
    fn test_row_echelon_simple_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0, 0.0, 4.0, 2.0],
            vec![1.0, 2.0, 6.0, 2.0],
            vec![2.0, 0.0, 8.0, 8.0],
            vec![2.0, 1.0, 9.0, 4.0],
        ]);
        let expected = Matrix::new(vec![
            vec![1.0, 0.0, 4.0, 0.0],
            vec![0.0, 1.0, 1.0, 0.0],
            vec![0.0, 0.0, 0.0, 1.0],
            vec![0.0, 0.0, 0.0, 0.0],
        ]);
        assert_eq!(mat.row_echelon(), expected);
    }

    #[test]
    fn test_row_echelon_identity_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0, 0.0, 0.0],
            vec![0.0, 1.0, 0.0],
            vec![0.0, 0.0, 1.0],
        ]);
        let expected = mat.clone();
        assert_eq!(mat.row_echelon(), expected);
    }

    #[test]
    fn test_row_echelon_all_zero_matrix() {
        let mat = Matrix::new(vec![
            vec![0.0, 0.0, 0.0],
            vec![0.0, 0.0, 0.0],
            vec![0.0, 0.0, 0.0],
        ]);
        let expected = mat.clone();
        assert_eq!(mat.row_echelon(), expected);
    }

    #[test]
    #[should_panic(expected = "Matrix must be rectangular; all rows must have the same number of columns.")]
    fn test_row_echelon_invalid_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0, 2.0],
            vec![3.0, 4.0, 5.0], // invalid row length
        ]);
        mat.row_echelon();
    }

    #[test]
    fn test_determinant_1x1() {
        let mat = Matrix::new(vec![
            vec![5.0],
        ]);
        assert_eq!(mat.determinant(), 5.0);
    }

    #[test]
    fn test_determinant_2x2() {
        let mat = Matrix::new(vec![
            vec![1.0, 2.0],
            vec![3.0, 4.0],
        ]);
        assert_eq!(mat.determinant(), -2.0);

        let singular_mat = Matrix::new(vec![
            vec![2.0, 4.0],
            vec![1.0, 2.0],
        ]);
        assert_eq!(singular_mat.determinant(), 0.0);
    }

    #[test]
    fn test_determinant_3x3() {
        let identity = Matrix::new(vec![
            vec![1.0, 0.0, 0.0],
            vec![0.0, 1.0, 0.0],
            vec![0.0, 0.0, 1.0],
        ]);
        assert_eq!(identity.determinant(), 1.0);

        let mat = Matrix::new(vec![
            vec![6.0, 1.0, 1.0],
            vec![4.0, -2.0, 5.0],
            vec![2.0, 8.0, 7.0],
        ]);
        assert_eq!(mat.determinant(), -306.0);

        let singular_mat = Matrix::new(vec![
            vec![1.0, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
            vec![7.0, 8.0, 9.0],
        ]);
        assert_eq!(singular_mat.determinant(), 0.0);
    }

    #[test]
    fn test_determinant_4x4() {
        let mat = Matrix::new(vec![
            vec![1.0, 0.0, 4.0, 0.0],
            vec![0.0, 1.0, 1.0, 0.0],
            vec![0.0, 0.0, 0.0, 1.0],
            vec![0.0, 0.0, 0.0, 0.0],
        ]);
        assert_eq!(mat.determinant(), 0.0);

        let identity = Matrix::new(vec![
            vec![1.0, 0.0, 0.0, 0.0],
            vec![0.0, 1.0, 0.0, 0.0],
            vec![0.0, 0.0, 1.0, 0.0],
            vec![0.0, 0.0, 0.0, 1.0],
        ]);
        assert_eq!(identity.determinant(), 1.0);

        let non_singular_mat = Matrix::new(vec![
            vec![3.0, 2.0, 0.0, 1.0],
            vec![4.0, 0.0, 1.0, 2.0],
            vec![3.0, 0.0, 2.0, 1.0],
            vec![9.0, 2.0, 3.0, 1.0],
        ]);
        assert_eq!(non_singular_mat.determinant(), 24.0);
    }

    #[test]
    fn test_determinant_4x4_singular() {
        let singular_mat = Matrix::new(vec![
            vec![1.0, 2.0, 3.0, 4.0],
            vec![2.0, 4.0, 6.0, 8.0],
            vec![3.0, 6.0, 9.0, 12.0],
            vec![4.0, 8.0, 12.0, 16.0],
        ]);
        assert_eq!(singular_mat.determinant(), 0.0);
    }

    #[test]
    #[should_panic(expected = "Matrix must be square to compute determinant.")]
    fn test_determinant_rectangular_3x2_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0, 2.0],
            vec![5.0, 6.0],
            vec![9.0, 10.0],
        ]);
        mat.determinant();
    }

    #[test]
    #[should_panic(expected = "Matrix dimensions must be between 1 and 4.")]
    fn test_determinant_large_square_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0, 0.0, 0.0, 0.0, 0.0],
            vec![0.0, 1.0, 0.0, 0.0, 0.0],
            vec![0.0, 0.0, 1.0, 0.0, 0.0],
            vec![0.0, 0.0, 0.0, 1.0, 0.0],
            vec![0.0, 0.0, 0.0, 0.0, 1.0],
        ]);
        mat.determinant();
    }


    #[test]
    #[should_panic(expected = "Matrix must be square to compute determinant.")]
    fn test_determinant_rectangular_2x3_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
        ]);
        mat.determinant();
    }

    #[test]
    fn test_determinant_empty_matrix() {
        let mat: Matrix<f32> = Matrix::new(vec![]);
        assert_eq!(mat.determinant(), 0.0)
    }

    #[test]
    fn test_inverse_of_2x2_matrix() {
        let mat = Matrix::new(vec![
            vec![4.0, 7.0],
            vec![2.0, 6.0],
        ]);

        let expected_inverse = Matrix::new(vec![
            vec![0.6, -0.7],
            vec![-0.2, 0.4],
        ]);

        let inverse = mat.inverse().expect("Matrix is singular and cannot be inverted.");

        assert_eq!(inverse, expected_inverse);
    }

    #[test]
    fn test_inverse_of_3x3_matrix() {
        let mat = Matrix::new(vec![
            vec![1.0, 2.0, 3.0],
            vec![0.0, 1.0, 4.0],
            vec![5.0, 6.0, 0.0],
        ]);

        let expected_inverse = Matrix::new(vec![
            vec![-24.0, 18.0, 5.0],
            vec![20.0, -15.0, -4.0],
            vec![-5.0, 4.0, 1.0],
        ]);

        let inverse = mat.inverse().expect("Matrix is singular and cannot be inverted.");

        assert_eq!(inverse, expected_inverse);
    }

    #[test]
    fn test_inverse_of_4x4_matrix() {
        let mat = Matrix::new(vec![
            vec![3.0, 2.0, -1.0, 1.0],
            vec![1.0, 0.0, 1.0, 2.0],
            vec![2.0, 1.0, 1.0, -1.0],
            vec![1.0, 1.0, 1.0, 0.0],
        ]);

        let expected_inverse = Matrix::new(vec![
            vec![0.09090909090909090909,  0.27272727272727272727,  0.63636363636363636364,  -0.81818181818181818182],
            vec![0.18181818181818181818,  -0.45454545454545454546, -0.72727272727272727272, 1.3636363636363636363],
            vec![-0.27272727272727272727, 0.18181818181818181819,  0.090909090909090909,    0.45454545454545454546],
            vec![0.09090909090909090909,  0.27272727272727272727,  -0.36363636363636363636, 0.18181818181818181818],
        ]);

        let inverse = mat.inverse().expect("Matrix is singular and cannot be inverted.");

        assert_eq!(inverse, expected_inverse);
    }

    #[test]
    fn test_singular_matrix_inverse() {
        // A matrix with a zero determinant is singular and should return an error
        let singular_mat = Matrix::new(vec![
            vec![1.0, 2.0],
            vec![2.0, 4.0],
        ]);

        let result = singular_mat.inverse();
        assert!(result.is_err(), "Expected an error for singular matrix");
    }

    #[test]
    fn test_identity_matrix_inverse() {
        let identity_matrix = Matrix::new(vec![
            vec![1.0, 0.0, 0.0],
            vec![0.0, 1.0, 0.0],
            vec![0.0, 0.0, 1.0],
        ]);

        let inverse = identity_matrix.inverse().expect("Matrix is singular and cannot be inverted.");
        assert_eq!(inverse, identity_matrix);
    }

    #[test]
    #[should_panic(expected = "Matrix must be square for inversion.")]
    fn test_non_square_matrix_inverse() {
        let non_square_matrix = Matrix::new(vec![
            vec![1.0, 2.0, 3.0],
            vec![4.0, 5.0, 6.0],
        ]);

        non_square_matrix.inverse().unwrap();
    }

    #[test]
    fn test_rank_4x2() {
        let rectangular_mat = Matrix::new(vec![
            vec![1, 2, 3],
            vec![2, 3, 5],
            vec![3, 4, 7],
            vec![4, 5, 9],
        ]);
        assert_eq!(rectangular_mat.rank(), 2);
    }
}
