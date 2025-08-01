#ifndef FT_FUNCTIONAL_HPP
#define FT_FUNCTIONAL_HPP

namespace ft {

template <typename T>
struct less {
  bool operator()(const T& lhs, const T& rhs) const {
    return lhs < rhs;
  }
};

} // namespace ft

#endif // FT_FUNCTIONAL_HPP
