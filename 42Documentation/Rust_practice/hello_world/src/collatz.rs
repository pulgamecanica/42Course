/// Determine the length of the collatz sequence beginning at `n`.


pub fn collatz_length(n: i32) -> u32 {
  if n == 1 {
    1
  } else {
    collatz_length( if n % 2 == 0 {n / 2} else { (3 * n) + 1 } ) + 1
  }
}

#[test]
fn test_collatz_length() {
    assert_eq!(collatz_length(11), 15);
}