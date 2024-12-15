#include "random_2D_coordinate_generator.hpp"

Random2DCoordinateGenerator::Random2DCoordinateGenerator(const long long& seed)
  : seed_value(seed) {}

long long Random2DCoordinateGenerator::seed() const {
  return seed_value;
}

// https://en.wikipedia.org/wiki/Linear_congruential_generator
long long Random2DCoordinateGenerator::operator()(const long long& x, const long long& y) const {
  return (seed_value ^ x ^ y) * 25214903917LL + 11LL;
}
