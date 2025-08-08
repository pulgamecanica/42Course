#ifndef FT_SET_HPP
#define FT_SET_HPP

#include <memory>
#include "utility.hpp"
#include "utils/rbt.hpp"
#include "utils/exception.hpp"
#include "utils/less.hpp"
#include "utils/lexicographical_compare.hpp"
#include "iterators/reverse_iterator.hpp"
#include "iterators/bt_iterator.hpp"

namespace ft {

template <
  class Key,
  class Compare = ft::less<Key>,
  class Alloc = std::allocator<Key>
>
class set {
public:
  typedef Key                                     key_type;
  typedef Key                                     value_type;
  typedef Compare                                 key_compare;
  typedef Alloc                                   allocator_type;
  typedef value_type&                             reference;
  typedef const value_type&                       const_reference;
  typedef typename allocator_type::pointer        pointer;
  typedef typename allocator_type::const_pointer  const_pointer;
  typedef ptrdiff_t                               difference_type;
  typedef size_t                                  size_type;

private:
  typedef RedBlackTree<
      key_type,
      value_type,
      ft::identity<value_type>,
      key_compare,
      allocator_type,
      false // Multi = false
  > tree_type;

  tree_type _tree;

public:
  typedef typename tree_type::iterator                iterator;
  typedef typename tree_type::const_iterator          const_iterator;
  typedef typename tree_type::reverse_iterator        reverse_iterator;
  typedef typename tree_type::const_reverse_iterator  const_reverse_iterator;
  typedef typename tree_type::value_compare           value_compare;

  // Constructors
  set() {}

  explicit set(const key_compare& comp, const allocator_type& alloc = allocator_type())
    : _tree(comp, alloc) {}

  template <class InputIt>
  set(InputIt first, InputIt last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
    : _tree(first, last, comp, alloc) {}

  set(const set& other): _tree(other._tree) {}

  set& operator=(const set& other) {
    if (this != &other)
      _tree = other._tree;
    return *this;
  }

  ~set() {
    clear();
  }

  // Iterators
  iterator begin() { return _tree.begin(); }
  const_iterator begin() const { return _tree.begin(); }
  iterator end() { return _tree.end(); }
  const_iterator end() const { return _tree.end(); }
  reverse_iterator rbegin() { return _tree.rbegin(); }
  const_reverse_iterator rbegin() const { return _tree.rbegin(); }
  reverse_iterator rend() { return _tree.rend(); }
  const_reverse_iterator rend() const { return _tree.rend(); }

  // Capacity
  bool empty() const { return _tree.empty(); }
  size_type size() const { return _tree.size(); }
  size_type max_size() const { return _tree.max_size(); }

  // Modifiers
  void clear() { _tree.clear(); }

  ft::pair<iterator, bool> insert(const value_type& val) {
    return _tree.insert(val);
  }

  iterator insert(iterator hint, const value_type& val) {
    return _tree.insert(hint, val);
  }

  template <class InputIt>
  void insert(InputIt first, InputIt last) {
    _tree.insert(first, last);
  }

  void erase(iterator pos) { _tree.erase(pos); }

  size_type erase(const key_type& key) { return _tree.erase(key); }

  void erase(iterator first, iterator last) { _tree.erase(first, last); }

  void swap(set& other) { _tree.swap(other._tree); }

  // Lookup
  size_type count(const key_type& key) const {
    return _tree.find(key) != _tree.end();
  }

  iterator find(const key_type& key) { return _tree.find(key); }

  const_iterator find(const key_type& key) const { return _tree.find(key); }

  iterator lower_bound(const key_type& key) { return _tree.lower_bound(key); }

  const_iterator lower_bound(const key_type& key) const { return _tree.lower_bound(key); }

  iterator upper_bound(const key_type& key) { return _tree.upper_bound(key); }

  const_iterator upper_bound(const key_type& key) const { return _tree.upper_bound(key); }

  ft::pair<iterator, iterator> equal_range(const key_type& key) {
    return _tree.equal_range(key);
  }

  ft::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
    return _tree.equal_range(key);
  }

  // Observers
  key_compare key_comp() const { return _tree.key_comp(); }

  value_compare value_comp() const { return _tree.value_comp(); }

  allocator_type get_allocator() const { return _tree.get_allocator(); }

  void print() {
    _tree.print_metadata();
    _tree.print_tree_top_down();
  }
};

// Relational operators
template <class Key, class Compare, class Alloc>
bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
  return (lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template <class Key, class Compare, class Alloc>
bool operator!=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <class Key, class Compare, class Alloc>
bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
  return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class Key, class Compare, class Alloc>
bool operator<=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
  return !(rhs < lhs);
}

template <class Key, class Compare, class Alloc>
bool operator>(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
  return (rhs < lhs);
}

template <class Key, class Compare, class Alloc>
bool operator>=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
  return !(lhs < rhs);
}

template <class Key, class Compare, class Alloc>
void swap(ft::set<Key, Compare, Alloc>& lhs, ft::set<Key, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}

} // namespace ft

#endif // FT_SET_HPP
