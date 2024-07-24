//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Simulation/Node.hpp"
#include <cmath>

uint Node::GLOBAL_ID = 0;

Node::Node(): id_(GLOBAL_ID++), pos_(rand() % GetScreenWidth(), rand() % GetScreenHeight()), color_(PINK) {}

const std::string & Node::GetName() const {
  return name_;
}

const Vector2 & Node::GetPosition() const {
  return pos_;
}
const Color & Node::GetColor() const {
  return color_;
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

void Node::SetPosition(Vector2 new_pos) {
  pos_ = new_pos;
}

void Node::SetColor(Color new_color) {
  color_ = new_color;
}
