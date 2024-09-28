// Calculate the magnitude of a vector by summing the squares of its coordinates
// and taking the square root. Use the `sqrt()` method to calculate the square
// root, like `v.sqrt()`.


pub fn magnitude(array : &[f64; 3]) -> f64 {
  let mut res : f64 = 0.0;
  for item in array {
    res += item * item
  }
  res
}

// Normalize a vector by calculating its magnitude and dividing all of its
// coordinates by that magnitude.


pub fn normalize(array : &mut [f64; 3]) {
  let mag : f64 = magnitude(array);
  for item in array {
    *item /= mag;
  }
}
