#ifndef FT_RBT_HPP
#define FT_RBT_HPP

#include <memory>
#include <functional>
#include "node.hpp"
#include "../iterators/bt_iterator.hpp"
#include "../iterators/reverse_iterator.hpp"
#include "../utils/less.hpp"
#include "../utility.hpp"

#include <sstream>
#include <iomanip> // for std::setw
#include <vector>

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

  void debug(std::string msg) {
    (void)msg;
    // std::cout << "[rbt] " << msg << std::endl;
  }

public:
  RedBlackTree()
    : _alloc(), _node_alloc(), _comp(), _key_of_value(), _size(0),
      _super_root(), _first(&_super_root), _last(&_super_root) {
    debug("Default Constructor");
  }

  explicit RedBlackTree(const key_compare& comp, const allocator_type& alloc = allocator_type())
    : _alloc(alloc), _node_alloc(), _comp(comp), _key_of_value(), _size(0),
      _super_root(), _first(&_super_root), _last(&_super_root) {
    debug("`comp`, `alloc` Constructor");

  }

  template<class InputIt>
  RedBlackTree(InputIt first, InputIt last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
    : _alloc(alloc), _node_alloc(), _comp(comp), _key_of_value(), _size(0),
      _super_root(), _first(&_super_root), _last(&_super_root) {
    insert(first, last);
    debug("Iterator Constructor");
  }

  RedBlackTree(const RedBlackTree& other)
    : _alloc(other._alloc), _node_alloc(other._node_alloc), _comp(other._comp), _key_of_value(other._key_of_value),
      _size(0), _super_root(), _first(&_super_root), _last(&_super_root) {
    *this = other;
    debug("Copy Constructor");
  }

  RedBlackTree& operator=(const RedBlackTree& other) {
    if (this != &other) {
      clear();
      _alloc = other._alloc;
      _node_alloc = other._node_alloc;
      _comp = other._comp;
      _key_of_value = other._key_of_value;
      _size = other._size;
      _super_root = other._super_root;
      _first = &_super_root;
      _last = &_super_root;
    }
    debug("Assignment Operator");
    return *this;
  }

  ~RedBlackTree() {
    clear();
    debug("Destructor");
  }

  // Iterators
  iterator begin() { return iterator(_first); }
  const_iterator begin() const { return const_iterator(_first); }
  iterator end() { return iterator(&_super_root); }
  const_iterator end() const { return const_iterator(&_super_root); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // Capacity
  bool empty() const { return _size == 0; }
  size_type size() const { return _size; }
  size_type max_size() const { return _node_alloc.max_size(); }

  // Modifiers
  void clear() {
    _clear(_super_root.left); // Recursively delete from root
    _super_root.left = NULL;
    _first = &_super_root;
    _last = &_super_root;
    _size = 0;
    debug("Clear");
  }

  ft::pair<iterator, bool> insert(const value_type& val) {
    debug("insert(val)");
    key_type key = _key_of_value(val);
    node_pointer parent = &_super_root;
    node_pointer* link = &_super_root.left;

    while (*link != NULL) {
      parent = *link;
      if (_less_keys(key, _key_of_value(parent->data)))
        link = &parent->left;
      else if (_less_keys(_key_of_value(parent->data), key))
        link = &parent->right;
      else {
        if (Multi)
          link = &parent->right;
        else
          return ft::make_pair(iterator(parent), false);
      }
    }

    node_pointer new_node = _create_node(val);
    new_node->parent = parent;
    *link = new_node;
    ++_size;

    _insert_fixup(new_node);

    if (_first == &_super_root || _less_keys(_key_of_value(new_node->data), _key_of_value(_first->data)))
      _first = new_node;
    if (_last == &_super_root || _less_keys(_key_of_value(_last->data), _key_of_value(new_node->data))) {
      node_pointer succ = new_node->successor();
      _last = succ ? succ : &_super_root; // ← this line is the fix
    }

    // if (_last == &_super_root || _less_keys(_key_of_value(_last->data), _key_of_value(new_node->data)))
    //   _last = new_node->successor();

    return ft::make_pair(iterator(new_node), true);
  }

  iterator insert(iterator hint, const value_type& val) {
    debug("insert(hint, val)");
    if (hint == end()) return insert(val).first;

    node_pointer current = hint._ptr;
    node_pointer next = current ? current->successor() : NULL;

    if (_less_keys(_key_of_value(current->getData()), _key_of_value(val)) &&
      (!next || _less_keys(_key_of_value(val), _key_of_value(next->getData())))) {
      // val fits between current and next
      node_pointer inserted = _insert_node_near(current, val);
      return iterator(inserted);
    }
    return insert(val).first;
  }

  template<class InputIt>
	void insert(InputIt first, InputIt last) {
		iterator tmp = begin();
		while (first != last) {
			tmp = insert(tmp, (value_type)*first);
			first++;
		}
	}
  
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

  /**
   * Inserts a new node with the given value near a known node (hint).
   * This is used by insert(hint, value) to optimize insertion when the
   * position is known or approximately known. The function places the new
   * node as a leftmost or rightmost child relative to `near`, while
   * maintaining red-black tree invariants.
   *
   * Assumes `near` is not null and the key comparison has already been validated
   * by the caller (i.e., this does not check if insertion is valid).
   *
   * After linking, the tree is rebalanced via _insert_fixup.
   */
  node_pointer _insert_node_near(node_pointer near, const value_type& val) {
    node_pointer z = _create_node(val);
    node_pointer parent = near;
    bool go_left = _less_keys(_key_of_value(val), _key_of_value(near->getData()));

    // Go left or right depending on key
    if (go_left) {
      while (parent->left)
        parent = parent->left;
      parent->left = z;
    } else {
      while (parent->right)
        parent = parent->right;
      parent->right = z;
    }

    z->parent = parent;

    // Fix tree structure
    _insert_fixup(z);
    _size++;

    // Update _first and _last
    if (_first == &_super_root || _less_keys(_key_of_value(z->getData()), _key_of_value(_first->getData())))
      _first = z;
    if (_last == &_super_root || _less_keys(_key_of_value(_last->getData()), _key_of_value(z->getData())))
      _last = z->successor();

    return z;
  }

  /**
   * Performs a left rotation around node x.
   * Right child y takes x's place and x becomes y's left child.
   *
   *        x                 y
   *       / \               / \
   *      a   y     =>      x   c
   *         / \           / \
   *        b   c         a   b
   */
  void _rotate_left(node_pointer x) {
    node_pointer y = x->right;
    x->right = y->left;
    if (y->left)
      y->left->parent = x;
    y->parent = x->parent;
    if (x == _super_root.left)
      _super_root.left = y;
    else if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
    y->left = x;
    x->parent = y;
  }

  /**
   * Performs a right rotation around node x.
   * Left child y takes x's place and x becomes y's right child.
   *
   *        x                 y
   *       / \               / \
   *      y   c     =>      a   x
   *     / \                   / \
   *    a   b                 b   c
   */
  void _rotate_right(node_pointer x) {
    node_pointer y = x->left;
    x->left = y->right;
    if (y->right)
      y->right->parent = x;
    y->parent = x->parent;
    if (x == _super_root.left)
      _super_root.left = y;
    else if (x == x->parent->right)
      x->parent->right = y;
    else
      x->parent->left = y;
    y->right = x;
    x->parent = y;
  }

  void _delete_fixup(node_pointer x);
  void _transplant(node_pointer u, node_pointer v);
  node_pointer _minimum(node_pointer x) const;
  node_pointer _maximum(node_pointer x) const;
  
  // Clear recursive
  void _clear(node_pointer node) {
    if (!node)
      return;
    _clear(node->left);
    _clear(node->right);
    _destroy_node(node);
  }

  node_pointer _copy(node_pointer x, node_pointer parent);

  // Internal key comparison helpers
  bool _equal_keys(const key_type& a, const key_type& b) const {
    return !_comp(a, b) && !_comp(b, a);
  }

  /**
   * Compares two keys and returns true if a < b
   * Uses the key comparator provided by the template parameter
   */
  bool _less_keys(const key_type& a, const key_type& b) const {
    return _comp(a, b);
  }

  /**
   * https://kubokovac.eu/gnarley-trees/Redblack.html
   * 
   * Fixes up the red-black properties after insertion
   * Performs rotations and recoloring to ensure:
   * 1. Every node is either red or black
   * 2. The root is always black
   * 3. Red nodes cannot have red children (no two red nodes in a row)
   * 4. Every path from a node to NULL has the same number of black nodes
   */
  void _insert_fixup(node_pointer z) {
    while (z->parent && z->parent->color == B_RED) {
      node_pointer gp = z->parent->parent;
      if (!gp)
        break;
      if (z->parent == gp->left) {
        node_pointer y = gp->right; // Uncle
        if (y && y->color == B_RED) {
          // Case 1: Uncle is red -> recolor
          z->parent->color = B_BLACK;
          y->color = B_BLACK;
          gp->color = B_RED;
          z = gp;
        } else {
          if (z == z->parent->right) {
            // Case 2: z is right child -> rotate left
            z = z->parent;
            _rotate_left(z);
          }
          // Case 3: z is left child -> rotate right
          z->parent->color = B_BLACK;
          gp->color = B_RED;
          _rotate_right(gp);
        }
      } else {
        node_pointer y = gp->left; // Uncle
        if (y && y->color == B_RED) {
          // Case 1: Uncle is red -> recolor
          z->parent->color = B_BLACK;
          y->color = B_BLACK;
          gp->color = B_RED;
          z = gp;
        } else {
          if (z == z->parent->left) {
            // Case 2: z is left child -> rotate right
            z = z->parent;
            _rotate_right(z);
          }
          // Case 3: z is right child -> rotate left
          z->parent->color = B_BLACK;
          gp->color = B_RED;
          _rotate_left(gp);
        }
      }
    }
    _super_root.left->color = B_BLACK; // Root is always black
  }

public:
  int _compute_tree_width(node_pointer node) const {
    if (!node) return 0;
    return std::max(1, _compute_tree_width(node->left) + _compute_tree_width(node->right));
  }

  int _build_tree_display(node_pointer node, int depth, int x, std::vector<std::string>& canvas, int spacing) const {
    if (!node) return 0;

    std::ostringstream label;
    label << (node->color == B_RED ? "R:" : "B:") << _key_of_value(node->getData());
    std::string lbl = label.str();

    if ((int)canvas.size() <= depth)
      canvas.resize(depth + 1, std::string(512, ' '));

    int curr_x = x;
    if (curr_x + (int)lbl.size() >= (int)canvas[depth].size())
      canvas[depth].resize(curr_x + lbl.size() + 1, ' ');

    // Write label
    for (size_t i = 0; i < lbl.size(); ++i)
      canvas[depth][curr_x + i] = lbl[i];

    int left_size = _build_tree_display(node->left, depth + 2, x - spacing, canvas, spacing / 2);
    int right_size = _build_tree_display(node->right, depth + 2, x + spacing, canvas, spacing / 2);

    // Draw branches
    if (node->left) {
      canvas[depth + 1][x - spacing / 2] = '/';
    }
    if (node->right) {
      canvas[depth + 1][x + spacing / 2] = '\\';
    }

    return 1;
  }

  void print_tree_top_down() const {
    std::vector<std::string> canvas;
    int spacing = 42;
    int root_x = 140;

    _build_tree_display(_super_root.left, 0, root_x, canvas, spacing);

    std::cout << "\n[ Tree Top-Down View ]\n";
    for (size_t i = 0; i < canvas.size(); ++i) {
      std::cout << canvas[i] << '\n';
    }
    std::cout << std::endl;
  }

  void print_metadata() {
    std::cout << "First: ";
    if (_first && _first != &_super_root) std::cout << _first->data.first;
    else std::cout << "null";

    std::cout << ", Last: ";
    if (_last && _last != &_super_root) std::cout << _last->data.first;
    else std::cout << "end()";

    std::cout << ", _last->predecessor(): ";
    if (_last != &_super_root && _last->predecessor())
        std::cout << _last->predecessor()->data.first;
    else
        std::cout << "none";

    std::cout << std::endl;
  }
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
