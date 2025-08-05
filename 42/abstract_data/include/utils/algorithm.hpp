#ifndef FT_ALGORITHM_HPP
#define FT_ALGORITHM_HPP

namespace ft {

template<class T> 
const T& max(const T& a, const T& b)
{
  return (a < b) ? b : a;
}

template<class T, class Compare> 
const T& max(const T& a, const T& b, Compare comp)
{
  return (comp(a, b)) ? b : a;
}

template<class T>
const T& min(const T& a, const T& b)
{
  return (b < a) ? b : a;
}

template<class T, class Compare>
const T& min(const T& a, const T& b, Compare comp)
{
  return (comp(b, a)) ? b : a;
}

template <class InputIterator1, class InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
  for (; first1 != last1; first1++, first2++)
    if (!(*first1 == *first2))
      return (false);
  return (true);
}

template <class InputIterator1, class InputIterator2, class BinaryPredicate>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred) {
  for (; first1 != last1; first1++, first2++)
    if (!pred(*first1, *first2))
      return (false);
  return (true);
}

} // namespace ft

#endif // FT_ALGORITHM_HPP
