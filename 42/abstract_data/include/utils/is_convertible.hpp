#ifndef FT_IS_CONVERTIBLE_HPP
#define FT_IS_CONVERTIBLE_HPP

namespace ft {

namespace _detail {

typedef char yes;
typedef struct { char _[2]; } no;

template <typename From, typename To>
struct is_convertible_helper {
    static yes test(To);
    static no test(...);
    static From make();
    enum { value = sizeof(test(make())) == sizeof(yes) };
};

} // namespace _detail

template <typename From, typename To>
struct is_convertible {
    static const bool value = _detail::is_convertible_helper<From, To>::value;
};

} // namespace ft

#endif // FT_IS_CONVERTIBLE_HPP
