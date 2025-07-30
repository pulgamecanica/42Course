#ifndef FT_ENABLE_IF_HPP
#define FT_ENABLE_IF_HPP

//------------------------------------
// Lightweight SFINAE support – enable_if (Substitution Failure is not an Error)
//------------------------------------

namespace ft {

template <bool B, class T = void>
struct enable_if { };

template <class T>
struct enable_if<true, T> {
    typedef T type;
};

} // namespace ft

#endif // FT_ENABLE_IF_HPP
