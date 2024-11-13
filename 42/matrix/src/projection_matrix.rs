use crate::matrix::Matrix;

/// Generates a 4x4 perspective projection matrix for a 3D rendering pipeline.
///
/// In 3D graphics, a projection matrix transforms 3D points in camera space to 2D points on the screen.
/// This transformation applies a perspective effect, making objects farther from the camera appear smaller.
/// This matrix is a core part of rendering, mapping a 3D scene into a 2D representation.
///
/// # Parameters
///
/// - `fov`: Field of View (in radians).
///     - Represents the vertical angle of the camera’s viewing cone. A larger FOV results in a wider view of the scene.
///     - It should be given in **radians** (for example, 1.57 radians is approximately 90 degrees).
///     - The half-angle tangent is used to determine the extent of scaling for the perspective effect.
///
/// - `ratio`: Aspect ratio of the view window, calculated as `width / height`.
///     - Determines the horizontal stretch of the view window.
///     - Common values include 4:3 (1.33), 16:9 (1.78), etc.
///     - This value adjusts the x-axis scale to prevent distortion caused by non-square aspect ratios.
///
/// - `near`: Near clipping plane distance.
///     - This represents the distance to the nearest visible depth from the camera.
///     - Any points closer than this distance are clipped and not rendered.
///     - Must be a positive, non-zero value.
///
/// - `far`: Far clipping plane distance.
///     - Represents the farthest visible distance in the scene.
///     - Any points beyond this distance are clipped and not rendered.
///     - Should be larger than `near` to ensure a positive depth range.
///
/// # Projection Matrix Structure
///
/// The perspective projection matrix generated has the form:
///
/// <a href="https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix.html">See formula here</a>
///
/// ```text
/// P = 
/// [
///     a    0    0    0
///     0    b    0    0
///     0    0    c    d
///     0    0   -1    0
/// ]
/// ```
///
/// Where:
///
/// - `a = 1 / (tan(fov / 2) * ratio)`
/// - `b = 1 / tan(fov / 2)`
/// - `c = -(far + near) / (far - near)`
/// - `d = -(2 * far * near) / (far - near)`
///
/// This matrix serves the following roles:
///
/// 1. **Perspective Scaling**: The values `a` and `b` scale the x and y coordinates, respectively, to create the perspective effect.
///    - `a` scales by the reciprocal of the half-angle tangent, adjusted by the aspect ratio.
///    - `b` scales by the reciprocal of the half-angle tangent for the y-axis.
///
/// 2. **Depth Mapping**: The values `c` and `d` map the `near` and `far` planes to a normalized depth range.
///    - `c` and `d` control the depth scaling, mapping the range `[near, far]` to `[-1, 1]` in normalized device coordinates.
///
/// 3. **Homogeneous Coordinate Adjustment**: The matrix’s fourth row, `[0, 0, -1, 0]`, adjusts the homogeneous coordinates
///    to handle the perspective division, which converts the 3D coordinates to 2D screen space.
///
/// # Example
///
/// ```rust
/// use matrix::Matrix;
/// use matrix::projection_matrix::projection;
/// 
/// let fov = 1.57; // Field of view in radians (~90 degrees)
/// let ratio = 16.0 / 9.0; // Aspect ratio of 16:9 for widescreen
/// let near = 0.1; // Near clipping plane at 0.1 units
/// let far = 100.0; // Far clipping plane at 100 units
///
/// let projection_matrix: Matrix<f32> = projection(fov, ratio, near, far);
///
/// // Resulting matrix can be used in rendering pipelines
/// println!("{}", projection_matrix); // Displays the matrix
/// ```
///
/// # Returns
///
/// Returns a new `4x4` matrix in `Matrix<f32>` representing the perspective projection.
/// This matrix can be used in 3D rendering to transform points from camera space to screen space.
///
/// # Panics
///
/// Panics if `near` is zero or negative, as the near plane must be positive and non-zero.
/// A `near` value of zero would lead to division by zero in the depth mapping.
pub fn projection(fov: f32, ratio: f32, near: f32, far: f32) -> Matrix::<f32> {
   let f = 1.0 / (fov / 2.0).tan();
        
    let a = f / ratio;
    let b = f;
    let c = (far + near) / (near - far);
    let d = (2.0 * far * near) / (near - far);

    Matrix {
        data: vec![
            vec![a, 0.0, 0.0, 0.0],
            vec![0.0, b, 0.0, 0.0],
            vec![0.0, 0.0, c, d],
            vec![0.0, 0.0, -1.0, 0.0],
        ],
    }
}