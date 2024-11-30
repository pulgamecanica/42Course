/**
 * @file ivector3.hpp
 * @brief Header file for the `IVector3` struct, a 3D vector template class.
 */

#ifndef IVECTOR3_HPP
#define IVECTOR3_HPP

#include <cmath>

/**
 * @struct IVector3
 * @brief Represents a 3D vector with templated type.
 * 
 * Provides various operations like addition, subtraction, multiplication, and division,
 * as well as utility methods for vector normalization, dot product, and cross product.
 * 
 * @tparam TType The type of the vector's components (e.g., int, float, double).
 */
template <typename TType>
struct IVector3 {
    /**
   * @brief Vector2 constructor
   * @param x TType value representing `x`
   * @param y TType value representing `y`
   * @param z TType value representing `z`
   */
  IVector3(const TType& x, const TType& y, const TType& z): x(x), y(y), z(z) {}

  /**
   * @brief Adds this vector to another vector.
   * @param other The other vector to add.
   * @return A new vector representing the result of the addition.
   */
  IVector3 operator+(const IVector3& other) const {
    return {x + other.x, y + other.y, z + other.z};
  }

  /**
   * @brief Subtracts another vector from this vector.
   * @param other The other vector to subtract.
   * @return A new vector representing the result of the subtraction.
   */
  IVector3 operator-(const IVector3& other) const {
    return {x - other.x, y - other.y, z - other.z};
  }

  /**
   * @brief Multiplies this vector by a scalar value.
   * @param scalar The scalar to multiply by.
   * @return A new vector representing the result of the multiplication.
   */
  IVector3 operator*(const TType& scalar) const {
    return {x * scalar, y * scalar, z * scalar};
  }

  /**
   * @brief Multiplies this vector by another vector.
   * @param other The vector to multiply by.
   * @return A new vector representing the result of the multiplication.
   */
  IVector3 operator*(const IVector3& other) const {
    return {x * other.x, y * other.y, z * other.z};
  }

  /**
   * @brief Divides this vector by a scalar value.
   * @param scalar The scalar to divide by.
   * @return A new vector representing the result of the division.
   */
  IVector3 operator/(const TType& scalar) const {
    return {x / scalar, y / scalar, z / scalar};
  }

  /**
   * @brief Divides this vector by anoter vector.
   * @param other The vector to divide by.
   * @return A new vector representing the result of the division.
   */
  IVector3 operator/(const IVector3& other) const {
    return {x / other.x, y / other.y, z / other.z};
  }

  /**
   * @brief Compares this vector with another vector for equality.
   * @param other The other vector to compare with.
   * @return True if the vectors are equal, false otherwise.
   */
  bool operator==(const IVector3& other) const {
    return x == other.x && y == other.y && z == other.z;
  }

  /**
   * @brief Compares this vector with another vector for inequality.
   * @param other The other vector to compare with.
   * @return True if the vectors are not equal, false otherwise.
   */
  bool operator!=(const IVector3& other) const {
    return !(*this == other);
  }

  /**
   * @brief Computes the length (norm) of the vector.
   * @return The length of the vector as a float.
   */
  float length() const {
    return std::sqrt(x * x + y * y + z * z);
  }

  /**
   * @brief Normalizes the vector (makes its length equal to 1).
   * @return A new vector representing the normalized vector.
   */
  IVector3<float> normalize() const {
    float len = length();
    if (len == 0) {
      return {0.0f, 0.0f, 0.0f};
    }
    return {x / len, y / len, z / len};
  }

  /**
   * @brief Computes the dot product of this vector with another vector.
   * @param other The other vector to compute the dot product with.
   * @return The dot product as a float.
   */
  float dot(const IVector3& other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  /**
   * @brief Computes the cross product of this vector with itself.
   * 
   * @note Two parallel vectors will always return zero cross product.
   * @return A new vector representing the cross product.
   */
  IVector3 cross() const {
    return IVector3(0, 0, 0);
  }

  /**
   * @brief Computes the cross product of this vector with another vector.
   * @param other The other vector to compute the cross product with.
   * @return A new vector representing the cross product.
   */
  IVector3 cross(const IVector3& other) const {
    return {
      y * other.z - z * other.y,
      z * other.x - x * other.z,
      x * other.y - y * other.x
    };
  }

  TType x; ///< First coordinate of the vector.
  TType y; ///< Second coordinate of the vector.
  TType z; ///< Third coordinate of the vector.
};

#endif // IVECTOR3_HPP
