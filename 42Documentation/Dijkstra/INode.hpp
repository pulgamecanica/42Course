#ifndef __NODE_INTERFACE_HPP__
#define __NODE_INTERFACE_HPP__

#include <unordered_map>

template <typename KeyType>
class INode {
public:
  virtual ~INode() = default;
  virtual KeyType GetKey() const = 0;
  virtual void SetNeighbor(KeyType key, int weight) = 0;
  virtual const std::unordered_map<KeyType, unsigned>& GetNeighbors() const = 0;
};

#endif // __NODE_INTERFACE_HPP__
