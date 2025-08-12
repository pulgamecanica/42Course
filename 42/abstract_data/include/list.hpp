#ifndef FT_LIST_HPP
#define FT_LIST_HPP

#include <limits>
#include "utils/less.hpp"
#include "utils/swap.hpp"
#include "utils/node.hpp"
#include "utils/enable_if.hpp"
#include "exception.hpp"
#include "iterators/list_bidirectional_iterator.hpp"
#include "iterators/reverse_iterator.hpp"

#include <iostream>

namespace ft {

template <class T, class Alloc = std::allocator<T> >
class list {
private:
  typedef ft::Node<T>                   Node;
  typedef typename std::allocator<Node> node_allocator_type;

public:
  typedef T                                         value_type;
  typedef Alloc                                     allocator_type;
  typedef typename allocator_type::reference        reference;
  typedef typename allocator_type::const_reference  const_reference;
  typedef typename allocator_type::pointer          pointer;
  typedef typename allocator_type::const_pointer    const_pointer;
  typedef std::size_t                               size_type;            // See 23.1
  typedef std::ptrdiff_t                            difference_type;      // See 23.1

  typedef ft::list_bidirectional_iterator<T, Node>        iterator;       // See 23.1
  typedef ft::list_bidirectional_iterator<const T, Node>  const_iterator; // See 23.1
  typedef ft::reverse_iterator<iterator>                  reverse_iterator;
  typedef ft::reverse_iterator<const_iterator>            const_reverse_iterator;

private:
  Node*               _head;     // sentinel node (circular linked list)
  size_type           _size;
  allocator_type      _alloc;
  node_allocator_type _node_alloc;

  Node* _create_node(const T& val = T()) {
    Node* n = _node_alloc.allocate(1);
    _node_alloc.construct(n, Node(val));
    return n;
  }

  void _destroy_node(Node* node) {
    _node_alloc.destroy(node);
    _node_alloc.deallocate(node, 1);
  }

  void _link_between(Node* prev, Node* node, Node* next) {
    if (!node) return;
    node->prev = prev;
    node->next = next;
    if (prev) prev->next = node;
    if (next) next->prev = node;
  }

  void _remove_node(Node *node) {
    if (!node) return;
    Node* prev = node->prev;
    Node* next = node->next;

    prev->next = next;
    next->prev = prev;

    _destroy_node(node);
  }

  Node* _insert_before(Node* pos, const T& value) {
    Node* node = _create_node(value);
    _link_between(pos->prev, node, pos);
    ++_size;
    return node;
  }

  Node* _insert_after(Node* pos, const T& value) {
    Node* node = _create_node(value);
    _link_between(pos, node, pos->next);
    ++_size;
    return node;
  }

  void _transfer_range(Node* first, Node* last, Node* pos) {
    if (first == last) return;

    // Disconnect [first, last) from source list
    Node* before = first->prev;
    Node* after  = last->prev;

    before->next = last;
    last->prev   = before;

    // Connect [first, after] before pos
    Node* pos_prev = pos->prev;

    pos_prev->next = first;
    first->prev    = pos_prev;

    after->next    = pos;
    pos->prev      = after;
  }

  void _init_empty_list() {
    _head = _create_node(); // sentinel node
    _head->next = _head;
    _head->prev = _head;
  }
  
public:
  // 23.2.2.1 construct/copy/destroy:
  explicit list(const Alloc& alloc = Alloc())
    : _head(NULL), _size(0), _alloc(alloc), _node_alloc(node_allocator_type()) {
    _init_empty_list();
  }

  explicit list(size_type n, const T& value = T(), const Alloc& alloc = Alloc())
    : _head(NULL), _size(0), _alloc(alloc), _node_alloc(node_allocator_type()) {
    _init_empty_list();
    if (n > max_size())
      throw ft::out_of_range("list: n exceeds max_size");
    for (size_type i = 0; i < n; ++i)
      push_back(value);
  }

  template <class InputIterator>
  list(InputIterator first, InputIterator last,
        const Alloc& alloc = Alloc(),
        typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0)
    : _head(NULL), _size(0), _alloc(alloc), _node_alloc(node_allocator_type()) {
    _init_empty_list();
    for (; first != last; ++first)
      push_back(*first);
  }

  list(const list& x)
    : _head(NULL), _size(0), _alloc(x._alloc), _node_alloc(node_allocator_type()) {
    _init_empty_list();
    for (const_iterator it = x.begin(); it != x.end(); ++it)
      push_back(*it);
  }

  ~list() {
    clear();
    _destroy_node(_head);
  }

  list& operator=(const list& x) {
    if (this != &x) {
      clear();
      for (const_iterator it = x.begin(); it != x.end(); ++it)
        push_back(*it);
    }
    return *this;
  }

  template <class InputIterator>
  void assign(InputIterator first, InputIterator last,
              typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0) {
    clear();
    for (; first != last; ++first)
      push_back(*first);
  }

  void assign(size_type n, const T& t) {
    clear();
    for (size_type i = 0; i < n; i++)
      push_back(t);
  }

  allocator_type get_allocator() const { return _alloc; }

  // iterators:
  iterator begin() { return iterator(_head->next); }
  const_iterator begin() const { return const_iterator(_head->next); }
  iterator end()   { return iterator(_head); }
  const_iterator end()   const { return const_iterator(_head); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // 23.2.2.2 capacity:
  bool empty() const { return _size == 0; };
  size_type size() const { return _size; }
  size_type max_size() const { return _alloc.max_size(); }

  void resize(size_type sz, T c = T()) {
    size_type current = size();
    if (sz < current) {
      for (size_type i = 0; i < current - sz; ++i)
        pop_back();
    } else if (sz > current) {
      for (size_type i = 0; i < sz - current; ++i)
        push_back(c);
    }
  }

  // element access:
  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }
  reference back() { return *(--end()); }
  const_reference back() const { return *(--end()); }

  // 23.2.2.3 modifiers:
  void push_front(const T& x) {
    _insert_after(_head, x);
  }

  void pop_front() {
    if (empty()) return;
    _remove_node(_head->next);
    --_size;
  }

  void push_back(const T& x) {
    _insert_before(_head, x);
  }

  void pop_back() {
    if (empty()) return;
    _remove_node(_head->prev);
    --_size;
  }

  iterator insert(iterator position, const T& x) {
    Node* node = _insert_before(position.base(), x);
    return iterator(node);
  }

  void insert(iterator position, size_type n, const T& x) {
    for (size_type i = 0; i < n; ++i)
      _insert_before(position.base(), x);
  }

  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0) {
    for (; first != last; ++first)
      _insert_before(position.base(), *first);
  }

  iterator erase(iterator position) {
    Node* node = position.base();
    iterator next(node->next);
    _remove_node(node);
    --_size;
    return next;
  }

  iterator erase(iterator first, iterator last) {
    while (first != last)
      first = erase(first);
    return last;
  }

  void swap(list& other) {
    ft::swap(_head, other._head);
    ft::swap(_size, other._size);
    ft::swap(_alloc, other._alloc);
    ft::swap(_node_alloc, other._node_alloc);
  }

  void clear() {
    Node* cur = _head->next;
    while (cur != _head) {
      Node* next = cur->next;
      _destroy_node(cur);
      cur = next;
    }
    _head->next = _head;
    _head->prev = _head;
    _size = 0;
  }

  // 23.2.2.4 list operations:
  // splice <===> JOIN or CONNECT
  void splice(iterator position, list& x) {
    if (x.empty()) return;

    _transfer_range(x._head->next, x._head, position.base());

    _size += x._size;
    x._head->next = x._head;
    x._head->prev = x._head;
    x._size = 0;
  }

  void splice(iterator position, list& x, iterator i) {
    iterator next = i;
    ++next;
    if (position == i || position == next) return; // no-op

    _transfer_range(i.base(), next.base(), position.base());

    ++_size;
    --x._size;
  }

  void splice(iterator position, list& x, iterator first, iterator last) {
    if (first == last) return;

    // Count number of nodes
    size_type count = 0;
    for (iterator it = first; it != last; ++it)
      ++count;

    _transfer_range(first.base(), last.base(), position.base());

    _size += count;
    x._size -= count;
  }

  void remove(const T& value) {
    if (empty()) return;

    iterator it = begin();
    while (it != end()) {
      if (*it == value)
        it = erase(it);
      else
        ++it;
    }
  }

  template <class Predicate>
  void remove_if(Predicate pred) {
    if (empty()) return;

    iterator it = begin();
    while (it != end()) {
      if (pred(*it))
        it = erase(it);
      else
        ++it;
    }
  }

  void unique() {
    if (empty()) return;

    iterator it = begin();
    iterator next = it;
    ++next;

    while (next != end()) {
      if (*it == *next)
        next = erase(next);
      else
        it = next++;
    }
  }

  template <class BinaryPredicate>
  void unique(BinaryPredicate binary_pred) {
    if (empty()) return;

    iterator it = begin();
    iterator next = it;
    ++next;

    while (next != end()) {
      if (binary_pred(*it, *next))
        next = erase(next);
      else
        it = next++;
    }
  }

  void merge(list& x) {
    if (this == &x || x.empty()) return;

    iterator this_it = begin();
    iterator x_it = x.begin();

    while (this_it != end() && x_it != x.end()) {
      if (*x_it < *this_it) {
        iterator next = x_it;
        ++next;
        splice(this_it, x, x_it);
        x_it = next;
      } else {
        ++this_it;
      }
    }

    if (x_it != x.end())
      splice(end(), x, x_it, x.end());
  }

  template <class Compare>
  void merge(list& x, Compare comp) {
    if (this == &x || x.empty()) return;

    iterator this_it = begin();
    iterator x_it = x.begin();

    while (this_it != end() && x_it != x.end()) {
      if (comp(*x_it, *this_it)) {
        iterator next = x_it;
        ++next;
        splice(this_it, x, x_it);
        x_it = next;
      } else {
        ++this_it;
      }
    }

    if (x_it != x.end())
      splice(end(), x, x_it, x.end());
  }

  void sort() {
    sort(ft::less<T>());
  }

  /**
   * Repeatedly pulls one element from the list
   * Merges it into increasingly larger bins (counter[])
   * Uses your already-implemented merge() to combine sorted runs
   * At the end, merges everything back together
   */
  template <class Compare>
  void sort(Compare comp) {
    if (_size <= 1) return;

    list carry;
    list counter[64];
    int fill = 0;

    while (!empty()) {
      carry.splice(carry.begin(), *this, begin());

      int i = 0;
      while (i < fill && !counter[i].empty()) {
        counter[i].merge(carry, comp);
        carry.swap(counter[i]);
        ++i;
      }
      carry.swap(counter[i]);
      if (i == fill) ++fill;
    }

    for (int i = 1; i < fill; ++i)
      counter[i].merge(counter[i - 1], comp);

    swap(counter[fill - 1]);
  }

  void reverse() {
    if (_size <= 1) return;

    Node* current = _head;
    do {
      Node* tmp = current->next;
      current->next = current->prev;
      current->prev = tmp;
      current = tmp;
    } while (current != _head);
  }
};

// Relational operators
template <class T, class Alloc>
bool operator==(const ft::list<T, Alloc>& x, const ft::list<T, Alloc>& y) {
  if (x.size() != y.size())
    return false;

  typename ft::list<T, Alloc>::const_iterator it1 = x.begin();
  typename ft::list<T, Alloc>::const_iterator it2 = y.begin();

  for (; it1 != x.end(); ++it1, ++it2) {
    if (!(*it1 == *it2))
      return false;
  }

  return true;
}

template <class T, class Alloc>
bool operator<(const ft::list<T, Alloc>& x, const ft::list<T, Alloc>& y) {
  typename ft::list<T, Alloc>::const_iterator it1 = x.begin();
  typename ft::list<T, Alloc>::const_iterator it2 = y.begin();

  for (; it1 != x.end() && it2 != y.end(); ++it1, ++it2) {
    if (*it1 < *it2) return true;
    if (*it2 < *it1) return false;
  }

  return it1 == x.end() && it2 != y.end();
}

template <class T, class Alloc>
bool operator!=(const ft::list<T, Alloc>& x, const ft::list<T, Alloc>& y) {
  return !(x == y);
}

template <class T, class Alloc>
bool operator>(const ft::list<T, Alloc>& x, const ft::list<T, Alloc>& y) {
  return y < x;
}

template <class T, class Alloc>
bool operator>=(const ft::list<T, Alloc>& x, const ft::list<T, Alloc>& y) {
  return !(x < y);
}

template <class T, class Alloc>
bool operator<=(const ft::list<T, Alloc>& x, const ft::list<T, Alloc>& y) {
  return !(y < x);
}

// specialized algorithms:
template <class T, class Alloc>
void swap(ft::list<T, Alloc>& x, ft::list<T, Alloc>& y) {
  x.swap(y);
}


} // namespace ft

#endif // FT_LIST_HPP