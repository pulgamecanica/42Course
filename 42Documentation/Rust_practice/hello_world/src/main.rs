fn interproduct(a: i16, b: i16, c: i16) -> i16 {
  return a * b + b * c + c * a;
}

fn fib(n: u32) -> u32 {
  return if n < 2 { n } else { fib(n - 2) + fib(n - 1) };
}

fn print_series(n: u32) -> () {
  println!("Series {n}");
  for i in 0..n {
    print!("[{i}] ")
  }
  println!("");
}

fn print_it() -> () {
  let z : u32 = 13;
  let x : u32 = {
    let y : u32 = 10;
    println!("y: {y}");
    z - y
  };
  println!("z: {z}");
  println!("x: {x}");
  let a = 10;
  println!("before: {a}");
  {
    let a = "hello";
    println!("inner scope: {a}");

    let a = true;
    println!("shadowed in inner scope: {a}");
  }

  println!("after: {a}");
}

fn gcd(a: u32, b: u32) -> u32 {
  if b > 0 {
    return gcd(b, a % b);
  } else {
    return a;
  }
}

fn factorial(n: u32) -> u32 {
  let mut product = 1;
  for i in 1..=n {
    product *= dbg!(i);
  }
  product
}


fn print_array() -> () {
  let mut a: [i8; 10] = [42; 10];
  a[5] = 0;
  println!("a: {a:?}");
}

fn transpose(matrix: [[i32; 3]; 3]) -> [[i32; 3]; 3] {
  let mut result = [[0; 3]; 3];
  for i in 0..3 {
    for j in 0..3 {
      result[j][i] = matrix[i][j];
    }
  }
  result
}

#[test]
fn test_transpose() {
  let matrix = [
    [101, 102, 103], //
    [201, 202, 203],
    [301, 302, 303],
  ];
  let transposed = transpose(matrix);
  assert_eq!(
    transposed,
    [
      [101, 201, 301], //
      [102, 202, 302],
      [103, 203, 303],
    ]
  );
}

mod collatz;
mod array_utils;
mod elevator;
mod expression;
mod logger;
mod counter;
mod rot13;
mod builder;

use collatz::collatz_length;
use array_utils::magnitude;
use array_utils::normalize;
use elevator::print_elevator_events;
use logger::print_logger;
use counter::Counter;
use std::cmp::Ordering;
use builder::test_builder;

// fn min<T: Ord>(left: T, right: T) -> T {
//   return left.min(right);
// }

fn min<T: Ord>(left: T, right: T) -> T {
    match left.cmp(&right) {
      Ordering::Greater => right,
      _ => left,
    }
}

#[test]
fn test_min() {
  assert_eq!(min(0, 10), 0);
  assert_eq!(min(500, 123), 123);

  assert_eq!(min('a', 'z'), 'a');
  assert_eq!(min('7', '1'), '1');

  assert_eq!(min("hello", "goodbye"), "goodbye");
  assert_eq!(min("bat", "armadillo"), "armadillo");
}


fn main() {
  let mut num: u16 = 10 ;
  println!("Hello {num} 🌍!");
  num = 9;
  println!("interproduct {}", interproduct(num.try_into().unwrap(), 2, 16));
  println!("fib({num}) = {}", fib(num.try_into().unwrap()));
  print_series(num.try_into().unwrap());
  print_it();
  println!("CGD ({num}, 42): {}", gcd(num.try_into().unwrap(), 42));
  println!("{num}! = {}", factorial(num.try_into().unwrap()));
  println!("Length: {}", collatz_length(11));
  print_array();

  let matrix = [
        [101, 102, 103], // <-- the comment makes rustfmt add a newline
        [201, 202, 203],
        [301, 302, 303],
    ];

    println!("matrix: {:#?}", matrix);
    let transposed = transpose(matrix);
    println!("transposed: {:#?}", transposed);
    let mut point = (1, 2);
    let x_coord = &mut point.0;
    *x_coord = 20;
    println!("point: {point:?}");

    println!("Magnitude of a unit vector: {}", magnitude(&[0.0, 1.0, 0.0]));

    let mut v = [1.0, 2.0, 9.0];
    println!("Magnitude of {v:?}: {}", magnitude(&v));
    normalize(&mut v);
    println!("Magnitude of {v:#?} after normalization: {}", magnitude(&v));
    // Test elevator events
    print_elevator_events();
    // Test logger
    print_logger();
    // Use min
    min(1, 2);
    // Counter testing
    let mut ctr = Counter::new();
    ctr.count(13);
    ctr.count(14);
    ctr.count(16);
    ctr.count(14);
    ctr.count(14);
    ctr.count(11);

    for i in 10..20 {
        println!("saw {} values equal to {}", ctr.times_seen(i), i);
    }

    let mut strctr = Counter::new();
    strctr.count("apple");
    strctr.count("orange");
    strctr.count("apple");
    println!("got {} apples", strctr.times_seen("apple"));
    // Test builder
    test_builder();
}
