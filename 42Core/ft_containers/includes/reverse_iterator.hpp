#ifndef __REVERSE_ITERATOR_HPP__
# define __REVERSE_ITERATOR_HPP__

# include <cstddef>
# include <iterator>
#include "iterator_traits.hpp"


// https://stackoverflow.com/questions/6471019/can-should-i-inherit-from-an-stl-reverse_iterator
// https://cplusplus.com/reference/reverse_iterator/reverse_reverse_iterator/base/
// https://cplusplus.com/reference/reverse_iterator/reverse_reverse_iterator/

namespace ft {
    template <typename Iterator>
    class reverse_iterator {
        public:
            typedef Iterator                                                iterator_type;
            typedef typename ft::iterator_traits<Iterator>::iterator_category   iterator_category;
            typedef typename ft::iterator_traits<Iterator>::value_type          value_type;
            typedef typename ft::iterator_traits<Iterator>::difference_type     difference_type;
            typedef typename ft::iterator_traits<Iterator>::pointer             pointer;
            typedef typename ft::iterator_traits<Iterator>::reference           reference;

            reverse_iterator(): _it() {}
            
            reverse_iterator(iterator_type it): _it(it) {}

            template <typename It>
            reverse_iterator(const reverse_iterator<It>& it): _it(it.base()) {}

            iterator_type base() const {
                return (_it);
            }

            reference operator*() const {
                iterator_type tmp = _it;
                return (*--tmp);
            }

            reverse_iterator operator+ (difference_type n) const {
                return (reverse_iterator(_it - n));
            }

            reverse_iterator& operator++() {
                --_it;
                return (*this);
            }

            reverse_iterator operator++(int) {
                reverse_iterator tmp = *this;
                --_it;
                return (tmp);
            }

            reverse_iterator operator+= (difference_type n) {
                _it -= n;
                return (*this);
            }

            reverse_iterator operator-= (difference_type n) {
                _it += n;
                return (*this);
            }

            reverse_iterator operator- (difference_type n) const {
                return (reverse_iterator(_it + n));
            }

            difference_type operator- (const reverse_iterator r_it) const {
                return (_it + r_it._it);
            }

            reverse_iterator& operator--() {
                ++_it;
                return (*this);
            }

            reverse_iterator operator-- (int) {
                reverse_iterator tmp = *this;
                ++_it;
                return (tmp);
            }

            pointer operator->() const {
                return (&(operator*()));
            }

            reference operator[] (difference_type n) const {
                return (*(*this + n));
            }

            bool operator==(const reverse_iterator &other) {
                return ((this->_it == other._it));
            }

            bool operator!=(const reverse_iterator &other) {
                return ((this->_it != other._it));
            }

        private:
            iterator_type _it;
    };

    template <typename Iterator>
    bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return (lhs.base() == rhs.base());
    }

    template <typename Iterator>
    bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return (!(lhs == rhs));
    }

    template <typename Iterator>
    bool operator<(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return (lhs.base() > rhs.base());
    }

    template <typename Iterator>
    bool operator<=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return (lhs.base() >= rhs.base());
    }

    template <typename Iterator>
    bool operator>(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return (lhs.base() < rhs.base());
    }

    template <typename Iterator>
    bool operator>=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return (lhs.base() <= rhs.base());
    }

    template <typename Iterator>
    reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& rev_it) {
        return (rev_it + n);
    }

    template <typename Iterator>
    typename reverse_iterator<Iterator>::difference_type operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return (rhs.base() - lhs.base());
    }
}

#endif
