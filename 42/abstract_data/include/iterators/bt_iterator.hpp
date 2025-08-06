#ifndef FT_BT_ITERATOR_HPP
#define FT_BT_ITERATOR_HPP

#include "../utils/node.hpp"
#include <cstddef>

namespace ft {

template <typename T, typename Pointer, typename Reference, class Compare>
class BTIterator {
public:
    typedef BTIterator<T, Pointer, Reference, Compare>              self_type;
    typedef BTIterator<T, T*, T&, Compare>                          iterator;
    typedef BTIterator<T, const T*, const T&, Compare>              const_iterator;
    typedef T                                                       value_type;
    typedef Pointer                                                 pointer;
    typedef Reference                                               reference;
    typedef ptrdiff_t                                               difference_type;
    typedef std::bidirectional_iterator_tag                         iterator_category;
    typedef BTNode<T>*                                              node_pointer;

    node_pointer ptr;

    BTIterator() : ptr(NULL) {}
    explicit BTIterator(node_pointer p) : ptr(p) {}

    // Allow const_iterator = iterator
    BTIterator(const iterator& it) : ptr(it.ptr) {}

    // Dereference
    reference operator*() const { return ptr->getData(); }
    pointer operator->() const { return &ptr->getData(); }

    // Increment
    self_type operator++(int) { self_type tmp = *this; ptr = ptr->successor(); return tmp; }
    self_type& operator++()   { ptr = ptr->successor(); return *this; }

    // Decrement
    self_type operator--(int) { self_type tmp = *this; ptr = ptr->predecessor(); return tmp; }
    self_type& operator--()   { ptr = ptr->predecessor(); return *this; }

    // Comparison
    template<typename T1, typename P1, typename R1, typename C1,
             typename T2, typename P2, typename R2, typename C2>
    friend bool operator==(const BTIterator<T1, P1, R1, C1>& lhs,
                           const BTIterator<T2, P2, R2, C2>& rhs) {
        return lhs.ptr == rhs.ptr;
    }

    template<typename T1, typename P1, typename R1, typename C1,
             typename T2, typename P2, typename R2, typename C2>
    friend bool operator!=(const BTIterator<T1, P1, R1, C1>& lhs,
                           const BTIterator<T2, P2, R2, C2>& rhs) {
        return !(lhs == rhs);
    }
};

}

#endif