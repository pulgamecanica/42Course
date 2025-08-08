#include "associative.tpp"

void run_map_compliance_tests() {
  run_associative_container_tests<std::map<std::string, int>, std::string, int>();
  run_associative_container_tests<std::multimap<std::string, int>, std::string, int>();
}
