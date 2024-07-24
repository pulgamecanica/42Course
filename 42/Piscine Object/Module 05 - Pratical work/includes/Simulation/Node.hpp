#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include "INode.hpp" // From Dijkstra's Lib
#include <raylib.h>

class Node: public INode<std::string> {
public:
  Node(); // Change this
  std::string GetKey() const override;
  void SetNeighbor(std::string, int weight) override;
  const std::unordered_map<std::string, uint>& GetNeighbors() const override;
  const std::string & GetName() const;
  const Vector2 & GetPosition() const;
  const Color & GetColor() const;
  void SetName(const std::string & name);
  void SetPosition(Vector2 new_pos);
  void SetColor(Color new_color);
private:
  uint id_;
  std::string name_;
  std::unordered_map<std::string, uint> neighbors_;
  Vector2 pos_;
  Color   color_;
  static uint GLOBAL_ID;
};

#endif  // NODE_HPP