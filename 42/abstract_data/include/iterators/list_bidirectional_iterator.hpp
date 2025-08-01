#ifndef FT_LIST_ITERATOR_HPP
#define FT_LIST_ITERATOR_HPP

#include "utils/enable_if.hpp"
#include "utils/is_convertible.hpp"
#include <iterator>

namespace ft {

template <typename T, typename Node>
class list_bidirectional_iterator {
public:
  typedef T                                value_type;
  typedef T&                               reference;
  typedef T*                               pointer;
  typedef std::ptrdiff_t                   difference_type;
  typedef std::bidirectional_iterator_tag  iterator_category;

private:
  Node* _node;

public:
  list_bidirectional_iterator() : _node(NULL) {}
  explicit list_bidirectional_iterator(Node* node) : _node(node) {}

  // Allow conversion from iterator<T> to const_iterator<T>
  template <typename U, typename = typename ft::enable_if<ft::is_convertible<U*, T*>::value>::type>
  list_bidirectional_iterator(const list_bidirectional_iterator<U, Node>& other) : _node(other.base()) {}

  reference operator*() const { return _node->value; }
  pointer operator->() const { return &(_node->value); }

  list_bidirectional_iterator& operator++() { _node = _node->next; return *this; }
  list_bidirectional_iterator operator++(int) { list_bidirectional_iterator tmp(*this); ++(*this); return tmp; }

  list_bidirectional_iterator& operator--() { _node = _node->prev; return *this; }
  list_bidirectional_iterator operator--(int) { list_bidirectional_iterator tmp(*this); --(*this); return tmp; }

  Node* base() const { return _node; }

  template <typename T1, typename T2, typename N>
  friend bool operator==(const list_bidirectional_iterator<T1, N>& lhs, const list_bidirectional_iterator<T2, N>& rhs);

  template <typename T1, typename T2, typename N>
  friend bool operator!=(const list_bidirectional_iterator<T1, N>& lhs, const list_bidirectional_iterator<T2, N>& rhs);
};

template <typename T1, typename T2, typename Node>
bool operator==(const list_bidirectional_iterator<T1, Node>& lhs, const list_bidirectional_iterator<T2, Node>& rhs) {
  return lhs.base() == rhs.base();
}

template <typename T1, typename T2, typename Node>
bool operator!=(const list_bidirectional_iterator<T1, Node>& lhs, const list_bidirectional_iterator<T2, Node>& rhs) {
  return lhs.base() != rhs.base();
}

} // namespace ft

#endif // FT_LIST_ITERATOR_HPP
