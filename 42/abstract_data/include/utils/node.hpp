#ifndef FT_NODE_HPP
#define FT_NODE_HPP

namespace ft {

template <typename T>
struct Node {
  T value;
  Node* prev;
  Node* next;
};

template <typename T>
struct SimpleNode {
  T value;
  SimpleNode* next;
};

} // namespace ft

#endif // FT_NODE_HPP
