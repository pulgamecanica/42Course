#ifndef FT_NODE_HPP
#define FT_NODE_HPP

# include "algorithm.hpp"

# define B_RED false
# define B_BLACK true

namespace ft {

template <typename T>
struct Node {
  T value;
  Node* prev;
  Node* next;
  
  Node(const T& val = T()) : value(val), prev(0), next(0) {}
};

template <typename T>
struct SimpleNode {
  T value;
  SimpleNode* next;

  SimpleNode(const T& val = T()) : value(val), next(0) {}
};

template <typename T, class C>
class BTNode {
public:
  typedef T         value_type;
  typedef T&        value_reference;
  typedef const T&  const_reference;
  typedef T*        node_pointer;
  typedef const T*  const_node_pointer;
  typedef C         key_compare;
  
  explicit BTNode(value_type const& val = value_type(), bool color = B_RED)
    : data(val), parent(NULL), left(NULL), right(NULL), color(color) {}
  
  BTNode(const BTNode& node)
    : data(node.data), parent(node.parent), left(node.left), right(node.right), color(B_RED) {}

  BTNode& operator=(const BTNode& x) {
    if (this != &x) {
      this->data = x.data;
      this->parent = x.parent;
      this->left = x.left;
      this->right = x.right;
      this->color = x.color;
    }
    return *this;
  }

  virtual ~BTNode() {}

  value_reference getData() { return (data); }

  bool operator==(const BTNode& other) { return (this->data == other.data); }
  bool operator!=(const BTNode& other) { return (this->data != other.data); }
  bool operator>(const BTNode& other)  { return (this->data > other.data); }
  bool operator<(const BTNode& other)  { return (this->data < other.data); }
  bool operator<=(const BTNode& other) { return (this->data <= other.data); }
  bool operator>=(const BTNode& other) { return (this->data >= other.data); }

  // static T getData(BTNode *node) {
  //     return (static_cast<BTNode<T>*>(node)->data);
  // }

  bool isRed(BTNode x) {
    if (x == NULL)
      return false;
    return x.color == B_RED;
  }
  
  size_t size() const {
    size_t size(1);
    if (this->left && this->left->parent)
      size += this->left->size();
    if (this->right && this->right->parent)
      size += this->right->size();
    return (size);
  }

  size_t height() const {
    if (!this->parent)
      return (1);
    return (ft::max(this->left->height(), this->right->height()) + 1);
  }

  size_t bheight() const {
    if (!this->parent)
      return (1);
    size_t height = (this->color == B_BLACK) ? 1 : 0;
    height = ft::max(height, this->left->bheight());
    height = ft::max(height, this->right->bheight());
    return (height);
  }
  
  /* Travell left */
  BTNode *min() {
      BTNode *node = this;

      while (node->left && node->left->parent)
          node = node->left;
      return (node);
  }

  /* Travell right */
  BTNode *max() {
      BTNode *node = this;

      while (node->right && node->right->parent)
          node = node->right;
      return (node);
  }

  /*
  * Get the next smaller number
  * - If the node is a red black tree child, then:
  *   - For the right node, the successor is the first parent.
  *   - For the middle node, the the successor is the second parent.
  *   - For the left node, the successor is the next min node.
  * - If the node is not a red black tree the next successor is just the next min.
  */
  BTNode *successor() {
    BTNode* node = this;

    if (node && node->right && node->right->parent)
      return (node->right->min());
    while (node && node->parent && node->parent->parent && node == node->parent->right)
      node = node->parent;
    return (node->parent);
  }
  /*
  * Get the next bigger number
  * - If the node is a red black tree child, then:
  *   - For the left node, the successor is the first parent.
  *   - For the middle node, the the successor is the second parent.
  *   - For the right node, the successor is the next max node.
  * - If the node is not a red black tree the next successor is just the next max.
  */
  BTNode *predecessor() {
    BTNode* node = this;

    if (node && node->left && node->left->parent)
      return (node->left->max());
    while (node && node->parent && node->parent->parent && node == node->parent->left)
      node = node->parent;
    return (node->parent);
  }

  T data;
  BTNode *parent, *left, *right;
  bool color;
};

} // namespace ft

#endif // FT_NODE_HPP
