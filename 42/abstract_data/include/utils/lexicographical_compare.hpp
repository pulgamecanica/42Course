#ifndef FT_LEXICOGRAPHICAL_COMPARE_HPP
# define FT_LEXICOGRAPHICAL_COMPARE_HPP

namespace ft {
	template <class InputIterator1, class InputIterator2>
	bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
		while (first1 != last1 && first2 != last2) {
      if (*first1 < *first2)
        return (true);
      else if (*first2 < *first1)
        return (false);
      first1++;
      first2++;
		}
    return (first1 == last1 && first2 != last2);
	}

	template <class InputIterator1, class InputIterator2, class Compare>
	bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
		while (first1 != last1 && first2 != last2) {
      if (comp(*first1, *first2))
        return (true);
      else if (comp(*first2, *first1))
        return (false);
      first1++;
      first2++;
		}
    return (first1 == last1 && first2 != last2);
	}
}

#endif