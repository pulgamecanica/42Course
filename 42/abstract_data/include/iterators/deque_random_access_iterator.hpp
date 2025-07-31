#ifndef DEQUE_RANDOM_ACCESS_ITERATOR_HPP
# define DEQUE_RANDOM_ACCESS_ITERATOR_HPP

# include <iterator>
# include <cstddef>
# include "utils/enable_if.hpp"
# include "utils/is_convertible.hpp"

namespace ft {

template <typename T>
class deque_random_access_iterator {
public:
  typedef std::size_t                     size_type; // See 23.1
  typedef T                               value_type;
  typedef T*                              pointer;
  typedef T&                              reference;
  typedef std::ptrdiff_t                  difference_type;
  typedef std::random_access_iterator_tag iterator_category;

private:
  static const size_type  BLOCK_SIZE = 64; // Must match deque impl
  void**  _map;           // pointer to array of block pointers (avoids the double pointer constness issue using a void)
  size_type _map_index;     // index into map
  size_type _block_offset;  // offset within current block

public:
  // Constructors
  deque_random_access_iterator()
    : _map(0), _map_index(0), _block_offset(0) {}

  deque_random_access_iterator(pointer* map, size_type index, size_type offset)
    : _map(map), _map_index(index), _block_offset(offset) {}

  template <typename U>
  deque_random_access_iterator(U** map, size_type index, size_type offset,
    typename ft::enable_if<ft::is_convertible<U*, T*>::value>::type* = 0)
    : _map(reinterpret_cast<void**>(map)), _map_index(index), _block_offset(offset) {}


  template <typename U>
  deque_random_access_iterator(const deque_random_access_iterator<U>& other,
    typename ft::enable_if<ft::is_convertible<U*, T*>::value>::type* = 0)
    : _map(other._map), _map_index(other._map_index), _block_offset(other._block_offset) {}

  // Dereference
  reference operator*() const {
    return *(reinterpret_cast<pointer>(_map[_map_index]) + _block_offset);
  }

  pointer operator->() const {
    return reinterpret_cast<pointer>(_map[_map_index]) + _block_offset;
  }

  reference operator[](difference_type n) const {
    deque_random_access_iterator tmp = *this + n;
    return *tmp;
  }

  // Increment / Decrement
  deque_random_access_iterator& operator++() {
    if (++_block_offset == BLOCK_SIZE) {
      ++_map_index;
      _block_offset = 0;
    }
    return *this;
  }

  deque_random_access_iterator operator++(int) {
    deque_random_access_iterator tmp(*this);
    ++(*this);
    return tmp;
  }

  deque_random_access_iterator& operator--() {
    if (_block_offset == 0) {
      --_map_index;
      _block_offset = BLOCK_SIZE - 1;
    } else {
      --_block_offset;
    }
    return *this;
  }

  deque_random_access_iterator operator--(int) {
    deque_random_access_iterator tmp(*this);
    --(*this);
    return tmp;
  }

  // Arithmetic
  deque_random_access_iterator operator+(difference_type n) const {
    difference_type offset = static_cast<difference_type>(_map_index * BLOCK_SIZE + _block_offset) + n;
    return deque_random_access_iterator(_map, offset / BLOCK_SIZE, offset % BLOCK_SIZE);
  }

  deque_random_access_iterator operator-(difference_type n) const {
    return *this + (-n);
  }

  difference_type operator-(const deque_random_access_iterator& rhs) const {
    return static_cast<difference_type>(_map_index * BLOCK_SIZE + _block_offset) -
           static_cast<difference_type>(rhs._map_index * BLOCK_SIZE + rhs._block_offset);
  }

  deque_random_access_iterator& operator+=(difference_type n) {
    *this = *this + n;
    return *this;
  }

  deque_random_access_iterator& operator-=(difference_type n) {
    *this = *this - n;
    return *this;
  }

  // Comparison operators
  template <typename U>
  bool operator==(const deque_random_access_iterator<U>& rhs) const {
    return _map == rhs._map &&
           _map_index == rhs._map_index &&
           _block_offset == rhs._block_offset;
  }

  template <typename U>
  bool operator!=(const deque_random_access_iterator<U>& rhs) const {
    return !(*this == rhs);
  }

  template <typename U>
  bool operator<(const deque_random_access_iterator<U>& rhs) const {
    return (*this - rhs) < 0;
  }

  template <typename U>
  bool operator<=(const deque_random_access_iterator<U>& rhs) const {
    return (*this - rhs) <= 0;
  }

  template <typename U>
  bool operator>(const deque_random_access_iterator<U>& rhs) const {
    return (*this - rhs) > 0;
  }

  template <typename U>
  bool operator>=(const deque_random_access_iterator<U>& rhs) const {
    return (*this - rhs) >= 0;
  }

  // Access to raw state (optional)
  pointer* map() const { return _map; }
  size_type map_index() const { return _map_index; }
  size_type block_offset() const { return _block_offset; }
};

template <typename T>
deque_random_access_iterator<T> operator+(
  typename deque_random_access_iterator<T>::difference_type n,
  const deque_random_access_iterator<T>& it) {
  return it + n;
}

} // namespace ft

#endif // DEQUE_RANDOM_ACCESS_ITERATOR_HPP
