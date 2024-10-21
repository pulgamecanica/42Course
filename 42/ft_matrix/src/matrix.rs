use crate::scalar::Scalar;
use crate::Vector;

/// A struct representing a mathematical matrix.
/// 
/// The `Matrix` struct is generic over type `K` but is currently implemented
/// for `K`, which represents a matrix of 32-bit Scalar numbers (such as f32).
#[derive(Debug)]
pub struct Matrix<K: Scalar> {
    // The underlying data of the matrix stored as a `Vec<Vec<K>>`.
    pub data: Vec<Vec<K>>,
}

impl<K: Scalar + std::fmt::Debug> Matrix<K> {
    /// Creates a new `Matrix<K>` from a vector of vectors (rows of the matrix).
    ///
    /// # Arguments
    ///
    /// * `data` - A 2D vector of `K` values representing the rows of the matrix.
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

    /// Reshapes the matrix into a vector by flattening its rows.
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
}


