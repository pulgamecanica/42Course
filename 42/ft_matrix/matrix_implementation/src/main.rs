use ft_matrix::{Matrix, Vector};
use ft_matrix::linear_interpolation::lerp;

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
        0.0, 12.5, 25.0,
        37.5, 50.0, 62.5,
        75.0, 87.5, 100.0,
    ]);

    let mat2 = Matrix::from([
        5.0, 20.0, 45.0,
        30.0, 60.0, 80.0,
        25.0, 70.0, 95.0,
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
        0.0, 2.0, 42.0,
        -3.0, 4.0, 0.0,
        -10.0, -24.0, 44.0,
    ]);

    let mat2 = Matrix::from([
        1.0, 2.0, 4.0,
        3.0, 4.0, 5.0,
        3.0, 4.0, 5.0,
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
            2., 0.,
            0., 2.,
        ]);
        let v = Vector::from([4., 2.]);
        println!("{}", u.mul_vec(&v));
        // [8.]
        // [4.]
    }
    {
        let u = Matrix::from([
            2., -2.,
            -2., 2.,
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
        1., 0.,
        0., 1.,
        ]);
        let v = Matrix::from([
        1., 0.,
        0., 1.,
        ]);
        println!("{}", u.mul_mat(&v));
        // [1., 0.]
        // [0., 1.]
    }
    {
        let u = Matrix::from([
            1., 0.,
            0., 1.,
        ]);
        let v = Matrix::from([
            2., 1.,
            4., 2.,
        ]);
        println!("{}", u.mul_mat(&v));
        // [2., 1.]
        // [4., 2.]
    }
    {
        let u = Matrix::from([
            3., -5.,
            6., 8.,
        ]);
        let v = Matrix::from([
            2., 1.,
            4., 2.,
        ]);
        println!("{}", u.mul_mat(&v));
        // [-14., -7.]
        // [44., 22.]
    }
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
}
