#ifndef __NODE_INTERFACE_HPP__
#define __NODE_INTERFACE_HPP__

#include <unordered_map>

class INode {
public:
  virtual ~INode() = default;
  virtual unsigned GetID() const = 0;
  virtual void SetNeighbor(int id, int weight) = 0;
  virtual const std::unordered_map<unsigned, unsigned>& GetNeighbors() const = 0;
};

#endif // __NODE_INTERFACE_HPP__
