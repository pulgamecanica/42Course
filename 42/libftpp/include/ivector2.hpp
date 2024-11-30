/**
 * @file ivector2.hpp
 * @brief Header file for the `IVector2` struct, a 2D vector template class.
 */

#ifndef IVECTOR2_HPP
#define IVECTOR2_HPP

#include <cmath>

/**
 * @struct IVector2
 * @brief Represents a 2D vector with templated type.
 * 
 * Provides various operations like addition, subtraction, multiplication, and division,
 * as well as utility methods for vector normalization, dot product, and cross product.
 * 
 * @tparam TType The type of the vector's components (e.g., int, float, double).
 */
template <typename TType>
struct IVector2 {
  /**
   * @brief Vector2 constructor
   * @param x TType value representing `x`
   * @param y TType value representing `y`
   */
  IVector2(const TType& x, const TType& y): x(x), y(y) {}

  /**
   * @brief Adds this vector to another vector.
   * @param other The other vector to add.
   * @return A new vector representing the result of the addition.
   */
  IVector2 operator+(const IVector2& other) const {
    return {x + other.x, y + other.y};
  }

  /**
   * @brief Subtracts another vector from this vector.
   * @param other The other vector to subtract.
   * @return A new vector representing the result of the subtraction.
   */
  IVector2 operator-(const IVector2& other) const {
    return {x - other.x, y - other.y};
  }

  /**
   * @brief Multiplies this vector by a scalar value.
   * @param scalar The scalar to multiply by.
   * @return A new vector representing the result of the multiplication.
   */
  IVector2 operator*(const TType& scalar) const {
    return {x * scalar, y * scalar};
  }

  /**
   * @brief Multiplies this vector by another vector.
   * @param other The other vector to multiply.
   * @return A new vector representing the result of the multiplication.
   */
  IVector2 operator*(const IVector2& other) const {
    return {x * other.x, y * other.y};
  }

  /**
   * @brief Divides this vector by a scalar value.
   * @param scalar The scalar to divide by.
   * @return A new vector representing the result of the division.
   */
  IVector2 operator/(const TType& scalar) const {
    return {x / scalar, y / scalar};
  }


  /**
   * @brief Divides this vector by another vector.
   * @param scalar The scalar to divide by.
   * @return A new vector representing the result of the division.
   */
  IVector2 operator/(const IVector2& other) const {
    return {x / other.x, y / other.y};
  }

  /**
   * @brief Compares this vector with another vector for equality.
   * @param other The other vector to compare with.
   * @return True if the vectors are equal, false otherwise.
   */
  bool operator==(const IVector2& other) const {
    return x == other.x && y == other.y;
  }

  /**
   * @brief Compares this vector with another vector for inequality.
   * @param other The other vector to compare with.
   * @return True if the vectors are not equal, false otherwise.
   */
  bool operator!=(const IVector2& other) const {
    return !(*this == other);
  }

  /**
   * @brief Computes the length (norm) of the vector.
   * @return The length of the vector as a float.
   */
  float length() const {
    return std::sqrt(x * x + y * y);
  }

  /**
   * @brief Normalizes the vector (makes its length equal to 1).
   * @return A new vector representing the normalized vector.
   */
  IVector2<float> normalize() const {
    float len = length();
    if (len == 0) {
      return {0.0f, 0.0f};
    }
    return {x / len, y / len};
  }

  /**
   * @brief Computes the dot product of this vector with another vector.
   * @param other The other vector to compute the dot product with.
   * @return The dot product as a float.
   */
  float dot(const IVector2& other) const {
    return x * other.x + y * other.y;
  }

  /**
   * @brief Computes the cross product of this vector itself.
   * 
   * @note Two parallel vectors will always return zero cross product.
   * @return Zero
   */
  IVector2 cross() const {
    return IVector2(0, 0);
  }

  /**
   * @brief Computes the cross product of this vector with another vector.
   * @param other The other vector to compute the cross product with.
   * @return The cross product as a float.
   */
  float cross(const IVector2& other) const {
    return x * other.y - y * other.x;
  }

  const TType x; ///< First coordinate of the vector.
  const TType y; ///< Second coordinate of the vector.
};

#endif // IVECTOR2_HPP
