#include <stdexcept>
#include <iostream>
#include <typeinfo>

// #include "enable_if.hpp"
// template<class T, class Allocator = std::allocator<T> >


namespace ft {
	template < class T, class Alloc = std::allocator<T> >
	class vector {
		public:
			typedef T value_type;
			typedef Alloc allocator_type;
			typedef std::size_t size_type;
			typedef typename allocator_type::pointer pointer;

			explicit vector (const allocator_type& alloc = allocator_type()): _alloc(alloc), _size(0), _capacity(0), _buff(pointer()) {} ;
			explicit vector (size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()): _alloc(alloc), _size(n), _capacity(n) {
				if (n > max_size())
					throw std::length_error("cannot create a vector with a size larger than max_size");
				_buff = _alloc.allocate(n);
				for (size_type i(0); i < n; i++)
					_alloc.construct(_buff + i, val);
			};



			size_type max_size() const {
				return _alloc.max_size();
			};
		private:
			allocator_type  _alloc;
			size_type		_size;
			size_type		_capacity;
			pointer			_buff;
	};
}
