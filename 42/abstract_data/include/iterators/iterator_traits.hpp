#ifndef ITERATOR_TRAITS_HPP
# define ITERATOR_TRAITS_HPP

# include <cstddef>
# include <iterator>
# include "../utils/enable_if.hpp"

namespace ft {

// minimal iterator tags
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag       : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// fallback: no members at all
template <typename, typename = void>
struct iterator_traits { };

// iterators that have the right nested typedefs
template <typename Iterator>
struct iterator_traits<
  Iterator,
  typename ft::check_type<typename Iterator::iterator_category>::type
> {
  typedef typename Iterator::difference_type   difference_type;
  typedef typename Iterator::value_type        value_type;
  typedef typename Iterator::pointer           pointer;
  typedef typename Iterator::reference         reference;
  typedef typename Iterator::iterator_category iterator_category;
};

// raw pointer specializations (stay as you had)
template <typename T>
struct iterator_traits<T*> {
  typedef std::ptrdiff_t              difference_type;
  typedef T                           value_type;
  typedef T*                          pointer;
  typedef T&                          reference;
  typedef ft::random_access_iterator_tag iterator_category;
};

template <typename T>
struct iterator_traits<const T*> {
  typedef std::ptrdiff_t              difference_type;
  typedef T                           value_type;
  typedef const T*                    pointer;
  typedef const T&                    reference;
  typedef ft::random_access_iterator_tag iterator_category;
};


} // namespace ft

#endif // ITERATOR_TRAITS_HPP

