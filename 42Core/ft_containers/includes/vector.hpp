#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <typeinfo>
#include <sstream>
#include "iterator.hpp"
#include "reverse_iterator.hpp"
#include "enable_if.hpp"

#ifndef LINUX
# define LINUX 0
#endif

// #include "enable_if.hpp"
// template<class T, class Allocator = std::allocator<T> >

// https://cplusplus.com/reference/vector/vector/vector/
// https://cplusplus.com/reference/vector/vector/?kw=vector
// https://cplusplus.com/reference/memory/allocator/
// https://cplusplus.com/reference/memory/allocator/allocate/
// https://cplusplus.com/reference/memory/allocator/deallocate/
// https://en.cppreference.com/w/cpp/language/sfinae
// https://cplusplus.com/reference/stdexcept/length_error/
// https://cplusplus.com/reference/vector/vector/capacity/
// https://cplusplus.com/reference/stdexcept/out_of_range/

namespace ft {
	template < class T, class Alloc = std::allocator<T> >
	class vector {
		public:
			typedef T											value_type;
			typedef Alloc										allocator_type;
			typedef std::size_t									size_type;
			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			typedef ft::iterator<T>								iterator;
			typedef ft::iterator<const T>						const_iterator;
			typedef ft::reverse_iterator<T>						reverse_iterator; //<iterator>
			typedef ft::reverse_iterator<const T>				const_reverse_iterator;//<const_iterator>
			typedef typename iterator::difference_type			difference_type;

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
				if (n > _capacity)
					reserve(n);
				if (n > _size) {
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

			void reserve (size_type n) {
				if (n > _capacity) {
					pointer tmp = _alloc.allocate(n);
					for (size_type i = 0; i < _size; ++i)
							_alloc.construct(tmp + i, *(_ptr + i));
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
			reference operator[] (size_type n) {
				return (*(_ptr + n));
			}

			const_reference operator[] (size_type n) const {
				return (*(_ptr + n));
			}

			reference at (size_type n) {
				if (n >= _size)
					throw std::out_of_range(_at_exception_msg(n));
				return (*(_ptr + n));
			}

			const_reference at (size_type n) const {
				if (n >= _size)
					throw std::out_of_range(_at_exception_msg(n));
				return (*(_ptr + n));
			}

			reference front() {
				return (*_ptr);
			}
			
			const_reference front() const {
				return (*_ptr);
			}

			reference back() {
				return (*(_ptr + _size - 1));
			}
			const_reference back() const {
				return (*(_ptr + _size - 1));
			}
			/*
			 * MODIFIER
			 */
			template <class InputIterator>
			void assign (InputIterator first, InputIterator last, typename enable_if<!is_integral<InputIterator>::value>::type * = 0) {
				clear();
				while(first != last) {
					push_back(*first);
				}
			}

			void assign (size_type n, const value_type& val) {
				clear();
				while(n--)
					push_back(val);
			}

			void push_back (const value_type& val) {
				if (!_capacity)
					reserve(1);
				else if (_size >= _capacity)
					reserve(2 * _capacity);
				_alloc.construct(_ptr + _size++, val);
			}

			void pop_back() {
				if (!empty()) {
					_alloc.destroy(_ptr + _size - 1);
					_size--;
				}
			}

			iterator insert (iterator position, const value_type& val) {
				size_type idx = position - begin();
	            while (_capacity < _size + 1)
	            	reserve(_capacity * 2);
	            pointer ptr = _ptr + idx;
	            std::copy(ptr, _ptr + _size, ptr + 1);
	            _alloc.construct(ptr, val);
	           	_size++;
	            return (ptr);

			}

    		void insert (iterator position, size_type n, const value_type& val) {
				size_type idx = position - begin();
	            while (_capacity < _size + n)
	            	reserve(_capacity * 2);
	            pointer ptr = _ptr + idx;
	            std::copy(ptr, _ptr + _size, ptr + n);
	            while (n--) {
	            	_alloc.construct(ptr + n, val);
	           		_size++;
	            }
    		}

			template <class InputIterator>
			void insert (iterator position, InputIterator first, InputIterator last, typename enable_if<!is_integral<InputIterator>::value>::type * = 0) {
				size_type n = last - first;
				size_type idx = position - begin();
	            while (_capacity < _size + n)
	            	reserve(_capacity * 2);
	            pointer ptr = _ptr + idx;
	            std::copy(ptr, _ptr + _size, ptr + n);
	            for (size_type i = 0; first != last && i < n; first++, _size++, i++)
	            	_alloc.construct(ptr + i, *first);
			}

			iterator erase (iterator position) {
				size_type idx = position - begin();
	            pointer ptr = _ptr + idx;
	            std::copy(ptr + 1, _ptr + _size, ptr);
	            pop_back();
	            return (position);
			}

			iterator erase (iterator first, iterator last) {
				size_type idx = first - begin();
				size_type elems = last - first;
	            pointer ptr = _ptr + idx;
	            std::copy(ptr + idx + elems, _ptr + _size, ptr);
	            while (first++ != last)
	            	pop_back();
				return (first);
			}

			void clear() {
				for (size_type i = 0; i < _size; i++)
					_alloc.destroy(_ptr + i);
				_size = 0;
			}

			void swap (vector& x) {
        		std::swap(_ptr, x._ptr);
            	std::swap(_capacity, x._capacity);
            	std::swap(_size, x._size);
			}
			/*
			 * ALLOCATOR
			 */
			allocator_type get_allocator() const {
				return (allocator_type(_alloc));
			}

		private:
			const std::string _at_exception_msg(size_type n) const {
				if (LINUX) {
					std::stringstream ss;
					ss << "vector::_M_range_check: __n (which is " << n << ") >= this->size() (which is " << _size << ")";
					return (ss.str());
				} else {
					return ("vector");
				}
			}

			// const void _shift(iterator start_pos =, size_type n = 1) {
			// 	while (_capacity < _size + n)
			// 		reserve(_capacity * 2);

			// 	vector tmp_vec = *this;
			// 	iterator it = start_pos;
			// 	for (size_type i = 0; i < n; i++)
			// 		_sizeAlloc();
			// 	for (size_type i = start + n; i < size(); it++, i++)
			// 		_setValue(_buff + i, *it);
			// }

			allocator_type  _alloc;
			size_type		_size;
			size_type		_capacity;
			pointer			_ptr;
	};
	/*
	 * NON-MEMBER FUNCTION OVERLOADS
	 */
	template <class T, class Alloc>
	bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		if (lhs.size() == rhs.size())
			return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
		return (false);
	}

	template <class T, class Alloc>
	bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		return (!(lhs == rhs));
	}

	template <class T, class Alloc>
	bool operator< (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		if (lhs.size() == rhs.size()) {
			for (unsigned long int i = 0; i < lhs.size(); ++i)
				if (lhs[i] != rhs[i])
					return (lhs[i] < rhs[i]);
			return (false);
		}
		return (lhs.size() < rhs.size());
	}

	template <class T, class Alloc>
	bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		if (lhs.size() == rhs.size()) {
			for (unsigned long int i = 0; i < lhs.size(); ++i)
				if (lhs[i] != rhs[i])
					return (lhs[i] <= rhs[i]);
			return (true);
		}
		return (lhs.size() < rhs.size());
	}

	template <class T, class Alloc>
	bool operator> (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		if (lhs.size() == rhs.size()) {
			for (unsigned long int i = 0; i < lhs.size(); ++i)
				if (lhs[i] != rhs[i])
					return (lhs[i] > rhs[i]);
			return (false);
		}
		return (lhs.size() > rhs.size());
	}

	template <class T, class Alloc>
	bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
		if (lhs.size() == rhs.size()) {
			for (unsigned long int i = 0; i < lhs.size(); ++i)
				if (lhs[i] != rhs[i])
					return (lhs[i] >= rhs[i]);
			return (true);
		}
		return (lhs.size() > rhs.size());
	}

	template <class T, class Alloc>
	void swap (vector<T,Alloc>& x, vector<T,Alloc>& y) {
		x.swap(y);
	}
}
