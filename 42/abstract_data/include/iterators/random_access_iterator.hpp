#ifndef RANDOM_ACCESS_ITERATOR_HPP
# define RANDOM_ACCESS_ITERATOR_HPP

# include <iterator>
# include <cstddef>
# include "utils/enable_if.hpp"
# include "utils/is_convertible.hpp"

namespace ft {

template <typename T>
class random_access_iterator {
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
  random_access_iterator(): _ptr(0) {}
  explicit random_access_iterator(pointer ptr): _ptr(ptr) {}
  random_access_iterator(const random_access_iterator& other): _ptr(other._ptr) {}

  // Conversion from iterator<U> to iterator<T> if U* is convertible to T*
  template <typename U>
  random_access_iterator(const random_access_iterator<U>& other,
    typename ft::enable_if<ft::is_convertible<U*, T*>::value>::type* = 0)
    : _ptr(other.base()) {}

  // Assignment
  random_access_iterator& operator=(const random_access_iterator& other) {
    if (this != &other) _ptr = other._ptr;
    return *this;
  }

  // Access
  reference operator*() const { return *_ptr; }
  pointer operator->() const { return _ptr; }
  reference operator[](difference_type n) const { return _ptr[n]; }

  // Increment / Decrement
  random_access_iterator& operator++() { ++_ptr; return *this; }
  random_access_iterator operator++(int) { random_access_iterator tmp(*this); ++_ptr; return tmp; }
  random_access_iterator& operator--() { --_ptr; return *this; }
  random_access_iterator operator--(int) { random_access_iterator tmp(*this); --_ptr; return tmp; }

  // Arithmetic
  random_access_iterator operator+(difference_type n) const { return random_access_iterator(_ptr + n); }
  random_access_iterator operator-(difference_type n) const { return random_access_iterator(_ptr - n); }
  difference_type operator-(const random_access_iterator& rhs) const { return _ptr - rhs._ptr; }
  random_access_iterator& operator+=(difference_type n) { _ptr += n; return *this; }
  random_access_iterator& operator-=(difference_type n) { _ptr -= n; return *this; }

  // Getter
  pointer base() const { return _ptr; }
};

// Addition with int on the left
template <typename T>
random_access_iterator<T> operator+(
  typename random_access_iterator<T>::difference_type n,
  const random_access_iterator<T>& it) {
  return it + n;
}

// Comparison operators
template <typename T, typename U>
bool operator==(const random_access_iterator<T>& lhs, const random_access_iterator<U>& rhs) {
  return lhs.base() == rhs.base();
}

template <typename T, typename U>
bool operator!=(const random_access_iterator<T>& lhs, const random_access_iterator<U>& rhs) {
  return lhs.base() != rhs.base();
}

template <typename T, typename U>
bool operator<(const random_access_iterator<T>& lhs, const random_access_iterator<U>& rhs) {
  return lhs.base() < rhs.base();
}

template <typename T, typename U>
bool operator<=(const random_access_iterator<T>& lhs, const random_access_iterator<U>& rhs) {
  return lhs.base() <= rhs.base();
}

template <typename T, typename U>
bool operator>(const random_access_iterator<T>& lhs, const random_access_iterator<U>& rhs) {
  return lhs.base() > rhs.base();
}

template <typename T, typename U>
bool operator>=(const random_access_iterator<T>& lhs, const random_access_iterator<U>& rhs) {
  return lhs.base() >= rhs.base();
}

} // namespace ft

#endif // RANDOM_ACCESS_ITERATOR_HPP
