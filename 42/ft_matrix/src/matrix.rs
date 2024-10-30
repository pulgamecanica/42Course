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
}
