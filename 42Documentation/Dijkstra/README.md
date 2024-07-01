
---

# Dijkstra Graph Library

A C++ library to perform Dijkstra's algorithm for finding the shortest path between nodes in a graph.

## Table of Contents

- [Dijkstra Graph Library](#dijkstra-graph-library)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Dependencies](#dependencies)
  - [Usage](#usage)
    - [Library Usage](#library-usage)
    - [Example](#example)
  - [Creating Custom Nodes](#creating-custom-nodes)
  - [Contributing](#contributing)
  - [License](#license)

## Features

- Implements Dijkstra's algorithm to find the shortest path between two nodes.
- Allows the use of custom node types by implementing an interface.
- Supports bidirectional edges with weights.

## Dependencies

- A C++23 compliant compiler (e.g., g++ or clang++)
- Make (for building the project)

## Usage

### Library Usage

Include the library header and link the static library in your project.

1. Create a `main.cpp` file:
```cpp
#include <iostream>
#include "Graph.hpp"

int main() {
    Graph<int> graph;
    INode<int>* node1 = new IntNode(1);
    INode<int>* node2 = new IntNode(2);

    graph.AddEdge(node1, node2, 10);

    PathInfo<int> path = graph.Dijkstra(1, 2);

    for (const auto& [node, distance] : path.path) {
        std::cout << "Node: " << node.GetKey() << ", Distance: " << distance << std::endl;
    }

    delete node1;
    delete node2;
    return 0;
}
```

2. Compile and run the example:
```sh
g++ -Wall -Werror -Wextra -I. -o test main.cpp
./test
```

### Example

Here is an example `main.cpp` that demonstrates how to use the library with custom nodes:

```cpp
#include <iostream>
#include "Graph.hpp"

class IntNode : public INode<int> {
public:
    IntNode(int id) : id_(id) {}
    int GetKey() const override { return id_; }
    void SetNeighbor(int id, int weight) override { neighbors_[id] = weight; }
    const std::unordered_map<int, unsigned>& GetNeighbors() const override { return neighbors_; }

private:
    int id_;
    std::unordered_map<int, unsigned> neighbors_;
};

class StrNode : public INode<std::string> {
public:
    StrNode(std::string name) : name_(name) {}
    std::string GetKey() const override { return name_; }
    void SetNeighbor(std::string name, int weight) override { neighbors_[name] = weight; }
    const std::unordered_map<std::string, unsigned>& GetNeighbors() const override { return neighbors_; }

private:
    std::string name_;
    std::unordered_map<std::string, unsigned> neighbors_;
};

int main() {
    {
        Graph<std::string> graph;
        
        INode<std::string>* jun = new StrNode("Jungapeo");
        INode<std::string>* zit = new StrNode("Zitacuaro");
        INode<std::string>* mor = new StrNode("Morelia");
        INode<std::string>* cdmx = new StrNode("CDMX");
        INode<std::string>* jal = new StrNode("Jalisco");
        INode<std::string>* p_e = new StrNode("Puerto Escondido");
        INode<std::string>* oax = new StrNode("Oaxaca");
        INode<std::string>* zic = new StrNode("Zicatela");
        INode<std::string>* tux = new StrNode("Tuxpan");
        INode<std::string>* tol = new StrNode("Toluca");
        INode<std::string>* lerma = new StrNode("Lerma");
        INode<std::string>* cuau = new StrNode("Cuautla");

        graph.AddEdge(jun, zit, 26.8);
        graph.AddEdge(jun, mor, 150);
        graph.AddEdge(zit, mor, 158);
        graph.AddEdge(zit, cdmx, 142);
        graph.AddEdge(cdmx, mor, 294);
        graph.AddEdge(cdmx, jal, 540);
        graph.AddEdge(jal, p_e, 880);
        graph.AddEdge(p_e, oax, 257);
        graph.AddEdge(oax, zic, 11);
        graph.AddEdge(oax, tux, 245);
        graph.AddEdge(oax, tol, 376);
        graph.AddEdge(tol, lerma, 20);
        graph.AddEdge(tol, cuau, 145);
        graph.AddEdge(tol, cdmx, 65);
        graph.AddEdge(oax, cdmx, 462);

        for (auto& elem: graph.GetNodes()) {
            INode<std::string>& node = *elem.second;
            std::cout << "Node: " << node.GetKey() << std::endl;
        }

        std::cout << std::endl;
        PathInfo<std::string> path_cdmx_p_e = graph.Dijkstra("CDMX", "Puerto Escondido");
        PathInfo<std::string> path_jun_cuau = graph.Dijkstra("Jungapeo", "Cuautla");

        std::cout << "Trip: CDMX <-> Puerto Escondido" << std::endl;
        for (const auto& node : path_cdmx_p_e.path) {
            std::cout << node.first.GetKey() << " (Distance: " << node.second << ") ";
        }
        std::cout << std::endl << "Total Distance: " << path_cdmx_p_e.TotalDistance() << std::endl;    
        std::cout << std::endl;
        std::cout << "Trip: Jungapeo <-> Cuautla" << std::endl;
        for (const auto& node : path_jun_cuau.path) {
            std::cout << node.first.GetKey() << " (Distance: " << node.second << ") ";
        }
        std::cout << std::endl << "Total Distance: " << path_jun_cuau.TotalDistance() << std::endl << std::endl;

        // Clean up dynamically allocated nodes
        delete jun;
        delete zit;
        delete mor;
        delete cdmx;
        delete jal;
        delete p_e;
        delete oax;
        delete zic;
        delete tux;
        delete tol;
        delete lerma;
        delete cuau;
    }

    {
        Graph<int> graph;
        INode<int>* node1 = new IntNode(1);
        INode<int>* node2 = new IntNode(2);
        INode<int>* node3 = new IntNode(3);
        INode<int>* node4 = new IntNode(4);
        INode<int>* node5 = new IntNode(5);
        
        graph.AddEdge(node1, node2, 2);
        graph.AddEdge(node1, node4, 10);
        graph.AddEdge(node2, node3, 3);
        graph.AddEdge(node2, node4, 8);
        graph.AddEdge(node3, node5, 6);
        graph.AddEdge(node3, node4, 2);

        PathInfo<int> path = graph.Dijkstra(1, 4);
        
        for (const auto& node : path.path) {
            std::cout << node.first.GetKey() << " (Distance: " << node.second << ") ";
        }
        std::cout << std::endl << "Total Distance: " << path.TotalDistance() << std::endl;
        
        // Clean up dynamically allocated nodes
        delete node1;
        delete node2;
        delete node3;
        delete node4;
        delete node5;
    }

    return 0;
}
```

To compile and run this example, use the provided Makefile target:

```sh
g++ -Wall -Werror -Wextra -I. -o test main.cpp
./test
```

## Creating Custom Nodes

To create your own custom nodes, you need to implement the `INode` interface. Here is a step-by-step guide:

1. **Define Your Custom Node Class**:
```cpp
#include <iostream>
#include "Graph.hpp"

// Implement the INode with the type you prfer (I choose <int> in this case)
class IntNode : public INode<int> {
public:
    IntNode(int id) : id_(id) {} // Constructor
    int GetKey() const override { return id_; } // * Key, which must return the same type <int>
    void SetNeighbor(int id, int weight) override { neighbors_[id] = weight; } // * Set the neighbor with the Key you choose <int> in this case
    const std::unordered_map<int, unsigned>& GetNeighbors() const override { return neighbors_; } //  * Return a map with the neighbors, identified with the key

private:
    int id_;
    std::unordered_map<int, unsigned> neighbors_;
};

```

2. **Use Your Custom Node in the Graph**:
```cpp
Graph graph;
INode* node1 = new INode(1);
INode* node2 = new INode(2);

graph.AddEdge(node1, node2, 10);

PathInfo path = graph.Dijkstra(1, 2);

for (const auto& [node, distance] : path.path) {
   std::cout << "Node: " << node << ", Distance: " << distance << std::endl;
}

delete node1;
delete node2;
```

## Contributing

Contributions are welcome! Please submit a pull request or open an issue to discuss any changes.
