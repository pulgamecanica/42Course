#ifndef __ITERATOR_HPP__
# define __ITERATOR_HPP__

# include <cstddef>
# include <iterator>

// https://stackoverflow.com/questions/6471019/can-should-i-inherit-from-an-stl-iterator
// https://cplusplus.com/reference/iterator/
// https://cplusplus.com/reference/iterator/iterator/
// https://cplusplus.com/reference/iterator/reverse_iterator/

namespace ft {
    template <typename T>
    class iterator {
        public:
            typedef T                                  value_type;
            typedef ptrdiff_t                          difference_type;
            typedef T*                                 pointer;
            typedef T&                                 reference;
            typedef std::random_access_iterator_tag    iterator_category;
            
            iterator() : _ptr(NULL) {}

            iterator(T* ptr) : _ptr(ptr) {}

            iterator(const iterator& ref) : _ptr(ref._ptr) {}


            virtual ~iterator() {}

            iterator& operator=(const iterator& ref) {
                if (this != &ref)
                    _ptr = ref._ptr;
                return (*this);
            }

            operator iterator<const value_type>() const {
                return iterator<const value_type>(_ptr);
            }

            iterator& operator--()
            {
                --_ptr;
                return (*this);
            }
            
            iterator operator--(int)
            {
                iterator it(*this);
                --_ptr;
                return (it);
            }
            
            iterator& operator-=(const difference_type& n) {
                _ptr -= n;
                return (*this);
            }

            iterator operator-(const difference_type& n) const {
                iterator it(*this);
                it -= n;
                return (it);
            }

            difference_type operator-(const iterator& ref) const {
                return (_ptr - ref._ptr);
            }

            iterator& operator++()
            {
                ++_ptr;
                return (*this);
            }

            iterator operator++(int)
            {
                iterator it(*this);
                ++_ptr;
                return (it);
            }

            iterator& operator+=(const difference_type& n)
            {
                _ptr += n;
                return (*this);
            }

            iterator operator+(const difference_type& n) const
            {
                iterator it(*this);
                it += n;
                return (it);
            }

            friend iterator operator+(const difference_type& n, const iterator& ref)
            {
                return (ref + n);
            }

            bool operator==(const iterator& ref) const {
                return (_ptr == ref._ptr);
            }

            bool operator!=(const iterator& ref) const {
                return (!(*this ==  ref));
            }

            bool operator<(const iterator& ref) const {
                return (_ptr < ref._ptr);
            }
            
            bool operator<=(const iterator& ref) const {
                return (!(ref < *this));
            }

            bool operator>(const iterator& ref) const {
                return (ref < *this);
            }

            bool operator>=(const iterator& ref) const {
                return (!(*this < ref));
            }

            reference operator*() const {
                return (*_ptr);
            }
            
            pointer operator->() const {
                return (_ptr);
            }

            reference operator[](const difference_type& n) const {
                return (_ptr[n]);
            }

        private:
            T* _ptr;
    };
}

#endif
