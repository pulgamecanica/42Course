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

template <class>
struct check_type { typedef void type; };

} // namespace ft

#endif // FT_ENABLE_IF_HPP

#ifndef FT_IS_SAME_HPP
#define FT_IS_SAME_HPP

namespace ft {

// Minimal true_/false_ tags (handy generally)
struct true_type  { static const bool value = true;  };
struct false_type { static const bool value = false; };

// is_same
template <typename A, typename B> struct is_same : false_type { };
template <typename T> struct is_same<T, T> : true_type { };

} // namespace ft

#endif // FT_IS_SAME_HPP

#include "remove_const.hpp"

#ifndef FT_IS_CLASS_HPP
#define FT_IS_CLASS_HPP

namespace ft {

// Detects class/union types via pointer-to-member test.
template <typename T>
struct is_class {
private:
    template <typename U> static char  test(int U::*);
    template <typename U> static long  test(...);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

} // namespace ft

#endif // FT_IS_CLASS_HPP

#ifndef FT_IS_BASE_OF_HPP
#define FT_IS_BASE_OF_HPP

namespace ft {

// is_base_of<Base, Derived>
template <typename Base, typename Derived>
struct is_base_of {
private:
    typedef typename ft::remove_const<Base>::type    B;
    typedef typename ft::remove_const<Derived>::type D;

    // local yes/no types
    typedef char yes;
    typedef struct { char _[2]; } no;

    static yes test(B*);
    static no  test(...);

    static D* makeD();

    static const bool convertible = (sizeof(test(makeD())) == sizeof(yes));
    static const bool both_class  = ft::is_class<B>::value && ft::is_class<D>::value;

public:
    static const bool value = both_class ? convertible : false;
};

template <typename T>
struct is_base_of<T, T> {
private:
    typedef typename ft::remove_const<T>::type U;
public:
    static const bool value = ft::is_class<U>::value;
};

} // namespace ft

#endif // FT_IS_BASE_OF_HPP
