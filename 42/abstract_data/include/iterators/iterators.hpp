// #ifndef FT_ITERATORS_HPP
// #define FT_ITERATORS_HPP

// // Core iterator support library (minimal subset inspired by ISO/IEC 14882:1998 Clause 24)
// // This header defines:
// //   * container-specific iterators
// //   * advance / distance primitives that dispatch on iterator_category
// // The goal is to supply all machinery required by generic algorithms using iterators.

// #include <cstddef>   // ptrdiff_t, size_t
// #include <iterator>  // std::input_iterator_tag

// #include "vector_iterator.hpp"

// // namespace ft {  // "Abstract Data ft" namespace

// // //------------------------------------
// // // 24.3.4  Iterator primitives: advance & distance
// // //------------------------------------
// // namespace detail {

// // // advance – dispatch by category

// // template <class InputIt, class Dist>
// // inline void advance_impl(InputIt& it, Dist n, std::input_iterator_tag) {
// //     while (n-- > 0) ++it;
// // }

// // template <class BidiIt, class Dist>
// // inline void advance_impl(BidiIt& it, Dist n, std::bidirectional_iterator_tag) {
// //     if (n >= 0) {
// //         while (n-- > 0) ++it;
// //     } else {
// //         while (n++ < 0) --it;
// //     }
// // }

// // template <class RandIt, class Dist>
// // inline void advance_impl(RandIt& it, Dist n, std::random_access_iterator_tag) {
// //     it += n;
// // }

// // // distance – dispatch by category

// // template <class InputIt>
// // inline typename iterator_traits<InputIt>::difference_type
// //     distance_impl(InputIt first, InputIt last, std::input_iterator_tag) {
// //     typename iterator_traits<InputIt>::difference_type d = 0;
// //     for (; first != last; ++first) ++d;
// //     return d;
// // }

// // template <class RandIt>
// // inline typename iterator_traits<RandIt>::difference_type
// //     distance_impl(RandIt first, RandIt last, std::random_access_iterator_tag) {
// //     return last - first;
// // }

// // } // namespace detail

// // // Public wrappers

// // template <class InputIt, class Dist>
// // inline void advance(InputIt& it, Dist n) {
// //     typedef typename iterator_traits<InputIt>::iterator_category cat;
// //     detail::advance_impl(it, n, cat());
// // }

// // template <class InputIt>
// // inline typename iterator_traits<InputIt>::difference_type
// //     distance(InputIt first, InputIt last) {
// //     typedef typename iterator_traits<InputIt>::iterator_category cat;
// //     return detail::distance_impl(first, last, cat());
// // }

// // } // namespace ft
// #endif // FT_ITERATORS_HPP
