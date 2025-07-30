// https://en.cppreference.com/w/cpp/iterator/reverse_iterator/

#ifndef FT_REVERSE_ITERATOR_HPP
# define FT_REVERSE_ITERATOR_HPP

# include <cstddef>
# include "iterator_traits.hpp"
# include "utils/enable_if.hpp"
# include "utils/is_convertible.hpp"

namespace ft {

template <typename Iter>
class reverse_iterator {
public:
  typedef Iter                                                  iterator_type;
  typedef typename ft::iterator_traits<Iter>::iterator_category iterator_category;
  typedef typename ft::iterator_traits<Iter>::value_type        value_type;
  typedef typename ft::iterator_traits<Iter>::difference_type   difference_type;
  typedef typename ft::iterator_traits<Iter>::pointer           pointer;
  typedef typename ft::iterator_traits<Iter>::reference         reference;

private:
  iterator_type _current;

public:
  // Constructors
  reverse_iterator() : _current() {}
  explicit reverse_iterator(iterator_type it) : _current(it) {}

  template <typename U>
  reverse_iterator(const reverse_iterator<U>& other,
      typename ft::enable_if<ft::is_convertible<U, Iter>::value>::type* = 0)
      : _current(other.base()) {}

  // Base accessor
  iterator_type base() const { return _current; }

  // Dereference
  reference operator*() const { Iter tmp = _current; return *--tmp; }
  pointer operator->() const { return &(operator*()); }
  reference operator[](difference_type n) const { return *(*this + n); }

  // Increment / Decrement
  reverse_iterator& operator++() { --_current; return *this; }
  reverse_iterator operator++(int) { reverse_iterator tmp(*this); --_current; return tmp; }
  reverse_iterator& operator--() { ++_current; return *this; }
  reverse_iterator operator--(int) { reverse_iterator tmp(*this); ++_current; return tmp; }

  // Arithmetic
  reverse_iterator operator+(difference_type n) const { return reverse_iterator(_current - n); }
  reverse_iterator operator-(difference_type n) const { return reverse_iterator(_current + n); }
  reverse_iterator& operator+=(difference_type n) { _current -= n; return *this; }
  reverse_iterator& operator-=(difference_type n) { _current += n; return *this; }
};

// Non-member comparison operators
template <typename Iter1, typename Iter2>
bool operator==(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
  return lhs.base() == rhs.base();
}

template <typename Iter1, typename Iter2>
bool operator!=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
  return lhs.base() != rhs.base();
}

template <typename Iter1, typename Iter2>
bool operator<(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
  return lhs.base() > rhs.base(); // reverse logic
}

template <typename Iter1, typename Iter2>
bool operator<=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
  return lhs.base() >= rhs.base();
}

template <typename Iter1, typename Iter2>
bool operator>(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
  return lhs.base() < rhs.base();
}

template <typename Iter1, typename Iter2>
bool operator>=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
  return lhs.base() <= rhs.base();
}

// Addition with offset on the left
template <typename Iter>
reverse_iterator<Iter> operator+(
  typename reverse_iterator<Iter>::difference_type n,
  const reverse_iterator<Iter>& it) {
  return it + n;
}

// Difference between reverse iterators
template <typename Iter1, typename Iter2>
typename reverse_iterator<Iter1>::difference_type
operator-(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs) {
  return rhs.base() - lhs.base(); // reverse logic
}

} // namespace ft

#endif // FT_REVERSE_ITERATOR_HPP
