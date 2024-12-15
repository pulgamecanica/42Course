/**
 * @file perlin_noise_2D.hpp
 * @brief Header file for the `PerlinNoise2D` class.
 */

#ifndef PERLIN_NOISE_2D_HPP
#define PERLIN_NOISE_2D_HPP

#include <vector>

/**
 * @class PerlinNoise2D
 * @brief A class for generating 2D Perlin noise values.
 * 
 * Provides methods to sample Perlin noise at given 2D coordinates.
 */
class PerlinNoise2D {
public:
  /**
   * @brief Constructs a `PerlinNoise2D` generator with an optional seed.
   * @param seed The seed for initializing the noise generator (default: 42).
   */
  PerlinNoise2D(int seed = 42);

  /**
   * @brief Samples Perlin noise at the given 2D coordinates.
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @return A float value representing the Perlin noise at the coordinates in range [-1, 1].
   */
  float sample(float x, float y) const;

private:
  /**
   * @brief Computes a fade function for smoothing.
   * @param t The input value.
   * @return The smoothed value.
   */
  float fade(float t) const;

  /**
   * @brief Computes a linear interpolation.
   * @param t The interpolation factor.
   * @param a The start value.
   * @param b The end value.
   * @return The interpolated value.
   */
  float lerp(float t, float a, float b) const;

  /**
   * @brief Computes a gradient for a hash and coordinates.
   * @param hash The gradient hash value.
   * @param x The x-offset.
   * @param y The y-offset.
   * @return The gradient dot product.
   */
  float grad(int hash, float x, float y) const;

  std::vector<int> permutation_; ///< Permutation table for gradient hashing.
};

#endif // PERLIN_NOISE_2D_HPP
