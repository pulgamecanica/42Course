#ifndef FT_RBT_HPP
#define FT_RBT_HPP

#include <memory>
#include <functional>
#include "node.hpp"
#include "../iterators/bt_iterator.hpp"
#include "../iterators/reverse_iterator.hpp"
#include "../utility.hpp"

#include <iostream>

namespace ft {

template<
  class Key,
  class Value,
  class KeyOfValue,
  class Compare = std::less<Key>,
  class Alloc = std::allocator<Value>,
  bool Multi = false
>
class RedBlackTree {
public:
  // Type aliases
  typedef Key                                     key_type;
  typedef Value                                   value_type;
  typedef Compare                                 key_compare;
  typedef Alloc                                   allocator_type;
  typedef value_type&                             reference;
  typedef const value_type&                       const_reference;
  typedef typename allocator_type::pointer        pointer;
  typedef typename allocator_type::const_pointer  const_pointer;
  typedef ptrdiff_t                               difference_type;
  typedef size_t                                  size_type;

  // Node and iterator types
  typedef BTNode<value_type>                      node_type;
  typedef node_type*                              node_pointer;

  typedef BTIterator<value_type, pointer, reference, key_compare>              iterator;
  typedef BTIterator<value_type, const_pointer, const_reference, key_compare>  const_iterator;
  typedef ft::reverse_iterator<iterator>                          reverse_iterator;
  typedef ft::reverse_iterator<const_iterator>                    const_reverse_iterator;

  class value_compare;

private:
  allocator_type  _alloc;
  key_compare     _comp;
  KeyOfValue      _key_of_value;
  node_pointer    _root;
  node_pointer    _sentinel;
  size_type       _size;

public:
  // Constructors / destructor / assignment
  explicit RedBlackTree(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) {
    std::cout << "Hello, this is RedBlackTree constructor" << std::endl;
  }

  RedBlackTree(const RedBlackTree& other);
  RedBlackTree& operator=(const RedBlackTree& other);

  ~RedBlackTree() {
    std::cout << "DESTRUCTION OF THE MAP" << std::endl;
  }

  // Iterators
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;

  // Capacity
  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  // Modifiers
  void clear();
  ft::pair<iterator, bool> insert(const value_type& val);
  iterator insert(iterator hint, const value_type& val);
  template<class InputIt>
  void insert(InputIt first, InputIt last);
  size_type erase(const key_type& key);
  void erase(iterator pos);
  void erase(iterator first, iterator last);
  void swap(RedBlackTree& other);

  // Lookup
  iterator find(const key_type& key);
  const_iterator find(const key_type& key) const;
  size_type count(const key_type& key) const;
  iterator lower_bound(const key_type& key);
  const_iterator lower_bound(const key_type& key) const;
  iterator upper_bound(const key_type& key);
  const_iterator upper_bound(const key_type& key) const;
  ft::pair<iterator, iterator> equal_range(const key_type& key);
  ft::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

  // Observers
  key_compare key_comp() const;
  value_compare value_comp() const;

private:
  // Helper methods (to be implemented later)
  void _rotate_left(node_pointer x);
  void _rotate_right(node_pointer x);
  void _insert_fixup(node_pointer z);
  void _delete_fixup(node_pointer x);
  void _transplant(node_pointer u, node_pointer v);
  node_pointer _minimum(node_pointer x) const;
  node_pointer _maximum(node_pointer x) const;
  void _clear(node_pointer x);
  node_pointer _copy(node_pointer x, node_pointer parent);

  // Allocation helpers
  node_pointer _create_node(const value_type& val);
  void _destroy_node(node_pointer node);

  // Comparison helper
  bool _equal_keys(const key_type& a, const key_type& b) const;
  bool _less_keys(const key_type& a, const key_type& b) const;
};

// value_compare nested class
template <
  class Key, class Value, class KeyOfValue,
  class Compare, class Alloc, bool Multi
>
class RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc, Multi>::value_compare {
  friend class RedBlackTree;
protected:
  Compare comp;
  explicit value_compare(Compare c) : comp(c) {}
public:
  bool operator()(const value_type& lhs, const value_type& rhs) const {
      return comp(KeyOfValue()(lhs), KeyOfValue()(rhs));
  }
};

} // namespace ft

#endif // FT_RBT_HPP
