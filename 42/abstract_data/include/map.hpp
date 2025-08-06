#ifndef FT_MAP_HPP
#define FT_MAP_HPP

#include <memory>
#include "utility.hpp"
#include "utils/rbt.hpp"
#include "iterators/reverse_iterator.hpp"
#include "iterators/bt_iterator.hpp"

#include <iostream>

namespace ft {

template <typename Pair>
struct Select1st {
  typedef typename Pair::first_type result_type;
  const result_type& operator()(const Pair& p) const {
      return p.first;
  }
};

template <
  class Key,
  class T,
  class Compare = std::less<Key>,
  class Alloc = std::allocator<ft::pair<const Key, T> >
>
class map {
public:
  typedef Key                                     key_type;
  typedef T                                       mapped_type;
  typedef ft::pair<const Key, T>                  value_type;
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
      ft::Select1st<value_type>,
      key_compare,
      allocator_type,
      false // Multi = false, this is a map, not multimap
  > tree_type;

  tree_type _tree;

public:
  typedef typename tree_type::iterator                iterator;
  typedef typename tree_type::const_iterator          const_iterator;
  typedef typename tree_type::reverse_iterator        reverse_iterator;
  typedef typename tree_type::const_reverse_iterator  const_reverse_iterator;

  class value_compare;

public:
  // Constructors
  explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) {
    std::cout << "Hello, this is map constructor" << std::endl;
  }

  template <class InputIt>
  map(InputIt first, InputIt last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
  map(const map& other);
  map& operator=(const map& other);

  ~map() {
    std::cout << "DESTRUCTION OF THE MAP" << std::endl;
  }

  // Element access
  mapped_type& operator[](const key_type& key);
  mapped_type& at(const key_type& key);
  const mapped_type& at(const key_type& key) const;

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
  template <class InputIt>
  void insert(InputIt first, InputIt last);
  void erase(iterator pos);
  size_type erase(const key_type& key);
  void erase(iterator first, iterator last);
  void swap(map& other);

  // Lookup
  size_type count(const key_type& key) const;
  iterator find(const key_type& key);
  const_iterator find(const key_type& key) const;
  iterator lower_bound(const key_type& key);
  const_iterator lower_bound(const key_type& key) const;
  iterator upper_bound(const key_type& key);
  const_iterator upper_bound(const key_type& key) const;
  ft::pair<iterator, iterator> equal_range(const key_type& key);
  ft::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

  // Observers
  key_compare key_comp() const;
  value_compare value_comp() const;

  // Allocator
  allocator_type get_allocator() const;
};

// value_compare nested class
template <class Key, class T, class Compare, class Alloc>
class map<Key, T, Compare, Alloc>::value_compare {
  friend class map;
protected:
  Compare comp;
  explicit value_compare(Compare c) : comp(c) {}
public:
  bool operator()(const value_type& lhs, const value_type& rhs) const {
      return comp(lhs.first, rhs.first);
  }
};

template <class Key, class T, class Compare, class Alloc>
void swap(ft::map<Key, T, Compare, Alloc>& lhs, ft::map<Key, T, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}

} // namespace ft

#endif
