#include <stdexcept>
#include <iostream>
#include <typeinfo>
#include "iterator.hpp"
#include "reverse_iterator.hpp"
// #include "enable_if.hpp"
// template<class T, class Allocator = std::allocator<T> >

// https://cplusplus.com/reference/vector/vector/vector/
// https://cplusplus.com/reference/vector/vector/?kw=vector
// https://cplusplus.com/reference/memory/allocator/
// https://cplusplus.com/reference/memory/allocator/allocate/
// https://cplusplus.com/reference/memory/allocator/deallocate/
// https://en.cppreference.com/w/cpp/language/sfinae

namespace ft {
	template < class T, class Alloc = std::allocator<T> >
	class vector {
		public:
			typedef T										value_type;
			typedef Alloc									allocator_type;
			typedef std::size_t								size_type;
			typedef typename allocator_type::reference		reference;
			typedef typename allocator_type::pointer		pointer;
			typedef typename allocator_type::const_pointer	const_pointer;
			typedef ft::iterator<T>							iterator;
			typedef ft::iterator<const T>					const_iterator;
			typedef ft::reverse_iterator<T>					reverse_iterator; //<iterator>
			typedef ft::reverse_iterator<const T>			const_reverse_iterator;//<const_iterator>
			typedef typename iterator::difference_type		difference_type;

			explicit vector (const allocator_type& alloc = allocator_type()): _alloc(alloc), _size(0), _capacity(0), _ptr(pointer()) {};

			explicit vector (size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()): _alloc(alloc), _size(n), _capacity(n) {
				if (n > max_size())
					throw std::length_error("cannot create a vector with a size larger than max_size");
				_ptr = _alloc.allocate(n);
				for (size_type i(0); i < n; i++)
					_alloc.construct(_ptr + i, val);
			};

			virtual ~vector() {
				clear();
				if (_capacity > 0)
					_alloc.deallocate(_ptr, _capacity);
				_capacity = 0;
				_size = 0;
			}
			/*
			 * ITERATORS 
			 */
			iterator begin() {
				return (iterator(_ptr));
			}

			const_iterator begin() const {
				return (const_iterator(_ptr));
			}

			iterator end() {
				return (iterator(_ptr + _size));
			}
			
			const_iterator end() const {
				return (const_iterator(_ptr + _size));
			}

			reverse_iterator rbegin() {
				return (reverse_iterator(_ptr + _size));
			}

			const_reverse_iterator rbegin() const {
				return (const_reverse_iterator(_ptr + _size));
			}

			reverse_iterator rend() {
				return (iterator(_ptr));
			}

			const_reverse_iterator rend() const {
				return (const_iterator(_ptr));
			}
			/*
			 * CAPACITY
			 */
			size_type size() const {
				return (_size);
			}

			size_type max_size() const {
				return (_alloc.max_size());
			};

			void resize (size_type n, value_type val = value_type()) {
				if (n > _capacity || n > _size) {
					if (n > _capacity)
						reserve(n);
					for (size_type i = _size; i < n; ++i)
						push_back(val);
				} else if (n < _size) {
					for (size_type i = n; i < _size; ++i)
						_alloc.destroy(_ptr + i);
					_size = n;
				}
			}

			size_type capacity() const {
				return (_capacity);
			}
			
			bool empty() const {
				return (_size == 0);
			}

			// LENGHT ERROR EXCEPTION THROWN when capacity is > max_capacity?
			// https://cplusplus.com/reference/stdexcept/length_error/
			// https://cplusplus.com/reference/vector/vector/capacity/
			void reserve (size_type n) {
				if (n > _capacity) {
					pointer tmp = _alloc.allocate(n);
					for (size_type i = 0; i < _size; ++i)
						*(tmp + i) = *(_ptr + i);
					size_type tmp_size = _size;
					clear();
					_alloc.deallocate(_ptr, _capacity);
					_ptr = tmp;
					_size = tmp_size;
					_capacity = n;
				}
			}

			/*
			 * ELEMENT ACCESS
			 */

			// reference operator[] (size_type n);
			// const_reference operator[] (size_type n) const;

			/*
			 * MODIFIER
			 */
			void push_back (const value_type& val) {
				if (!_capacity)
					reserve(1);
				else if (_size >= _capacity)
					reserve(2 * _capacity);
				_alloc.construct(_ptr + _size++, val);
			}

			void clear() {
				for (size_type i = 0; i < _size; i++)
					_alloc.destroy(_ptr + i);
				_size = 0;
			}


		private:
			allocator_type  _alloc;
			size_type		_size;
			size_type		_capacity;
			pointer			_ptr;
	};
}
