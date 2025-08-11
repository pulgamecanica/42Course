#ifndef FT_ALGORITHM_HPP
#define FT_ALGORITHM_HPP

#include "less.hpp"
#include "swap.hpp"
#include "../iterators/iterator_traits.hpp"

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

// ----- detail helpers (not part of public API) -----
namespace detail {

template <class RandomAccessIterator, class Distance, class T, class Compare>
inline void sift_up(RandomAccessIterator first,
                    Distance hole_idx,
                    T value,
                    Compare comp)
{
    // Move the hole up until parent satisfies heap with 'value'
    while (hole_idx > 0) {
        Distance parent = (hole_idx - 1) / 2;
        // If parent < value (for less<T>, parent is "smaller"), we need to move parent down
        if (comp(*(first + parent), value)) {
            *(first + hole_idx) = *(first + parent);
            hole_idx = parent;
        } else {
            break;
        }
    }
    *(first + hole_idx) = value;
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
inline void sift_down(RandomAccessIterator first,
                      Distance hole_idx,
                      Distance n,
                      T value,
                      Compare comp)
{
  // Percolate the hole down selecting the "better" child according to comp
  // Invariant: we want parent not less-than child: comp(parent, child) == false
  Distance child = 2 * hole_idx + 1; // left child
  while (child < n) {
      // pick the child that should be above (the "max" for ft::less)
      Distance right = child + 1;
      if (right < n && comp(*(first + child), *(first + right))) {
          child = right;
      }
      // If value should be below child, move child up and continue
      if (comp(value, *(first + child))) {
          *(first + hole_idx) = *(first + child);
          hole_idx = child;
          child = 2 * hole_idx + 1;
      } else {
          break;
      }
  }
  *(first + hole_idx) = value;
}

} // namespace detail

template <class RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
  typedef typename ft::iterator_traits<RandomAccessIterator>::value_type value_type;
  make_heap(first, last, ft::less<value_type>());
}

template <class RandomAccessIterator, class Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
  typedef typename ft::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  typedef typename ft::iterator_traits<RandomAccessIterator>::value_type      value_type;

  difference_type n = last - first;
  if (n < 2) return;

  // Start from last parent and sift_down
  for (difference_type parent = (n - 2) / 2; ; --parent) {
    value_type value = *(first + parent);
    detail::sift_down(first, parent, n, value, comp);
    if (parent == 0) break; // avoid underflow
  }
}

template <class RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
  typedef typename ft::iterator_traits<RandomAccessIterator>::value_type value_type;
  push_heap(first, last, ft::less<value_type>());
}

template <class RandomAccessIterator, class Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
  typedef typename ft::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  typedef typename ft::iterator_traits<RandomAccessIterator>::value_type      value_type;

  difference_type n = last - first;
  if (n <= 1) return;

  difference_type hole_idx = n - 1;
  value_type value = *(first + hole_idx);
  detail::sift_up(first, hole_idx, value, comp);
}

template <class RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
  typedef typename ft::iterator_traits<RandomAccessIterator>::value_type value_type;
  pop_heap(first, last, ft::less<value_type>());
}

template <class RandomAccessIterator, class Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
  typedef typename ft::iterator_traits<RandomAccessIterator>::difference_type difference_type;
  typedef typename ft::iterator_traits<RandomAccessIterator>::value_type      value_type;

  difference_type n = last - first;
  if (n < 2) return;

  // Move the top to last-1
  --last;
  value_type top = *first;
  *first = *last;

  // Restore heap on [first, last)
  if (n - 1 > 0) {
    detail::sift_down(first, difference_type(0), n - 1, *first, comp);
  }

  // Put the former top at the end (heap-sorted position)
  *last = top;
}

} // namespace ft

#endif // FT_ALGORITHM_HPP
