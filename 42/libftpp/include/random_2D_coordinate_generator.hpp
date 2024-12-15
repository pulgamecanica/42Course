/**
 * @file random_2D_coordinate_generator.hpp
 * @brief Header file for the `Random2DCoordinateGenerator` class.
 */

#ifndef RANDOM_2D_COORDINATE_GENERATOR_HPP
#define RANDOM_2D_COORDINATE_GENERATOR_HPP

#include <cstdint>

/**
 * @class Random2DCoordinateGenerator
 * @brief A pseudo-random number generator using a 2D coordinate and seed.
 * 
 * Generates deterministic pseudo-random numbers based on a seed and 2D coordinates.
 */
class Random2DCoordinateGenerator {
public:
  /**
   * @brief Constructs a `Random2DCoordinateGenerator` with a given seed.
   * @param seed The seed for the generator.
   */
  Random2DCoordinateGenerator(const long long& seed = 42LL);

  /**
   * @brief Returns the current seed of the generator.
   * @return The seed as an long long.
   */
  long long seed() const;

  /**
   * @brief Generates a pseudo-random number using the given coordinates.
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @return A pseudo-random number as long long.
   */
  long long operator()(const long long& x, const long long& y) const;

private:
  const long long seed_value; ///< The seed value for the generator.
};

#endif // RANDOM_2D_COORDINATE_GENERATOR_HPP
