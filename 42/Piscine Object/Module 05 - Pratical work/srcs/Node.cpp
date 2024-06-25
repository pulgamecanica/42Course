//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Node.hpp"

uint Node::GLOBAL_ID = 0;

Node::Node(): id_(GLOBAL_ID++) {}

const std::string & Node::GetName() const {
  return name_;
}

// Node& Node::operator=(const Node& node) {
//   id_ = node.GetID();
//   name_ = node.GetName();
//   neighbors_ = node.GetNeighbors();
//   return *this;
// }

void Node::SetName(const std::string & name) {
  name_ = name;
}

uint Node::GetID() const {
  return id_;
}

const std::unordered_map<uint, uint>& Node::GetNeighbors() const {
  return neighbors_;
}

void Node::SetNeighbor(int id, int weight) {
  neighbors_[id] = weight;
}