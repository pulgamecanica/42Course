#ifndef FT_NODE_HPP
#define FT_NODE_HPP

namespace ft {

template <typename T>
struct Node {
  T value;
  Node* prev;
  Node* next;
  
  Node(const T& val = T()) : value(val), prev(NULL), next(NULL) {}
};

template <typename T>
struct SimpleNode {
  T value;
  SimpleNode* next;

  SimpleNode(const T& val = T()) : value(val), next(NULL) {}
};

} // namespace ft

#endif // FT_NODE_HPP
