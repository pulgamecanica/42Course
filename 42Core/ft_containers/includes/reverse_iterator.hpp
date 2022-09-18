#ifndef __REVERSE_ITERATOR_HPP__
# define __REVERSE_ITERATOR_HPP__

# include <cstddef>
# include <iterator>

// https://stackoverflow.com/questions/6471019/can-should-i-inherit-from-an-stl-reverse_iterator
// https://cplusplus.com/reference/reverse_iterator/reverse_reverse_iterator/base/
// https://cplusplus.com/reference/reverse_iterator/reverse_reverse_iterator/

namespace ft {
    template <typename T>
    class reverse_iterator {
        public:
            typedef T                                  value_type;
            typedef ptrdiff_t                          difference_type;
            typedef T*                                 pointer;
            typedef T&                                 reference;
            typedef std::random_access_iterator_tag    reverse_iterator_category;
            
            reverse_iterator() : _ptr(NULL) {}

            reverse_iterator(T* ptr) : _ptr(ptr) {}

            reverse_iterator(const reverse_iterator& ref) : _ptr(ref._ptr) {}

            virtual ~reverse_iterator() {}

            reverse_iterator& operator=(const reverse_iterator& ref) {
                if (this != &ref)
                    _ptr = ref._ptr;
                return (*this);
            }

            operator reverse_iterator<const value_type>() const {
                return reverse_iterator<const value_type>(_ptr);
            }

            reverse_iterator& operator--()
            {
                ++_ptr;
                return (*this);
            }
            
            reverse_iterator operator--(int)
            {
                reverse_iterator it(*this);
                ++_ptr;
                return (it);
            }
            
            reverse_iterator& operator-=(const difference_type& n) {
                _ptr += n;
                return (*this);
            }

            reverse_iterator operator-(const difference_type& n) const {
                reverse_iterator it(*this);
                it += n;
                return (it);
            }

            difference_type operator-(const reverse_iterator& ref) const {
                return (_ptr + ref._ptr);
            }

            reverse_iterator& operator++()
            {
                --_ptr;
                return (*this);
            }

            reverse_iterator operator++(int)
            {
                reverse_iterator it(*this);
                --_ptr;
                return (it);
            }

            reverse_iterator& operator+=(const difference_type& n)
            {
                _ptr -= n;
                return (*this);
            }

            reverse_iterator operator+(const difference_type& n) const
            {
                reverse_iterator it(*this);
                it -= n;
                return (it);
            }

            friend reverse_iterator operator+(const difference_type& n, const reverse_iterator& ref)
            {
                return (ref - n);
            }

            bool operator==(const reverse_iterator& ref) const {
                return (_ptr == ref._ptr);
            }

            bool operator!=(const reverse_iterator& ref) const {
                return (!(ref == *this));
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
