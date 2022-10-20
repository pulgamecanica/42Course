// #include <algorithm>
// #include <stdexcept>
// #include <iostream>
// #include <typeinfo>
// #include <sstream>
// #include "iterator.hpp"
// #include "iterator_traits.hpp"
// #include "reverse_iterator.hpp"
// #include "lexicographical_compare.hpp"
// #include "enable_if.hpp"
// #include "equal.hpp"
#include "pair.hpp"

#include "rbt.hpp"

#ifndef MAP_CPP
# define MAP_CPP

# ifndef LINUX
#  define LINUX 0
# endif

// https://cplusplus.com/reference/map/map/
// https://cplusplus.com/reference/map/map/map/

namespace ft {
	template <class Key, class T, class Compare = std::less<Key>, class Alloc = std::allocator<std::pair<const Key,T> > >
	class map {
		public:
			typedef Key												key_type;
	        typedef T												mapped_type;
	        typedef std::pair<const key_type, mapped_type>			value_type;
	        typedef Compare											key_compare;
	        typedef Alloc											allocator_type;
	        
		private:
	};
}

#endif
