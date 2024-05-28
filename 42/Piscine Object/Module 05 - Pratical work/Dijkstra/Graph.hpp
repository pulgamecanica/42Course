//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __GRAPH_HPP__
# define __GRAPH_HPP__

#include <list>
#include <vector>
#include <iostream>

class Connection {
public:
  Connection(int dest_node, int weight);
  Connection(const Connection &) = default;
  Connection& operator=(const Connection &) = default;
  int Weight() const;
  int Dest() const;
private:
  int dest_node_;  // Node to which it connects
  int weight_;   // Distance to the vercice
};

bool operator>(const Connection& lhs, const Connection& rhs);


class Graph {
public:
  Graph(int size);
  ~Graph();
  int GetSize() const;
  void AddEdge(int first, int second, int weight);
  std::vector<int> ShortestPaths(int vertice);
  std::list<Connection>& operator[](int node);
private:
  int size_;
  std::list<Connection> *vertices_;
};
#endif
