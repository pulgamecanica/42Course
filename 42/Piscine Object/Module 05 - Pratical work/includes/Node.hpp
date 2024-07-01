#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include "INode.hpp" // From Dijkstra's Lib

class Node: public INode<std::string> {
public:
  Node(); // Change this
  std::string GetKey() const override;
  void SetNeighbor(std::string, int weight) override;
  const std::unordered_map<std::string, uint>& GetNeighbors() const override;
  const std::string & GetName() const;
  void SetName(const std::string & name);
private:
  uint id_;
  std::string name_;
  std::unordered_map<std::string, uint> neighbors_;
  static uint GLOBAL_ID;
};

#endif  // NODE_HPP