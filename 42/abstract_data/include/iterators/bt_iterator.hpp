#ifndef FT_BT_ITERATOR_HPP
#define FT_BT_ITERATOR_HPP

#include "../utils/node.hpp"
#include <cstddef>

namespace ft {

template <typename T, typename Pointer, typename Reference, class Compare>
class BTIterator {
public:
  typedef BTIterator<T, Pointer, Reference, Compare>  type;
  typedef BTIterator<T, T*, T&, Compare>              iterator;
  typedef BTIterator<T, const T*, const T&, Compare>  const_iterator;
  typedef size_t                                      size_type;
  typedef ptrdiff_t                                   difference_type;
  typedef T                                           value_type;
  typedef Pointer                                     pointer;
  typedef Reference                                   reference;
  typedef BTNode<T>*                                  node_pointer;
  typedef std::bidirectional_iterator_tag             iterator_category;

  node_pointer _ptr;

  BTIterator() : _ptr(NULL) {}

  BTIterator(const node_pointer p) : _ptr(p) {}

  BTIterator(const BTIterator& other) : _ptr(other._ptr) {}

  template <typename P2, typename R2>
  BTIterator(const BTIterator<T, P2, R2, Compare>& other) : _ptr(other._ptr) {}

  BTIterator& operator=(const BTIterator& other) {
    if (this != &other)
      this->_ptr = other._ptr;
    return *this;
  }

  ~BTIterator() {}

  // Dereference
  reference operator*() const { return _ptr->getData(); }
  pointer operator->() const { return &_ptr->getData(); }

  // Increment
  type operator++(int) { type tmp = *this; _ptr = _ptr->successor(); return tmp; }
  type& operator++()   { _ptr = _ptr->successor(); return *this; }

  // Decrement
  type operator--(int) { type tmp = *this; _ptr = _ptr->predecessor(); return tmp; }
  type& operator--()   { _ptr = _ptr->predecessor(); return *this; }
};

template<typename T, typename P1, typename R1, typename C1,
         typename P2, typename R2, typename C2>
bool operator==(const BTIterator<T, P1, R1, C1>& lhs,
                const BTIterator<T, P2, R2, C2>& rhs) {
  return lhs._ptr == rhs._ptr;
}

template<typename T, typename P1, typename R1, typename C1,
         typename P2, typename R2, typename C2>
bool operator!=(const BTIterator<T, P1, R1, C1>& lhs,
                const BTIterator<T, P2, R2, C2>& rhs) {
  return !(lhs == rhs);
}

}

#endif