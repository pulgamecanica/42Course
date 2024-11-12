use std::ops::{AddAssign, SubAssign, MulAssign};

/// Compute the linear interpolation (lerp), which takes two objects of type V and a scalar t of type f32.
/// V should be an object implementing the `Clone`, `MulAssign<f32>`, `AddAssign` & `SubAssign` traits
///
/// Formula: `lerp(u,v,t) = u+t × (v−u)`
///
/// Prototype FMA: We: `A + B * (C1-C2)` <=> `A + B * C`
///
/// `fma` does: `a * b + c`, or in this case =>
///
/// Option1: `fma(B, C, A)`<br>
/// Option2: `fma(C, B, A)`
pub fn lerp<V>(u: V, v: V, t: f32) -> V
where
    V: Clone + AddAssign + SubAssign + MulAssign<f32>,
{
    let mut result = u.clone();
    let mut diff = v.clone();
    diff -= u;
    diff *= t;
    result += diff;
    result
}


// Unit Tests


#[cfg(test)]
mod tests {
    use crate::Vector;
    use crate::Matrix;
    use super::*;

    #[test]
    fn test_lerp_f32() {
        let num1: f32 = 42.0;
        let num2: f32 = 82.0;
        let t = 0.0;
        let result = lerp(num1, num2, t);
        assert_eq!(result, 42.0);
        assert_eq!(lerp(num1, num2, 1.0), 82.0);
    }

    #[test]
    fn test_matrix_lerp_f32() {
        let mat1 = Matrix::new(vec![vec![1.0_f32, 2.0], vec![3.0, 4.0]]);
        let mat2 = Matrix::new(vec![vec![5.0_f32, 6.0], vec![7.0, 8.0]]);
        let t = 0.5;
        let result = lerp(mat1, mat2, t);
        assert_eq!(result.data, vec![vec![3.0, 4.0], vec![5.0, 6.0]]);
    }

    #[test]
    fn test_vector_lerp_f32() {
        let vec1: Vector<f32> = Vector::new(vec![1.0, 2.0, 3.0]);
        let vec2: Vector<f32> = Vector::new(vec![3.0, 5.0, 6.0]);
        let t = 0.2;
        let result = lerp(vec1, vec2, t);
        assert_eq!(result.data, vec![1.4, 2.6, 3.6]);  // Expected rounded values
    }
}
