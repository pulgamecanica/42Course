
---

# Dijkstra Graph Library

A C++ library to perform Dijkstra's algorithm for finding the shortest path between nodes in a graph.

## Table of Contents

- [Dijkstra Graph Library](#dijkstra-graph-library)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Dependencies](#dependencies)
  - [Installation](#installation)
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

## Installation

1. Clone the repository:
```sh
git clone https://github.com/yourusername/dijkstra-graph-library.git
cd dijkstra-graph-library
```

2. Build the library:
   ```sh
   make
   ```

3. (Optional) Clean the build artifacts:
   ```sh
   make clean
   ```

4. (Optional) Clean all generated files:
   ```sh
   make fclean
   ```

5. (Optional) Rebuild the project:
   ```sh
   make re
   ```

## Usage

### Library Usage

Include the library header and link the static library in your project.

1. Create a `main.cpp` file:
```cpp
#include <iostream>
#include "Graph.hpp"

int main() {
   Graph graph;
   INode* node1 = new YourNode(1, "Node1", ... whatever attributes you add);
   INode* node2 = new YourNode(2, "Node2", ... whatever attributes you add);

   graph.AddEdge(node1, node2, 10);

   PathInfo path = graph.Dijkstra(1, 2);

   for (const auto& [node, distance] : path.path) {
       std::cout << "Node: " << node << ", Distance: " << distance << std::endl;
   }

   delete node1;
   delete node2;
   return 0;
}
```

2. Compile and run the example:
```sh
make test
./test
```

### Example

Here is an example `main.cpp` that demonstrates how to use the library:

```cpp
#include <iostream>
#include "Graph.hpp"

class CustomNode : public INode {
public:
    CustomNode(uint id, std::string name) : id(id), name(std::move(name)) {}

    uint GetID() const override {
        return id;
    }

    std::unordered_map<uint, uint>& GetNeighbors() override {
        return neighbors;
    }

    std::string GetName() const {
        return name;
    }

private:
    uint id;
    std::string name;
    std::unordered_map<uint, uint> neighbors;
};

int main() {
    Graph graph;
    INode* node1 = new CustomNode(1, "Node1");
    INode* node2 = new CustomNode(2, "Node2");
    INode* node3 = new CustomNode(3, "Node3");

    graph.AddEdge(node1, node2, 10);
    graph.AddEdge(node2, node3, 5);
    graph.AddEdge(node1, node3, 15);

    PathInfo path = graph.Dijkstra(1, 3);

    for (const auto& [node, distance] : path.path) {
        std::cout << "Node: " << node << ", Distance: " << distance << std::endl;
    }

    delete node1;
    delete node2;
    delete node3;

    return 0;
}
```

> Note:
>
> The `std::unordered_map<uint, uint> neighbors;` refers to: `<neighbor, weight>`
>

To compile and run this example, use the provided Makefile target:

```sh
make test
./test
```

## Creating Custom Nodes

To create your own custom nodes, you need to implement the `INode` interface. Here is a step-by-step guide:

1. **Define Your Custom Node Class**:
```cpp
#include "NodeInterface.hpp"
#include <string>
#include <unordered_map>

class CustomNode : public INode {
public:
   CustomNode(uint id, std::string name) : id(id), name(std::move(name)) {}

   uint GetID() const override {
       return id;
   }

   std::unordered_map<uint, uint>& GetNeighbors() override {
       return neighbors;
   }

   std::string GetName() const {
       return name;
   }

private:
   uint id;
   std::string name;
   std::unordered_map<uint, uint> neighbors;
};
```

2. **Use Your Custom Node in the Graph**:
```cpp
Graph graph;
INode* node1 = new CustomNode(1, "Node1");
INode* node2 = new CustomNode(2, "Node2");

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

