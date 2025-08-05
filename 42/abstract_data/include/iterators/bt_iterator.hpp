#ifndef BT_ITERATOR_HPP
# define BT_ITERATOR_HPP

# include <memory>
# include <cstddef>
# include "../utils/node.hpp"

namespace ft {
template < typename T, typename Pointer, typename Reference, class C >
class BTIterator {
public:
  typedef BTIterator<T, Pointer, Reference, C > type;
  typedef BTIterator<T,       T*,       T&, C > iterator;
  typedef BTIterator<T, const T*, const T&, C > const_iterator;
  typedef size_t                                size_type;
  typedef ptrdiff_t                             difference_type;
  typedef T                                     value_type;
  typedef Pointer                               pointer;
  typedef Reference                             reference;
  typedef BTNode<T, C>*                         node_pointer;
  typedef std::bidirectional_iterator_tag       iterator_category;

  BTIterator() : ptr(NULL) { }
  BTIterator(const node_pointer ptr_) : ptr(ptr_) {}
  BTIterator(const iterator& it) : ptr(const_cast<node_pointer>(it.ptr)) { }

  type&  operator=(const const_iterator& it) {
    this->ptr = it.ptr;
    return *this;
  }

  virtual ~BTIterator() {}

  BTIterator operator++(int) {
    BTIterator tmp(*this);
    this->ptr = ptr->successor();
    return tmp;
  }

  BTIterator&    operator++() {
    this->ptr = ptr->successor();
    return *this;
  }

  BTIterator operator--(int) {
    BTIterator tmp(*this);
    this->ptr = ptr->predecessor();
    return tmp;
  }

  BTIterator&    operator--() {
    this->ptr = ptr->predecessor();
    return *this;
  }

  reference   operator*() {
    return this->ptr->getData(); // CHECK IF THIS WORKS!
  }

  pointer     operator->() {
    return (&(this->ptr->getData()));
  }

  template<typename T2, typename P, typename R, class C2>
  friend inline bool operator==(const type& lhs, const BTIterator<T2, P, R, C2>& rhs) {
    return (lhs.ptr == rhs.ptr);
  }

  template<typename T2, typename P, typename R, class C2>
  friend inline bool operator!=(const type& lhs, const BTIterator<T2, P, R, C2>& rhs) { return !(lhs == rhs); }

  node_pointer    ptr;
};

}

#endif