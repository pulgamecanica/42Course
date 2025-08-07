#include <iostream>
#include <string>
#include <cstdlib>  // for std::rand, std::srand
#include <ctime>    // for std::time
#include "map.hpp"
#include <map>

#define ns ft

int main() {
  std::srand(static_cast<unsigned int>(std::time(NULL))); // Seed randomness
  { // insert with iterator
    // std::cout << "Map Insert Test (Range)" << std::endl;

    // std::vector<ns::pair<int, std::string> > vec;
    // vec.push_back(ns::make_pair(10, "ten"));
    // vec.push_back(ns::make_pair(20, "twenty"));
    // vec.push_back(ns::make_pair(15, "fifteen"));
    // vec.push_back(ns::make_pair(25, "twenty-five"));
    // vec.push_back(ns::make_pair(5, "five"));

    // ns::map<int, std::string> m1;
    // m1.insert(vec.begin(), vec.end());

    // std::cout << "Inserted " << vec.size() << " entries using range insert." << std::endl;
    // std::cout << "Map size: " << m1.size() << std::endl;

    // std::cout << "Iterating over sorted keys:" << std::endl;
    // for (ns::map<int, std::string>::iterator it = m1.begin(); it != m1.end(); ++it) {
    //     std::cout << it->first << " => " << it->second << std::endl;
    // }

    // // m1.print(); // optional: visualize structure if available
    // return 0;
  }
  { // insert with hint
    // std::cout << "\nMap Hint Insert Test (Ascending Keys)\n" << std::endl;
    // ns::map<int, std::string> m2;

    // const int N = 50;
    // ns::map<int, std::string>::iterator hint = m2.begin();

    // for (int i = 0; i < N; ++i) {
    //   int key = i * 2; // strictly ascending
    //   std::string value = "hinted_" + std::to_string(key);
    //   hint = m2.insert(hint, ns::make_pair(key, value)); // insert with hint
    // }

    // std::cout << "Inserted " << N << " entries using iterator hint." << std::endl;
    // std::cout << "Map size: " << m2.size() << std::endl;

    // for (ns::map<int, std::string>::iterator it = m2.begin(); it != m2.end(); ++it)
    //   std::cout << it->first << " => " << it->second << std::endl;
    // m2.print();
  }

  // { // Test insert(val)
  //   ns::map<int, std::string> m1;

  //   std::cout << "Map Insert Test (Randomized)" << std::endl;

  //   const int N = 10; // Number of nodes to insert

  //   for (int i = 0; i < N; ++i) {
  //       int key = std::rand() % (N * 10); // Allow some duplicates if multi
  //       std::string value = "val_" + std::to_string(key);
  //       m1.insert(ns::make_pair(key, value));
  //   }

  //   std::cout << "Inserted " << N << " random entries." << std::endl;
  //   std::cout << "Map size: " << m1.size() << std::endl;

  //   std::cout << "Iterating over sorted keys:" << std::endl;
  //   int count = 0;
  //   for (ns::map<int, std::string>::iterator it = m1.begin(); it != m1.end(); ++it) {
  //       if (count++ < 20)  // Print only the first 20 to avoid clutter
  //           std::cout << it->first << " => " << it->second << std::endl;
  //       else if (count == 21)
  //           std::cout << "... (remaining " << m1.size() - 20 << " entries not shown)" << std::endl;
  //   }

  //   m1.print();
  // }
  return 0;
}


// #include <iostream>
// #include <string>
// #include "list.hpp"

// template <typename List>
// void debug_reverse_print(const List& lst) {
//     typename List::const_reverse_iterator rit = lst.rbegin();
//     typename List::const_reverse_iterator rend = lst.rend();
//     std::size_t count = 0;

//     std::cout << "=== REVERSE ITERATION ===\n";
//     while (rit != rend) {
//         std::cout << "[" << *rit << "]\n";
//         ++rit;
//         if (++count > lst.size() + 5) {
//             std::cerr << "ERROR: reverse iteration may be infinite!\n";
//             break;
//         }
//     }
//     std::cout << "=== END REVERSE ===\n";
// }

// template <typename List>
// void debug_forward_print(const List& lst) {
//     std::cout << "=== FORWARD ITERATION ===\n";
//     for (typename List::const_iterator it = lst.begin(); it != lst.end(); ++it) {
//         std::cout << "[" << *it << "]\n";
//     }
//     std::cout << "=== END FORWARD ===\n";
// }

// int main() {
//     ft::list<std::string> a;

//     std::string values[] = {
//         "AAA", "BBB", "CCC", "DDD", "EEE",
//         "atencion", "etoky", "amigo de pepito", "achwd",
//         "mxxrd", "amigo de la tornada"
//     };

//     for (int i = 0; i < 11; ++i)
//         a.push_front(values[i]);

//     std::cout << "List size: " << a.size() << "\n";
//     std::cout << "List empty? " << (a.empty() ? "yes" : "no") << "\n";

//     debug_forward_print(a);
//     debug_reverse_print(a);

//     return 0;
// }


// #include <iostream>
// #include <string>
// #include <sstream>
// #include <vector>
// #include "deque.hpp"

// void print_deque(const ft::deque<int>& d, const std::string& name = "deque") {
//     std::cout << name << " [size=" << d.size() << "]: ";
//     for (ft::deque<int>::const_iterator it = d.begin(); it != d.end(); ++it)
//         std::cout << *it << " ";
//     std::cout << "\n";
// }

// int main() {
//     std::cout << "== Default constructor + push_back ==\n";
//     ft::deque<int> d;
//     for (int i = 0; i < 10; ++i)
//         d.push_back(i);
//     print_deque(d);
    
//     {
//     std::cout << "== push_front ==\n";
//     for (int i = 1; i <= 5; ++i)
//         d.push_front(i * -1);
//     print_deque(d);

//     std::cout << "== operator[] and at() access ==\n";
//     for (std::size_t i = 0; i < d.size(); ++i)
//         std::cout << "d[" << i << "] = " << d[i] << "\n";

//     try {
//         std::cout << "Accessing at(" << d.size() << "): ";
//         std::cout << d.at(d.size()) << "\n";
//     } catch (ft::exception &e) {
//         std::cout << "Caught exception: " << e.what() << "\n";
//     }

//     std::cout << "== pop_back and pop_front ==\n";
//     d.pop_back();
//     d.pop_front();
//     print_deque(d);

//     std::cout << "== front and back ==\n";
//     std::cout << "Front: " << d.front() << ", Back: " << d.back() << "\n";

//     std::cout << "== reverse iteration ==\n";
//     for (ft::deque<int>::reverse_iterator rit = d.rbegin(); rit != d.rend(); ++rit)
//         std::cout << *rit << " ";
//     std::cout << "\n";

//     std::cout << "== copy constructor ==\n";
//     ft::deque<int> d2(d);
//     print_deque(d2, "copy");

//     std::cout << "== assignment ==\n";
//     ft::deque<int> d3;
//     d3 = d2;
//     print_deque(d3, "assigned");

//     std::cout << "== assign(n, val) ==\n";
//     d3.assign(7, 42);
//     print_deque(d3, "assigned 42");

//     std::cout << "== assign(it, it) ==\n";
//     d3.assign(d.begin(), d.begin() + 4);
//     print_deque(d3, "assigned from range");

//     std::cout << "== resize smaller ==\n";
//     d3.resize(2);
//     print_deque(d3, "resized smaller");

//     std::cout << "== resize larger ==\n";
//     d3.resize(6, 99);
//     print_deque(d3, "resized larger");

//     std::cout << "== erase one ==\n";
//     if (!d.empty())
//         d.erase(d.begin() + 3);
//     print_deque(d, "erased one");

//     std::cout << "== erase range ==\n";
//     if (d.size() > 3)
//         d.erase(d.begin(), d.begin() + 3);
//     print_deque(d, "erased range");

//     std::cout << "== comparison ==\n";
//     ft::deque<int> a, b;
//     a.push_back(1);
//     a.push_back(2);
//     b.push_back(1);
//     b.push_back(3);

//     std::cout << "(a == b): " << (a == b) << "\n";
//     std::cout << "(a != b): " << (a != b) << "\n";
//     std::cout << "(a < b): " << (a < b) << "\n";
//     std::cout << "(a > b): " << (a > b) << "\n";
//     std::cout << "(a <= b): " << (a <= b) << "\n";
//     std::cout << "(a >= b): " << (a >= b) << "\n";

//     std::cout << "== swap ==\n";
//     ft::swap(a, b);
//     print_deque(a, "a after swap");
//     print_deque(b, "b after swap");

//     std::cout << "== clear ==\n";
//     a.clear();
//     std::cout << "a is " << (a.empty() ? "empty" : "not empty") << "\n";
//     }
//     {
//     std::cout << "\n== insert single element ==\n";
//     ft::deque<int> a;
//     for (int i = 0; i < 5; ++i) a.push_back(i);
//     print_deque(a);
//     a.insert(a.begin() + 2, 99); // insert at middle
//     print_deque(a);
//     a.insert(a.begin(), 100); // insert at beginning
//     print_deque(a);
//     a.insert(a.end(), 101); // insert at end
//     print_deque(a);

//     std::cout << "\n== insert fill (n, value) ==\n";
//     ft::deque<int> b;
//     for (int i = 0; i < 3; ++i) b.push_back(i);
//     print_deque(b);
//     b.insert(b.begin() + 1, 3, 7); // insert three 7s in the middle
//     print_deque(b);
//     b.insert(b.end(), 2, 9); // insert at the end
//     print_deque(b);
//     b.insert(b.begin(), 4, 1); // insert at the beginning
//     print_deque(b);

//     std::cout << "\n== insert range from array ==\n";
//     int arr[] = {10, 11, 12, 13};
//     ft::deque<int> c;
//     for (int i = 0; i < 4; ++i) c.push_back(i * 2);
//     print_deque(c);
//     c.insert(c.begin() + 2, arr, arr + 4);
//     print_deque(c);

//     std::cout << "\n== insert range from another deque ==\n";
//     ft::deque<int> d1;
//     for (int i = 1; i <= 5; ++i) d1.push_back(i);
//     ft::deque<int> d2;
//     d2.push_back(100);
//     d2.push_back(200);
//     print_deque(d1);
//     d1.insert(d1.begin() + 3, d2.begin(), d2.end());
//     print_deque(d1);

//     std::cout << "\n== insert range from std::vector ==\n";
//     std::vector<int> vec;
//     for (int i = 0; i < 5; ++i) vec.push_back(i + 50);
//     ft::deque<int> e;
//     e.push_back(0);
//     e.insert(e.end(), vec.begin(), vec.end());
//     print_deque(e);
//     }
//     return 0;
// }


// // Test to trigger insert seg fault
// #include <iostream>
// #include <string>
// #include "deque.hpp" // Your ft::deque implementation

// int main() {
//     ft::deque<std::string> a;
//     ft::deque<std::string> b;

//     // Fill `a` with some values to get a valid iterator
//     a.push_back("first");
//     a.push_back("second");

//     // Get a valid position inside `a`
//     ft::deque<std::string>::iterator pos = a.begin();  // points to "first"

//     // Create an empty range
//     ft::deque<std::string>::iterator empty_first = b.begin();
//     ft::deque<std::string>::iterator empty_last = b.end();

//     // 🔥 Will SEGFAULT if you don't guard against empty range in insert
//     a.insert(pos, empty_first, empty_last);

//     std::cout << "If you see this, no segfault occurred.\n";
//     return 0;
// }

// // Test to trigger push_front seg fault
// #include <iostream>
// #include <string>
// #include "deque.hpp"  // or your path

// int main() {
//     ft::deque<std::string> a;
//     ft::deque<std::string> b;

//     std::string values[] = {
//         "fgdwd", "gwpbt", "wblns", "deugu", "moqcd",
//         "atencion", "etoky", "amigo de pepito", "achwd",
//         "mxxrd", "amigo de la tornada"
//     };
//     a = b;
//     for (int i = 0; i < 11; ++i)
//         a.push_front(values[i]);

//     std::cout << "==================================== BEFORE ====================================\n";
//     std::cout << "a: \nsize: " << a.size() << "\ncontent:\n";
//     for (size_t i = 0; i < a.size(); ++i)
//         std::cout << "-> [" << a[i] << "]\n";
//     std::cout << "b: \nsize: " << b.size() << "\ncontent:\n";
//     for (size_t i = 0; i < b.size(); ++i)
//         std::cout << "-> [" << b[i] << "]\n";

//     std::cout << "================================== PUSH FRONT ==================================\n";
//     std::cout << "a.push_front(value) with value = [wcibx]\n";
//     a.push_front("wcibx");

//     std::cout << "a: \nsize: " << a.size() << "\ncontent:\n";
//     for (size_t i = 0; i < a.size(); ++i)
//         std::cout << "-> [" << a[i] << "]\n";
//     std::cout << "b: \nsize: " << b.size() << "\ncontent:\n";
//     for (size_t i = 0; i < b.size(); ++i)
//         std::cout << "-> [" << b[i] << "]\n";

//     std::cout << "[******************************]\n";

// }
