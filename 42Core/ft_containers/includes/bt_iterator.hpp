#ifndef BT_ITERATOR_HPP
# define BT_ITERATOR_HPP

# include <memory>
# include <cstddef>
# include "node.hpp"

namespace ft {
    template < typename T, typename Pointer, typename Reference, class C >
    class BTIterator {
    public:
        typedef BTIterator<T, Pointer, Reference, C >  type;
        typedef BTIterator<T,       T*,       T&, C >  iterator;
        typedef BTIterator<T, const T*, const T&, C >  const_iterator;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
        typedef T           value_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
        typedef Node<T, C>* node_pointer;
        typedef std::bidirectional_iterator_tag             iterator_category;
    
        BTIterator() : ptr(NULL) { }
        BTIterator(const node_pointer ptr_) : ptr(ptr_) {}
        BTIterator(const iterator& it) : ptr(const_cast<node_pointer>(it.ptr)) { }

        type&  operator=(const const_iterator& it) {
            this->ptr = it.ptr;
            return *this;
        }

        virtual ~BTIterator() {}

        BTIterator operator++(int) {
            BTIterator tmp(*this);
            this->ptr = ptr->successor();
            return tmp;
        }
        BTIterator&    operator++() {
            this->ptr = ptr->successor();
            return *this;
        }
        BTIterator operator--(int) {
            BTIterator tmp(*this);
            this->ptr = ptr->predecessor();
            return tmp;
        }
        BTIterator&    operator--() {
            this->ptr = ptr->predecessor();
            return *this;
        }
        reference   operator*() {
            return this->ptr->getData(); // CHECK IF THIS WORKS!
        }
        pointer     operator->() {
            return (&(this->ptr->getData()));
        }
        template<typename T2, typename P, typename R, class C2>
        friend inline bool operator==(const type& lhs, const BTIterator<T2, P, R, C2>& rhs) {
            return (lhs.ptr == rhs.ptr);
        }
        template<typename T2, typename P, typename R, class C2>
        friend inline bool operator!=(const type& lhs, const BTIterator<T2, P, R, C2>& rhs) { return !(lhs == rhs); }

        node_pointer    ptr;
    };

    template <class Iterator >
    class ReverseBI {
    protected:
        Iterator    current;
    public:
        typedef Iterator                            iterator_type;
        typedef typename Iterator::pointer          pointer;
        typedef typename Iterator::reference        reference;
        typedef typename Iterator::size_type        size_type;
        typedef typename Iterator::difference_type  difference_type;
        typedef typename Iterator::iterator_category    iterator_category;

        ReverseBI() : current() { }
        ReverseBI(const iterator_type& x) : current(x) { }
        ReverseBI(const ReverseBI& x) : current(x.current) { }
        template<typename Iter>
        ReverseBI(const ReverseBI<Iter>& x) : current(x.base()) { } //.base() returns the underlying iterator
        ~ReverseBI() { }
 
        iterator_type   base() const { return this->current; }

        reference   operator*() {
            return *current;
        }
        pointer     operator->() {
            return &operator*();
        }
        ReverseBI&  operator++() {
            --current;
            return *this;
        }
        ReverseBI   operator++(int) {
            ReverseBI   out(*this);
            --current;
            return out;
        }
        ReverseBI&  operator--() {
            ++current;
            return *this;
        }
        ReverseBI   operator--(int) {
            ReverseBI   out(*this);
            ++current;
            return out;
        }
    };
    template <typename Iterator>
    inline bool operator==(const ReverseBI<Iterator>& lhs, const ReverseBI<Iterator>& rhs) {
        return (lhs.base() == rhs.base());
    }
    template <typename Iterator>
    inline bool operator<(const ReverseBI<Iterator>& lhs, const ReverseBI<Iterator>& rhs) {
        return (rhs.base() < lhs.base());
    }
    template <typename Iterator>
    inline bool operator!=(const ReverseBI<Iterator>& lhs, const ReverseBI<Iterator>& rhs) {
        return !(lhs == rhs);
    }
    template <typename Iterator>
    inline bool operator>(const ReverseBI<Iterator>& lhs, const ReverseBI<Iterator>& rhs) {
        return (rhs < lhs);
    }
    template <typename Iterator>
    inline bool operator<=(const ReverseBI<Iterator>& lhs, const ReverseBI<Iterator>& rhs) {
        return !(rhs < lhs);
    }
    template <typename Iterator>
    inline bool operator>=(const ReverseBI<Iterator>& lhs, const ReverseBI<Iterator>& rhs) {
        return !(lhs < rhs);
    }
    // Comparison of reverse_iterator to const reverse iterator
    template <typename IteratorL, typename IteratorR>
    inline bool operator==(const ReverseBI<IteratorL>& lhs, const ReverseBI<IteratorR>& rhs) {
        return (lhs.base() == rhs.base());
    }
    template <typename IteratorL, typename IteratorR>
    inline bool operator<(const ReverseBI<IteratorL>& lhs, const ReverseBI<IteratorR>& rhs) {
        return (rhs.base() < lhs.base());
    }
    template <typename IteratorL, typename IteratorR>
    inline bool operator!=(const ReverseBI<IteratorL>& lhs, const ReverseBI<IteratorR>& rhs) {
        return !(lhs == rhs);
    }
    template <typename IteratorL, typename IteratorR>
    inline bool operator>(const ReverseBI<IteratorL>& lhs, const ReverseBI<IteratorR>& rhs) {
        return (rhs < lhs);
    }
    template <typename IteratorL, typename IteratorR>
    inline bool operator<=(const ReverseBI<IteratorL>& lhs, const ReverseBI<IteratorR>& rhs) {
        return !(rhs < lhs);
    }
    template <typename IteratorL, typename IteratorR>
    inline bool operator>=(const ReverseBI<IteratorL>& lhs, const ReverseBI<IteratorR>& rhs) {
        return !(lhs < rhs);
    }
    // Generator function for reverse_iterator
    template <typename Iterator>
    inline ReverseBI<Iterator>  make_reverse_iterator(Iterator i) {
        return ReverseBI<Iterator>(i);
    }
}

#endif
