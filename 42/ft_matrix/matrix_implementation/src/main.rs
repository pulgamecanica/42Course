use ft_matrix::{Matrix, Vector};


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

fn main() {
    try_add_sub_scale();
    try_linear_combination();
}
