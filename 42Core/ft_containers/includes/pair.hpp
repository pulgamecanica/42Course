// #include <algorithm>
// #include <stdexcept>
// #include <iostream>
// #include <typeinfo>
// #include <sstream>
// #include "iterator.hpp"
// #include "iterator_traits.hpp"
// #include "reverse_iterator.hpp"
// #include "lexicographical_compare.hpp"
// #include "enable_if.hpp"
// #include "equal.hpp"

#ifndef PAIR_HPP
# define PAIR_HPP

# ifndef LINUX
#  define LINUX 0
# endif

namespace ft {
    template <typename type_1, typename type_2>
    struct pair {
    public:
        typedef type_1  first_type;
        typedef type_2  second_type;

        pair(): first(), second() {}

        template<typename U, typename V>
        pair(const pair<U, V>& p): first(p.first), second(p.second) {}

        pair(const first_type& x, const second_type& y): first(x), second(y) {}

        type_1 first;
        type_2 second;
    };

    template <typename type_1, typename type_2>
    bool operator== (const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
        return (lhs.first == rhs.first && lhs.second == rhs.second);
    }

    template <typename type_1, typename type_2>
    bool operator!= (const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
        return (!(lhs == rhs));
    }

    template <typename type_1, typename type_2>
    bool operator<  (const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
        return (lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second));
    }

    template <typename type_1, typename type_2>
    bool operator<= (const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
        return (!(rhs < lhs));
    }

    template <typename type_1, typename type_2>
    bool operator>  (const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
        return (rhs < lhs);
    }

    template <typename type_1, typename type_2>
    bool operator>= (const pair<type_1, type_2>& lhs, const pair<type_1, type_2>& rhs) {
        return (!(lhs < rhs));
    }

    template<typename type_1, typename type_2>
    pair<type_1, type_2> make_pair(type_1 x, type_2 y) {
        return (pair<type_1, type_2>(x, y));
    }

    template <typename type_1, typename type_2>
    std::ostream&   operator<<(std::ostream& s, const pair<type_1, type_2>& pair) {
        return (s << "[" << pair.first << ", " << pair.second << "]"); 
    }
}


#endif
