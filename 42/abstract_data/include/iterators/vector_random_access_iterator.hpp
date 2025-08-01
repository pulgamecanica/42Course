#ifndef RANDOM_ACCESS_ITERATOR_HPP
# define RANDOM_ACCESS_ITERATOR_HPP

# include <iterator>
# include <cstddef>
# include "utils/enable_if.hpp"
# include "utils/is_convertible.hpp"

namespace ft {

template <typename T>
class vector_random_access_iterator {
public:
  typedef T                                value_type;
  typedef T*                               pointer;
  typedef T&                               reference;
  typedef std::ptrdiff_t                   difference_type;
  typedef std::random_access_iterator_tag  iterator_category;

private:
  pointer _ptr;

public:
  // Constructors
  vector_random_access_iterator(): _ptr(0) {}
  explicit vector_random_access_iterator(pointer ptr): _ptr(ptr) {}
  vector_random_access_iterator(const vector_random_access_iterator& other): _ptr(other._ptr) {}

  // Conversion from iterator<U> to iterator<T> if U* is convertible to T*
  template <typename U>
  vector_random_access_iterator(const vector_random_access_iterator<U>& other,
    typename ft::enable_if<ft::is_convertible<U*, T*>::value>::type* = 0)
    : _ptr(other.base()) {}

  // Assignment
  vector_random_access_iterator& operator=(const vector_random_access_iterator& other) {
    if (this != &other) _ptr = other._ptr;
    return *this;
  }

  // Access
  reference operator*() const { return *_ptr; }
  pointer operator->() const { return _ptr; }
  reference operator[](difference_type n) const { return _ptr[n]; }

  // Increment / Decrement
  vector_random_access_iterator& operator++() { ++_ptr; return *this; }
  vector_random_access_iterator operator++(int) { vector_random_access_iterator tmp(*this); ++_ptr; return tmp; }
  vector_random_access_iterator& operator--() { --_ptr; return *this; }
  vector_random_access_iterator operator--(int) { vector_random_access_iterator tmp(*this); --_ptr; return tmp; }

  // Arithmetic
  vector_random_access_iterator operator+(difference_type n) const { return vector_random_access_iterator(_ptr + n); }
  vector_random_access_iterator operator-(difference_type n) const { return vector_random_access_iterator(_ptr - n); }
  difference_type operator-(const vector_random_access_iterator& rhs) const { return _ptr - rhs._ptr; }
  vector_random_access_iterator& operator+=(difference_type n) { _ptr += n; return *this; }
  vector_random_access_iterator& operator-=(difference_type n) { _ptr -= n; return *this; }

  // Getter
  pointer base() const { return _ptr; }
};

// Addition with int on the left
template <typename T>
vector_random_access_iterator<T> operator+(
  typename vector_random_access_iterator<T>::difference_type n,
  const vector_random_access_iterator<T>& it) {
  return it + n;
}

// Comparison operators
template <typename T, typename U>
bool operator==(const vector_random_access_iterator<T>& lhs, const vector_random_access_iterator<U>& rhs) {
  return lhs.base() == rhs.base();
}

template <typename T, typename U>
bool operator!=(const vector_random_access_iterator<T>& lhs, const vector_random_access_iterator<U>& rhs) {
  return lhs.base() != rhs.base();
}

template <typename T, typename U>
bool operator<(const vector_random_access_iterator<T>& lhs, const vector_random_access_iterator<U>& rhs) {
  return lhs.base() < rhs.base();
}

template <typename T, typename U>
bool operator<=(const vector_random_access_iterator<T>& lhs, const vector_random_access_iterator<U>& rhs) {
  return lhs.base() <= rhs.base();
}

template <typename T, typename U>
bool operator>(const vector_random_access_iterator<T>& lhs, const vector_random_access_iterator<U>& rhs) {
  return lhs.base() > rhs.base();
}

template <typename T, typename U>
bool operator>=(const vector_random_access_iterator<T>& lhs, const vector_random_access_iterator<U>& rhs) {
  return lhs.base() >= rhs.base();
}

} // namespace ft

#endif // RANDOM_ACCESS_ITERATOR_HPP
