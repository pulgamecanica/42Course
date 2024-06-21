#ifndef __NODE_INTERFACE_HPP__
#define __NODE_INTERFACE_HPP__

#include <unordered_map>

class INode {
public:
  virtual ~INode() = default;
  virtual unsigned GetID() const = 0;
  virtual std::unordered_map<unsigned, unsigned>& GetNeighbors() = 0;
};

#endif // __NODE_INTERFACE_HPP__
