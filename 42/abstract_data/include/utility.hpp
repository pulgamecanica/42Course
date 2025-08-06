#ifndef FT_UTILITY_HPP
# define FT_UTILITY_HPP

namespace ft {
  
template <typename type_1, typename type_2>
struct pair {
public:
  typedef type_1 first_type;
  typedef type_2 second_type;

  pair(): first(), second() {}

  pair(const pair& p): first(p.first), second(p.second) {}

  pair(const type_1& x, const type_2& y): first(x), second(y) {}

  template<typename U, typename V>
  pair(const pair<U, V>& p): first(p.first), second(p.second) {}

  type_1  first;
  type_2  second;
};

template<typename type_1, typename type_2>
pair<type_1, type_2> make_pair(type_1 x, type_2 y) {
  return (pair<type_1, type_2>(x, y));
}

template <typename type_1, typename type_2>
bool operator==(const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
  return (lhs.first == rhs.first && lhs.second == rhs.second);
}

template <typename type_1, typename type_2>
bool operator!=(const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
  return (!(lhs == rhs));
}

template <typename type_1, typename type_2>
bool operator< (const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
  return (lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second));
}

template <typename type_1, typename type_2>
bool operator<=(const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
  return (!(rhs < lhs));
}

template <typename type_1, typename type_2>
bool operator>(const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
  return (rhs < lhs);
}

template <typename type_1, typename type_2>
bool operator>=(const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
  return (!(lhs < rhs));
}

} // namespace ft

#endif // FT_UTILITY_HPP