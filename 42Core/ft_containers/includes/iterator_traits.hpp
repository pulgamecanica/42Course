// https://www.cplusplus.com/reference/iterator/iterator_traits/

#ifndef ITERATOR_TRAITS_HPP
# define ITERATOR_TRAITS_HPP

# include <cstddef>

namespace ft {

    template <bool b, class T = void>struct enable_if {};
    template<class T>
    struct enable_if<true, T> {
        typedef T type;
    };
    template <class>
    struct check_type { typedef void type; };
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    template <class T>
    struct _has_iterator_typedefs
    {
    private:
        struct  _two { char _lx; char _lxx; };
        template <class U>  static _two _test(...);
        template <class U>  static char _test(typename  ft::check_type<typename U::iterator_category>::type * = 0,
                                                typename    ft::check_type<typename U::difference_type>::type * = 0,
                                                typename    ft::check_type<typename U::value_type>::type * = 0,
                                                typename    ft::check_type<typename U::reference>::type * = 0,
                                                typename    ft::check_type<typename U::pointer>::type * = 0
        );
    public:
        static const bool value = sizeof(_test<T>(0,0,0,0,0)) == 1;
    };
    template <class T>
    struct _has_iterator_category
    {
    private:
        struct _two { char _lx; char _lxx; };
        template <class U>  static _two _test(...);
        template <class U>  static char _test(typename U::iterator_category * = 0);
    public:
        static const bool value = sizeof(_test<T>(0)) == 1;
    };
    template <class Iter, bool>
    struct _iterator_traits {};


    template <typename Iterator>
    class iterator_traits {
        public:
            typedef typename Iterator::value_type           value_type;
            typedef typename Iterator::difference_type      difference_type;
            typedef typename Iterator::pointer              pointer;
            typedef typename Iterator::reference            reference;
            typedef typename Iterator::iterator_category    iterator_category;
    };

    template <typename T>
    class iterator_traits<T*> {
        public:
            typedef T                                       value_type;
            typedef std::ptrdiff_t                          difference_type;
            typedef T*                                      pointer;
            typedef T&                                      reference;
            typedef std::random_access_iterator_tag         iterator_category;
    };

    template <typename T>
    class iterator_traits<const T*> {
        public:
            typedef T                                       value_type;
            typedef ptrdiff_t                               difference_type;
            typedef const T*                                pointer;
            typedef const T&                                reference;
            typedef std::random_access_iterator_tag         iterator_category;
    };
}

#endif
