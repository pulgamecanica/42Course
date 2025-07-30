#ifndef FT_EXCEPTION_HPP
#define FT_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace ft {

// Base exception to match `NAMESPACE::exception`
class exception : public std::exception {
public:
  virtual const char* what() const throw() {
    return "ft::exception";
  }
};

class out_of_range : public std::out_of_range, public ft::exception {
public:
  explicit out_of_range(const std::string& what_arg)
    : std::out_of_range(what_arg) {}

  const char* what() const throw() {
    return std::out_of_range::what();
  }
};

} // namespace ft

#endif // FT_EXCEPTION_HPP
