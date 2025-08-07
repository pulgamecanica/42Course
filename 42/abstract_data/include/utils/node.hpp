#ifndef FT_NODE_HPP
#define FT_NODE_HPP

# include "algorithm.hpp"

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

#define B_RED false
#define B_BLACK true

template <typename T>
class BTNode {
public:
  typedef T         value_type;
  typedef T&        reference;
  typedef const T&  const_reference;

  BTNode* parent;
  BTNode* left;
  BTNode* right;
  value_type data;
  bool color;

  explicit BTNode(const value_type& val = value_type(), bool c = B_RED)
    : parent(0), left(0), right(0), data(val), color(c) {}

  reference getData() { return data; }
  const_reference getData() const { return data; }

  static bool isRed(const BTNode* node) {
    return node && node->color == B_RED;
  }

  BTNode* min() {
    BTNode* node = this;
    while (node->left)
      node = node->left;
    return node;
  }

  BTNode* max() {
    BTNode* node = this;
    while (node->right)
      node = node->right;
    return node;
  }

  BTNode* successor() {
    BTNode* node = this;
    if (node->right) {
      node = node->right;
      while (node->left)
        node = node->left;
      return node;
    }
    while (node->parent && node == node->parent->right)
      node = node->parent;
    return node->parent;
  }

  BTNode* predecessor() {
    BTNode* node = this;
    if (node->left) {
      node = node->left;
      while (node->right)
        node = node->right;
      return node;
    }
    while (node->parent && node == node->parent->left)
      node = node->parent;
    return node->parent;
  }
};

} // namespace ft

#endif // FT_NODE_HPP
