use matrix::{Matrix, Vector, Complex};
use matrix::linear_interpolation::lerp;
use matrix::projection_matrix::projection;

// Example from the subject
fn try_linear_combination() {
    println!("** Test Linear Combination **");
    let e1 = Vector::from([1., 0., 0.]);
    let e2 = Vector::from([0., 1., 0.]);
    let e3 = Vector::from([0., 0., 1.]);
    
    let v1 = Vector::from([1., 2., 3.]);
    let v2 = Vector::from([0., 10., -100.]);

    println!("{}", Vector::linear_combination(&[e1, e2, e3], &[10., -2., 0.5]));
    // [10.]
    // [-2.]
    // [0.5]
    println!("{}", Vector::linear_combination(&[v1, v2], &[10., -2.]));
    // [10.]
    // [0.]
    // [230.]
}

fn try_cos() {
    println!("Cosine");
    let u = Vector::from([1., 0.]);
    let v = Vector::from([1., 0.]);
    println!("{}", Vector::angle_cos(&u, &v));
    // 1.0
    let u = Vector::from([1., 0.]);
    let v = Vector::from([0., 1.]);
    println!("{}", Vector::angle_cos(&u, &v));
    // 0.0
    let u = Vector::from([-1., 1.]);
    let v = Vector::from([ 1., -1.]);
    println!("{}", Vector::angle_cos(&u, &v));
    // -1.0
    let u = Vector::from([2., 1.]);
    let v = Vector::from([4., 2.]);
    println!("{}", Vector::angle_cos(&u, &v));
    // 1.0
    let u = Vector::from([1., 2., 3.]);
    let v = Vector::from([4., 5., 6.]);
    println!("{}", Vector::angle_cos(&u, &v));
    // 0.974631846
}

fn try_dot_product() {
    let e1 = Vector::from([1., 0., 0.]);
    let e2 = Vector::from([0., 1., 0.]);
    let e3 = Vector::from([2., 1., 3.]);
    let e4 = Vector::from([1., 1., 1.]);

    println!("Should be zero: [{}]", e1.dot(&e2));
    println!("Should be two: [{}]", e1.dot(&e3));
    println!("Should be six: [{}]", e3.dot(&e4));
}

fn try_norms() {
    let e1 = Vector::from([33.5, 22.3, 9.9]);
    let e2 = Vector::from([42.0]);
    let e3 = Vector::from([22.2, 10.1, 3.0]);

    println!("Norm1: {} = {}", e1, e1.norm_1());
    println!("Norm: {} = {}", e2, e3.norm());
    println!("Norm inf: {} = {}", e3, e3.norm_inf());
}

fn try_linear_interpolation(factor: f32) {
    println!("Linear Interpolation");
    let mat1 = Matrix::from([
        [0.0, 12.5, 25.0],
        [37.5, 50.0, 62.5],
        [75.0, 87.5, 100.0],
    ]);

    let mat2 = Matrix::from([
        [5.0, 20.0, 45.0],
        [30.0, 60.0, 80.0],
        [25.0, 70.0, 95.0],
    ]);
    println!("{}", "*".repeat(42));
    println!("Having the following matrixes:\n{mat1}\n{mat2}\nThe linear interpolation for {factor} results in:\n");
    println!("{}", lerp(mat1, mat2, factor));
}

fn try_add_sub_scale() {
    println!("** Test Add Sub Scale **");
    let mut vec1 = Vector::new(vec![42.0, -42.0, 19.0]);
    let vec2 = Vector::from([
        1.0,
        2.0,
        3.0
    ]);
    let mut mat1 = Matrix::from([
        [0.0, 2.0, 42.0],
        [-3.0, 4.0, 0.0],
        [-10.0, -24.0, 44.0],
    ]);

    let mat2 = Matrix::from([
        [1.0, 2.0, 4.0],
        [3.0, 4.0, 5.0],
        [3.0, 4.0, 5.0],
    ]);

    println!("Vec1: {vec1}");
    println!("Vec2: {vec2}");

    vec1.sub(&vec2);
    println!("(- {})\t{}", vec2, vec1);
    vec1.scl(-2.0);
    println!("(* {})\t{}", -2, vec1);
    vec1.add(&vec2);
    println!("(+ {})\t{}", vec2, vec1);

    println!("\nMat1:\n{mat1}\nMat2:\n{mat2}");
    mat1.sub(&mat2);
    println!("(-)\n{}\n{}", mat2, mat1);
    mat1.scl(-2.0);
    println!("(* {})\n{}", -2, mat1);
    mat1.add(&mat2);
    println!("(+)\n{}\n{}", mat2, mat1);
}

fn try_cross_prod() {
    println!("Cross Product Tests");
    let u = Vector::from([0., 0., 1.]);
    let v = Vector::from([1., 0., 0.]);
    println!("{}", Vector::cross_product(&u, &v));
    // [0.]
    // [1.]
    // [0.]
    let u = Vector::from([1., 2., 3.]);
    let v = Vector::from([4., 5., 6.]);
    println!("{}", Vector::cross_product(&u, &v));
    // [-3.]
    // [6.]
    // [-3.]
    let u = Vector::from([4., 2., -3.]);
    let v = Vector::from([-2., -5., 16.]);
    println!("{}", Vector::cross_product(&u, &v));
    // [17.]
    // [-58.]
    // [-16.]
}

fn try_mat_vec_mul() {
    println!("Matrix x Vector");
    {
        let u = Matrix::new(vec![
            vec![1., 0.],
            vec![0., 1.],
        ]);
        let v = Vector::new(vec![4., 2.]);
        println!("{}", u.mul_vec(&v));
        // [4.]
        // [2.]
    }
    {
        let u = Matrix::from([
            [2., 0.],
            [0., 2.],
        ]);
        let v = Vector::from([4., 2.]);
        println!("{}", u.mul_vec(&v));
        // [8.]
        // [4.]
    }
    {
        let u = Matrix::from([
            [2., -2.],
            [-2., 2.],
        ]);
        let v = Vector::from([4., 2.]);
        println!("{}", u.mul_vec(&v));
        // [4.]
        // [-4.]
    }
}

fn try_mat_mat_mul() {
    println!("Matrix x Matrix");
    {
        let u = Matrix::from([
            [1., 0.],
            [0., 1.],
        ]);
        let v = Matrix::from([
            [1., 0.],
            [0., 1.],
        ]);
        println!("{}", u.mul_mat(&v));
        // [1., 0.]
        // [0., 1.]
    }
    {
        let u = Matrix::from([
            [1., 0.],
            [0., 1.],
        ]);
        let v = Matrix::from([
            [2., 1.],
            [4., 2.],
        ]);
        println!("{}", u.mul_mat(&v));
        // [2., 1.]
        // [4., 2.]
    }
    {
        let u = Matrix::from([
            [3., -5.],
            [6., 8.],
        ]);
        let v = Matrix::from([
            [2., 1.],
            [4., 2.],
        ]);
        println!("{}", u.mul_mat(&v));
        // [-14., -7.]
        // [44., 22.]
    }
}

fn try_trace() {
    println!("Matrix Trace");
    {
        let u = Matrix::from([
            [1., 0.],
            [0., 1.],
        ]);
        println!("{}", u.trace());
        // 2.0
    }
    {
        let u = Matrix::from([
            [2., -5., 0.],
            [4., 3., 7.],
            [-2., 3., 4.],
        ]);
        println!("{}", u.trace());
        // 9.0
    }
    {
        let u = Matrix::from([
            [-2., -8., 4.],
            [1., -23., 4.],
            [0., 6., 4.],
        ]);
        println!("{}", u.trace());
        // -21.0
    }
}

fn try_row_echelon() {
    println!("Row Echelon");
    let u = Matrix::new(vec![
        vec![1., 0., 0.],
        vec![0., 1., 0.],
        vec![0., 0., 1.],
    ]);
    println!("{}", u.row_echelon());
    // [1.0, 0.0, 0.0]
    // [0.0, 1.0, 0.0]
    // [0.0, 0.0, 1.0]
    let u = Matrix::new(vec![
        vec![1., 2.],
        vec![3., 4.],
    ]);
    println!("{}", u.row_echelon());
    // [1.0, 0.0]
    // [0.0, 1.0]
    let u = Matrix::new(vec![
        vec![1., 2.],
        vec![2., 4.],
    ]);
    println!("{}", u.row_echelon());
    // [1.0, 2.0]
    // [0.0, 0.0]
    let u = Matrix::new(vec![
    vec![8., 5., -2., 4., 28.],
    vec![4., 2.5, 20., 4., -4.],
    vec![8., 5., 1., 4., 17.],
    ]);
    println!("{}", u.row_echelon());
    // [1.0, 0.625, 0.0, 0.0, -12.1666667]
    // [0.0, 0.0, 1.0, 0.0, -3.6666667]
    // [0.0, 0.0, 0.0, 1.0, 29.5 ]
}

fn try_determinant() {
    println!("Determinant");
    let u = Matrix::from([
        [ 1., -1.],
        [-1., 1.],
    ]);
    println!("{}", u.determinant());
    // 0.0
    let u = Matrix::from([
        [2., 0., 0.],
        [0., 2., 0.],
        [0., 0., 2.],
    ]);
    println!("{}", u.determinant());
    // 8.0
    let u = Matrix::from([
        [8., 5., -2.],
        [4., 7., 20.],
        [7., 6., 1.],
    ]);
    println!("{}", u.determinant());
    // -174.0
    let u = Matrix::from([
        [ 8., 5., -2., 4.],
        [ 4., 2.5, 20., 4.],
        [ 8., 5., 1., 4.],
        [28., -4., 17., 1.],
    ]);
    println!("{}", u.determinant());
    // 1032
}

fn try_inverse() {
    println!("Inverse");
    let u = Matrix::from([
        [1., 0., 0.],
        [0., 1., 0.],
        [0., 0., 1.],
    ]);
    match u.inverse() {
        Ok(inverse_mat) => {
            println!("{}", inverse_mat);
            // [1.0, 0.0, 0.0]
            // [0.0, 1.0, 0.0]
            // [0.0, 0.0, 1.0]
        }
        Err(e) => {
            println!("{e}");
        }

    }
    let u = Matrix::from([
        [2., 0., 0.],
        [0., 2., 0.],
        [0., 0., 2.],
    ]);
    match u.inverse() {
        Ok(inverse_mat) => {
            println!("{}", inverse_mat);
            // [0.5, 0.0, 0.0]
            // [0.0, 0.5, 0.0]
            // [0.0, 0.0, 0.5]
        }
        Err(e) => {
            println!("{e}");
        }

    }
    let u = Matrix::from([
        [8., 5., -2.],
        [4., 7., 20.],
        [7., 6., 1.],
    ]);
    match u.inverse() {
        Ok(inverse_mat) => {
            println!("{}", inverse_mat);
            // [0.649425287, 0.097701149, -0.655172414]
            // [-0.781609195, -0.126436782, 0.965517241]
            // [0.143678161, 0.074712644, -0.206896552]
        }
        Err(e) => {
            println!("{e}");
        }

    }
}

fn try_rank() {
    println!("rank");
    let u = Matrix::from([
        [1., 0., 0.],
        [0., 1., 0.],
        [0., 0., 1.],
    ]);
    println!("{}", u.rank());
    // 3
    let u = Matrix::from([
        [ 1., 2., 0., 0.],
        [ 2., 4., 0., 0.],
        [-1., 2., 1., 1.],
    ]);
    println!("{}", u.rank());
    // 2
    let u = Matrix::from([
        [ 8., 5., -2.],
        [ 4., 7., 20.],
        [ 7., 6., 1.],
        [21., 18., 7.],
    ]);
    println!("{}", u.rank());
    // 3
}


fn try_projection_matrix() {
    println!("Projection Matrix");
    let fov = 1.0; // Example FOV in radians (~60 degrees)
    let ratio = 16.0 / 9.0; // Widescreen aspect ratio
    let near = 1.0; // Near clipping plane
    let far = 100.0; // Far clipping plane

    let projection_matrix: Matrix<f32> = projection(fov, ratio, near, far);

    println!("Projection Matrix: {}", projection_matrix);
}

fn try_complex() {
    println!("Complex");
    let c1: Complex = Complex::new(1.0, 2.0);
    let c2: Complex = Complex::new(0.0, 0.0);
    let c3: Complex = Complex::new(3.0, -2.0);
    let c4: Complex = Complex::new(-1.0, -1.0);

    let mut v1: Vector<Complex> = Vector::new(vec![c1, c2]);
    let v2: Vector<Complex> = Vector::new(vec![c3, c4]);

    println!("V1:{} & V2:{}", v1, v2);
    v1.add(&v2);
    println!("V1 + V2: {}", v1);
}

fn main() {
    try_add_sub_scale();
    try_linear_combination();
    try_linear_interpolation(0.0);
    try_linear_interpolation(0.5);
    try_linear_interpolation(2.0);
    try_dot_product();
    try_norms();
    try_cos();
    try_cross_prod();
    try_mat_vec_mul();
    try_mat_mat_mul();
    try_trace();
    try_row_echelon();
    try_determinant();
    try_inverse();
    try_rank();
    try_projection_matrix();
    try_complex();
}
