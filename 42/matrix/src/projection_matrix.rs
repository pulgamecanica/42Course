use crate::matrix::Matrix;

/// Generates a 4x4 perspective projection matrix for a 3D rendering pipeline with a symmetric frustum.
///
/// In 3D graphics, a perspective projection matrix transforms 3D points in camera space to 2D points
/// on the screen, applying a perspective effect where objects farther from the camera appear smaller.
///
/// The symmetric frustum is defined by a field of view (`fov`), aspect ratio (`ratio`), and near and far
/// clipping planes. These parameters determine the frustum bounds and scale.
///
/// # Parameters
///
/// - `fov`: Field of View (in radians).
///     - Represents the vertical angle of the camera's viewing cone. A larger FOV results in a wider view of the scene.
///     - Should be given in **radians** (for example, 1.57 radians is approximately 90 degrees).
///     - The tangent of half the `fov` is used to determine the extent of scaling for the projection.
///
/// - `ratio`: Aspect ratio of the view window, calculated as `width / height`.
///     - Adjusts the horizontal stretch of the view window.
///     - Common values include 4:3 (1.33), 16:9 (1.78), etc.
///     - This value adjusts the x-axis scale to match the frustum's width and prevent distortion.
///
/// - `near`: Near clipping plane distance.
///     - Represents the closest visible depth from the camera.
///     - Any points closer than this distance are clipped and not rendered.
///     - Must be a positive, non-zero value.
///
/// - `far`: Far clipping plane distance.
///     - Represents the farthest visible depth in the scene.
///     - Any points beyond this distance are clipped and not rendered.
///     - Must be greater than `near` to ensure a valid depth range.
///
/// # Projection Matrix Structure
///
///
/// <img src="https://github.com/user-attachments/assets/51d0356c-6e45-47aa-8f71-f8189b03eb01" alt="image projection"/>
///
/// <a href="https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix.html">See formula here</a>
///
/// The perspective projection matrix generated has the form:
///
/// <img src="https://github.com/user-attachments/assets/1d36e3bc-616b-49f7-94d4-c7f807481ead" alt="projection matrix formula"/>
///
/// Where:
///
/// <img src="https://github.com/user-attachments/assets/065f759e-e2f7-4e01-861d-01ab0e31e123" alt="attributes formula convert"/>
///
/// Also:
///
/// - `l = -r`,
/// - `r = t * ratio`,
/// - `t = near * tan(fov / 2)`,
/// - `b = -t`
/// - `n` is the near clipping plane distance.
/// - `f` is the far clipping plane distance.
///
/// ## Breakdown of Matrix Elements
/// 1. **Perspective Scaling**:
///     - The terms `2n / (r - l)` and `2n / (t - b)` scale the x and y coordinates, respectively, to create the perspective effect.
///     - These scaling factors depend on the field of view and the aspect ratio, defining how the frustum expands and how the depth is represented on screen.
///     
/// 2. **Centering**:
///     - The terms `(r + l) / (r - l)` and `(t + b) / (t - b)` shift the coordinates to the center of the frustum.
///     - These terms help offset the frustum to ensure it's aligned with the camera's view, particularly for the x and y axes.
///
/// 3. **Depth Mapping**:
///     - The terms `-(f + n) / (f - n)` and `-2fn / (f - n)` map the `near` and `far` planes to a normalized depth range of `[-1, 1]`.
///     - This allows the depth values to be represented in a range that the graphics pipeline can handle efficiently.
///
/// 4. **Homogeneous Coordinate Adjustment**:
///     - The final row `[0, 0, -1, 0]` adjusts the homogeneous coordinates for proper depth division and screen space transformation.
///
/// # Example
///
/// ```rust
/// use matrix::Matrix;
/// use matrix::projection_matrix::projection;
/// 
/// let fov = 1.57; // Field of view in radians (~90 degrees)
/// let ratio = 16.0 / 9.0; // Aspect ratio of 16:9
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
pub fn projection(fov: f32, ratio: f32, near: f32, far: f32) -> Matrix<f32> {
    if near <= 0.0 || far <= near {
        panic!("Invalid near or far plane distances: near must be positive, and far must be greater than near.");
    }

    // Compute the frustum bounds
    let t = near * (fov / 2.0).tan();
    let b = -t;
    let r = t * ratio;
    let l = -r;

    // Compute matrix elements
    let a = 2.0 * near / (r - l);
    let b = 2.0 * near / (t - b);
    let c = (r + l) / (r - l);
    let d = (t + b) / (t - b);
    let e = -(far + near) / (far - near);
    let f = -(2.0 * far * near) / (far - near);

    Matrix {
        data: vec![
            vec![a, 0.0, c, 0.0],
            vec![0.0, b, d, 0.0],
            vec![0.0, 0.0, e, f],
            vec![0.0, 0.0, -1.0, 0.0],
        ],
    }
}
