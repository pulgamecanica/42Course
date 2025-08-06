#ifndef FT_RBT_HPP
#define FT_RBT_HPP

#include <memory>
#include <functional>
#include "node.hpp"
#include "../iterators/bt_iterator.hpp"
#include "../iterators/reverse_iterator.hpp"
#include "../utils/less.hpp"
#include "../utility.hpp"

namespace ft {

template<
  class Key,
  class Value,
  class KeyOfValue,
  class Compare = ft::less<Key>,
  class Alloc = std::allocator<Value>,
  bool Multi = false
>
class RedBlackTree {
public:
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

  typedef BTNode<value_type>                      node_type;
  typedef node_type*                              node_pointer;

  typedef typename allocator_type::template rebind<node_type>::other node_allocator_type;

  typedef BTIterator<value_type, pointer, reference, key_compare>              iterator;
  typedef BTIterator<value_type, const_pointer, const_reference, key_compare>  const_iterator;
  typedef ft::reverse_iterator<iterator>                          reverse_iterator;
  typedef ft::reverse_iterator<const_iterator>                    const_reverse_iterator;

  class value_compare;

private:
  allocator_type      _alloc;
  node_allocator_type _node_alloc;
  key_compare         _comp;
  KeyOfValue          _key_of_value;
  size_type           _size;

  node_type           _super_root;
  node_pointer        _first;
  node_pointer        _last;

  node_pointer _create_node(const value_type& val) {
    node_pointer node = _node_alloc.allocate(1);
    _node_alloc.construct(node, node_type(val));
    node->left = node->right = node->parent = NULL;
    return node;
  }

  void _destroy_node(node_pointer node) {
    _node_alloc.destroy(node);
    _node_alloc.deallocate(node, 1);
  }

public:
  RedBlackTree()
    : _alloc(), _node_alloc(), _comp(), _key_of_value(), _size(0),
      _super_root(), _first(&_super_root), _last(&_super_root) {}

  explicit RedBlackTree(const key_compare& comp, const allocator_type& alloc = allocator_type())
    : _alloc(alloc), _node_alloc(), _comp(comp), _key_of_value(), _size(0),
      _super_root(), _first(&_super_root), _last(&_super_root) {}

  template<class InputIt>
  RedBlackTree(InputIt first, InputIt last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
    : _alloc(alloc), _node_alloc(), _comp(comp), _key_of_value(), _size(0),
      _super_root(), _first(&_super_root), _last(&_super_root) {
    insert(first, last);
  }

  RedBlackTree(const RedBlackTree& other)
    : _alloc(other._alloc), _node_alloc(other._node_alloc), _comp(other._comp), _key_of_value(other._key_of_value),
      _size(0), _super_root(), _first(&_super_root), _last(&_super_root) {
    *this = other;
  }

  RedBlackTree& operator=(const RedBlackTree& other) {
    if (this != &other) {
      clear();
      _alloc = other._alloc;
      _node_alloc = other._node_alloc;
      _comp = other._comp;
      _key_of_value = other._key_of_value;
      _size = other._size;
      _super_root = other._super_root; // careful: deep copy should be used
      _first = &_super_root;
      _last = &_super_root;
    }
    return *this;
  }

  ~RedBlackTree() {
    clear();
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
  bool empty() const { return size() > 0; }
  size_type size() const;
  size_type max_size() const { return _alloc.max_size(); }

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
  void _rotate_left(node_pointer x);
  void _rotate_right(node_pointer x);
  void _insert_fixup(node_pointer z);
  void _delete_fixup(node_pointer x);
  void _transplant(node_pointer u, node_pointer v);
  node_pointer _minimum(node_pointer x) const;
  node_pointer _maximum(node_pointer x) const;
  void _clear(node_pointer x);
  node_pointer _copy(node_pointer x, node_pointer parent);

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
