//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SINGLETON_HPP__
# define __SINGLETON_HPP__

#include <iostream>
#include <vector>
#include <mutex>
#include <algorithm>
#include <memory>

#include "ex03.inc"

/**
* The Singleton class defines the `GetInstance` method that serves as an
* alternative to constructor and lets clients access the same instance of this
* class over and over from any thread.
*/
// T: Objects to Store
template<typename T>
class Singleton {
  public:
  // Singletons have no copy-constructor or assignment operators
  Singleton(const Singleton &) = delete;
  Singleton(Singleton &cl) = delete;
  void operator=(const Singleton &) = delete;
  void operator=(Singleton &) = delete;

  Singleton(const std::string & list_type)
    : list_type_(list_type) {
      ;
  }

  virtual ~Singleton() {
    items_.clear();
  }

  std::shared_ptr<T>& operator[](int index) {
    return (items_.at(index));
  }

  bool has(std::shared_ptr<T> c) {
    return std::find(items_.begin(), items_.end(), c) !=  items_.end();
  }

  void add(std::shared_ptr<T> c) {
    if (!has(c)) {
      items_.push_back(c);
      if (DEBUG)
        std::cout << YELLOW << "[Singleton:" << list_type_ << "] " << GREEN << "ADD\t" << ENDC << *c << std::endl;
    } else if (DEBUG)
      std::cout << YELLOW << "[Singleton:" << list_type_ << "] " << RED << "ADD FAIL\t" << ENDC << *c << " is in the list" << std::endl;
  }

  void remove(std::shared_ptr<T> c) {
      typename std::vector<std::shared_ptr<T> *>::iterator it = std::find(items_.begin(), items_.end(), c);
      if (it != items_.end()) {
        items_.erase(it);
        if (DEBUG)
          std::cout << YELLOW << "[Singleton:" << list_type_ << "] " << GREEN << "REMOVE\t" << ENDC << *c << std::endl;
      } else if (DEBUG) {
        std::cout << YELLOW << "[Singleton:" << list_type_ << "] " << RED << "REMOVE FAIL\t" << ENDC << *c << " is not in the list" << std::endl;
      }
  }

  size_t size() const {
    return items_.size();
  }
  
  protected:
  std::vector<std::shared_ptr<T>> items_;
  static std::mutex mutex_;

  private:
  const std::string list_type_;
};
// std::ostream& operator<<(std::ostream&, const Singleton&);

template<typename T>
std::mutex Singleton<T>::mutex_{};

#endif
