//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Graph.hpp"

bool ReadInputFile(const std::string& file_path, int& nodes, int& edges, 
                   std::vector<std::pair<int, int>>& edge_list, 
                   std::pair<int, int>& query) {
  std::ifstream input_file(file_path);
  if (!input_file.is_open()) {
    std::cerr << "Error: Cannot open file " << file_path << "\n";
    return false;
  }

  std::string line;
  if (!std::getline(input_file, line)) {
    std::cerr << "Error: Failed to read the first line\n";
    return false;
  }

  std::istringstream iss(line);
  if (!(iss >> nodes >> edges)) {
    std::cerr << "Error: Invalid format for the first line\n";
    return false;
  }

  for (int i = 0; i < edges; ++i) {
    if (!std::getline(input_file, line)) {
      std::cerr << "Error: Failed to read edge line\n";
      return false;
    }

    int from, to;
    std::istringstream edge_iss(line);
    if (!(edge_iss >> from >> to)) {
      std::cerr << "Error: Invalid format for edge line\n";
      return false;
    }
    edge_list.emplace_back(from, to);
  }

  if (!std::getline(input_file, line)) {
    std::cerr << "Error: Failed to read the query line\n";
    return false;
  }

  std::istringstream query_iss(line);
  if (!(query_iss >> query.first >> query.second)) {
    std::cerr << "Error: Invalid format for query line\n";
    return false;
  }

  return true;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  std::string file_path = argv[1];
  int nodes, edges;
  std::vector<std::pair<int, int>> edge_list;
  std::pair<int, int> query;

  if (!ReadInputFile(file_path, nodes, edges, edge_list, query)) {
    return 1;
  }

  Graph graph(nodes);
  for (const auto& edge : edge_list) {
    if (!graph.AddEdge(edge.first, edge.second)) {
      std::cerr << "Error: Failed to add edge (" << edge.first << ", " << edge.second << ")\n";
      return 1;
    }
  }

  if (graph.IsReachable(query.first, query.second)) {
    std::cout << "1\n";
  } else {
    std::cout << "0\n";
  }

  return 0;
}
