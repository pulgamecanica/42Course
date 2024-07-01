//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Node.hpp"

uint Node::GLOBAL_ID = 0;

Node::Node(): id_(GLOBAL_ID++) {}

const std::string & Node::GetName() const {
  return name_;
}

void Node::SetName(const std::string & name) {
  name_ = name;
}

std::string Node::GetKey() const {
  return name_;
}

const std::unordered_map<std::string, uint>& Node::GetNeighbors() const {
  return neighbors_;
}

void Node::SetNeighbor(std::string name, int weight) {
  neighbors_[name] = weight;
}