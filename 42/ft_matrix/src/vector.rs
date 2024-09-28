use crate::Matrix;

/// A struct representing a mathematical vector.
/// 
/// The `Vector` struct is generic over type `K` but is currently implemented
/// for `f32`, which represents a vector of 32-bit floating-point numbers.
pub struct Vector<K> {
    // The underlying data of the vector stored as a `Vec<K>`.
    pub data: Vec<K>,
}

impl Vector<f32> {
    /// Creates a new `Vector<f32>` from a vector of `f32` values.
    ///
    /// # Arguments
    ///
    /// * `data` - A `Vec<f32>` representing the elements of the vector.
    ///
    /// # Example
    ///
    /// ```
    /// use ft_matrix::Vector;
    ///
    /// let vec = Vector::new(vec![1.0, 2.0, 3.0]);
    /// ```
    pub fn new(data: Vec<f32>) -> Self {
        Self { data }
    }

    /// Returns the size (i.e., the number of elements) of the vector.
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

    /// Prints the contents of the vector to the standard output.
    ///
    /// The vector will be printed in a single line format, like:
    /// 
    /// ```text
    /// [1.0, 2.0, 3.0]
    /// ```
    ///
    /// # Example
    /// use ft_matrix::Vector;
    ///
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

    /// Reshapes the vector into a matrix with the specified number of rows and columns.
    ///
    /// # Arguments
    ///
    /// * `rows` - The number of rows in the resulting matrix.
    /// * `cols` - The number of columns in the resulting matrix.
    ///
    /// # Panics
    ///
    /// This function will panic if the size of the vector does not match the
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
    pub fn reshape(&self, rows: usize, cols: usize) -> Matrix<f32> {
        assert_eq!(self.data.len(), rows * cols, "Vector size does not match the dimensions of the matrix.");
        
        let data = self.data
            .chunks(cols)
            .map(|chunk| chunk.to_vec())
            .collect();

        Matrix { data }
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
    fn test_vector_print() {
        let vec = Vector::new(vec![1.0, 2.0, 3.0]);
        vec.print();
    }

    #[test]
    fn test_vector_reshape() {
        let vec = Vector::new(vec![1.0, 2.0, 3.0, 4.0]);
        let mat = vec.reshape(2, 2);
        assert_eq!(mat.size(), (2, 2));
    }
}
