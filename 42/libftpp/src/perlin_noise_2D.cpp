/**
 * @file perlin_noise_2D.cpp
 * @brief Implementation of the `PerlinNoise2D` class.
 */

#include "random_2D_coordinate_generator.hpp"
#include "perlin_noise_2D.hpp"
#include <random>
#include <algorithm>

// https://cs.nyu.edu/~perlin/noise/
PerlinNoise2D::PerlinNoise2D(int seed) {
  Random2DCoordinateGenerator random_gen(seed);

  permutation_.resize(256);
  std::iota(permutation_.begin(), permutation_.end(), 0);

  auto random_wrapper = [&random_gen](int max) {
    return random_gen(0, max - 1) % max;
  };

  std::shuffle(permutation_.begin(), permutation_.end(), 
    std::default_random_engine([&]() { return random_wrapper(256); }())
  );
  permutation_.insert(permutation_.end(), permutation_.begin(), permutation_.end());
}

float PerlinNoise2D::fade(float t) const {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise2D::lerp(float t, float a, float b) const {
  return a + t * (b - a);
}

float PerlinNoise2D::grad(int hash, float x, float y) const {
  int h = hash & 0xF; // Take the last 4 bits of the hash.
  float u = h < 8 ? x : y;
  float v = h < 4 ? y : (h == 12 || h == 14 ? x : 0);
  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// https://gpfault.net/posts/perlin-noise.txt.html
float PerlinNoise2D::sample(float x, float y) const {
  int X = static_cast<int>(std::floor(x)) & 255;
  int Y = static_cast<int>(std::floor(y)) & 255;

  x -= std::floor(x);
  y -= std::floor(y);

  float u = fade(x);
  float v = fade(y);

  int aa = permutation_[permutation_[X] + Y];
  int ab = permutation_[permutation_[X] + Y + 1];
  int ba = permutation_[permutation_[X + 1] + Y];
  int bb = permutation_[permutation_[X + 1] + Y + 1];

  float res = lerp(v,
    lerp(u, grad(aa, x, y), grad(ba, x - 1, y)),
    lerp(u, grad(ab, x, y - 1), grad(bb, x - 1, y - 1))
  );

  return res; // Output remains in [-1, 1].
}
