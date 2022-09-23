#ifndef FT_EQUAL_HPP
# define FT_EQUAL_HPP

namespace ft {
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
}

#endif
