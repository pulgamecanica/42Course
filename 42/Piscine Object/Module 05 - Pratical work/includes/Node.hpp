#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include "INode.hpp" // From Dijkstra's Lib

class Node: public INode {
public:
  Node();
  // Node& operator=(const Node&) = default;
  uint GetID() const override;
  const std::unordered_map<uint, uint>& GetNeighbors() const override;
  void SetNeighbor(int id, int weight) override;
  const std::string & GetName() const;
  void SetName(const std::string & name);
private:
  uint id_;
  std::string name_;
  std::unordered_map<uint, uint> neighbors_;
  static uint GLOBAL_ID;
};

#endif  // NODE_HPP