#include <iostream>
#include "deque.hpp"

int main ()
{
  ft::deque<int> mydeque;
  {
    try {
    mydeque.at(4);
    std::cout << "Poped\n";
    } catch (ft::exception &e) {
      std::cout << e.what() << std::endl;
    }
  }
  return 0;
}
