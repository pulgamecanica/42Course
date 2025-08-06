#ifndef FT_MAP_HPP
#define FT_MAP_HPP

#include <memory>
#include "utility.hpp"
#include "utils/rbt.hpp"
#include "utils/less.hpp"
#include "utils/lexicographical_compare.hpp"
#include "iterators/reverse_iterator.hpp"
#include "iterators/bt_iterator.hpp"

#include <iostream>

namespace ft {

template <typename Pair>
struct Select1st {
  typedef typename Pair::first_type result_type;
  const result_type& operator()(const Pair& p) const {
      return p.first;
  }
};

template <
  class Key,
  class T,
  class Compare = ft::less<Key>,
  class Alloc = std::allocator<ft::pair<const Key, T> >
>
class map {
public:
  typedef Key                                     key_type;
  typedef T                                       mapped_type;
  typedef ft::pair<const Key, T>                  value_type;
  typedef Compare                                 key_compare;
  typedef Alloc                                   allocator_type;
  typedef value_type&                             reference;
  typedef const value_type&                       const_reference;
  typedef typename allocator_type::pointer        pointer;
  typedef typename allocator_type::const_pointer  const_pointer;
  typedef ptrdiff_t                               difference_type;
  typedef size_t                                  size_type;

private:
  typedef RedBlackTree<
      key_type,
      value_type,
      ft::Select1st<value_type>,
      key_compare,
      allocator_type,
      false // Multi = false, this is a map, not multimap
  > tree_type;

  tree_type _tree;

public:
  typedef typename tree_type::iterator                iterator;
  typedef typename tree_type::const_iterator          const_iterator;
  typedef typename tree_type::reverse_iterator        reverse_iterator;
  typedef typename tree_type::const_reverse_iterator  const_reverse_iterator;

  class value_compare;

public:
  // Constructors
  explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
  
  template <class InputIt>
  map(InputIt first, InputIt last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
  map(const map& other);
  map& operator=(const map& other);

  ~map() {
    std::cout << "DESTRUCTION OF THE MAP" << std::endl;
  }

  // Element access
  mapped_type& operator[](const key_type& key);
  mapped_type& at(const key_type& key);
  const mapped_type& at(const key_type& key) const;

  // Iterators
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;

  // Capacity
  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  // Modifiers
  void clear();
  ft::pair<iterator, bool> insert(const value_type& val);
  iterator insert(iterator hint, const value_type& val);
  template <class InputIt>
  void insert(InputIt first, InputIt last);
  void erase(iterator pos);
  size_type erase(const key_type& key);
  void erase(iterator first, iterator last);
  void swap(map& other);

  // Lookup
  size_type count(const key_type& key) const;
  iterator find(const key_type& key);
  const_iterator find(const key_type& key) const;
  iterator lower_bound(const key_type& key);
  const_iterator lower_bound(const key_type& key) const;
  iterator upper_bound(const key_type& key);
  const_iterator upper_bound(const key_type& key) const;
  ft::pair<iterator, iterator> equal_range(const key_type& key);
  ft::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

  // Observers
  key_compare key_comp() const;
  value_compare value_comp() const;

  // Allocator
  allocator_type get_allocator() const;
};

// value_compare nested class
template <class Key, class T, class Compare, class Alloc>
class map<Key, T, Compare, Alloc>::value_compare {
  friend class map;
protected:
  Compare comp;
  explicit value_compare(Compare c) : comp(c) {}
public:
  bool operator()(const value_type& lhs, const value_type& rhs) const {
      return comp(lhs.first, rhs.first);
  }
};

template <class Key, class T, class Compare, class Alloc>
bool operator==(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
  return (lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template <class Key, class T, class Compare, class Alloc>
bool operator!=(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
  return !(lhs == rhs);	
}

template <class Key, class T, class Compare, class Alloc>
bool operator< (const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
  return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class Key, class T, class Compare, class Alloc>
bool operator<=(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
  return !(lhs > rhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator> (const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
  return (rhs < lhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator>=(const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs) {
    return !(lhs < rhs);
}

template <class Key, class T, class Compare, class Alloc>
void swap(ft::map<Key, T, Compare, Alloc>& lhs, ft::map<Key, T, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}

} // namespace ft

#endif


// #ifndef asssa
// #define asssa

// #include <memory>
// #include "utility.hpp"
// #include "utils/less.hpp"
// #include "iterators/reverse_iterator.hpp"


// #include "utils/rbt.hpp"
// #include "iterators/bt_iterator.hpp"
// #include "iterators/iterator_traits.hpp"
// #include "iterators/reverse_iterator.hpp"
// #include "utils/lexicographical_compare.hpp"
// #include "utility.hpp"
// #include "utils/algorithm.hpp"

// namespace ft {

// template < class Key, class Value, class Compare = std::less<Key>, class Alloc = std::allocator<ft::pair<const Key,Value> > >
// class map : public virtual RedBlackTree<const Key, Value, ft::pair<const Key, Value>, Compare, Alloc> {
// 	public:
// 		typedef RedBlackTree<const Key, Value, ft::pair<const Key, Value>, Compare, Alloc>	BTBase;
// 		using typename	BTBase::allocator_type;
// 		using typename	BTBase::btnode;
// 		using typename	BTBase::const_iterator;
// 		using typename	BTBase::const_reverse_iterator;
// 		using typename	BTBase::const_pointer;
// 		using typename	BTBase::const_reference;
// 		using typename	BTBase::difference_type;
// 		using typename	BTBase::iterator;
// 		using typename	BTBase::key_compare;
// 		typedef Key			key_type;
// 		// using typename	BTBase::key_type;
// 		using typename	BTBase::mapped_type;
// 		using typename	BTBase::node_pointer;
// 		using typename	BTBase::reference;
// 		using typename	BTBase::reverse_iterator;
// 		using typename	BTBase::pointer;
// 		using typename	BTBase::size_type;
// 		using typename	BTBase::value_type;

// 		explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
// 				: BTBase(comp, alloc) {}

// 		template <class InputIterator>
// 		map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(),
// 					typename ft::check_type<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0)
// 				: BTBase(comp, alloc)
// 		{ insert(first, last); }

// 		map (const map& x) : BTBase() { insert(x.begin(), x.end()); }

// 		virtual	~map() {}

// 		map& operator= (const map& x) {
// 			if (this != &x) {
// 				this->clear();
// 				this->_alloc = x._alloc;
// 				this->_cmp = x._cmp;
// 				insert(x.begin(), x.end());
// 			}
// 			return (*this);
// 		}

// 		mapped_type& at(const key_type& key) {
// 			iterator it = this->find(key);
// 			if (it == BTBase::end())
// 				throw std::out_of_range("map::at:  key not found");
// 			return it->second;
// 		}
// 		const mapped_type& at(const key_type& key) const {
// 			const_iterator it = this->find(key);
// 			if (BTBase::end() == it )
// 				throw std::out_of_range("map::at:  key not found");
// 			return it->second;
// 		}

// 		mapped_type& operator[](const key_type& key) {
// 			return insert(ft::make_pair(key, mapped_type())).first->second;
// 		}

// 		ft::pair<iterator, bool> insert(const value_type& val) {
// 			if (this->empty())
// 				return (ft::make_pair(iterator(BTBase::insert_root(val)), true));
// 			node_pointer it(this->_root);
// 			while (it) {
// 				if (key_compare()(val.first, it->data.first)) {
// 					if (it->left && it->left != this->_first)
// 						it = it->left;
// 					else
// 						return ft::make_pair(iterator(BTBase::insert_left(it, val)), true);
// 				}
// 				else if (key_compare()(it->data.first, val.first)) {
// 					if (it->right && it->right != this->_last)
// 						it = it->right;
// 					else
// 						return ft::make_pair(iterator(BTBase::insert_right(it, val)), true);
// 				}
// 				else break ;
// 			}
// 			return ft::make_pair(iterator(it), false);
// 		}

// 		iterator insert(iterator position, const value_type& val) {
// 			(void)position;
// 			return insert(val).first;
// 		}

// 		template <class InputIterator>
// 		void insert(InputIterator first, InputIterator last, typename ft::check_type<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0) {
// 			while (first != last) {
// 				insert(*first);
// 				++first;
// 			}
// 		}
// 	};

// 	template <class Key, class T, class Compare, class Alloc>
// 	bool operator== ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
// 		return (lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
// 	}
	
// 	template <class Key, class T, class Compare, class Alloc>
// 	bool operator!= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
// 		return !(lhs == rhs);	
// 	}
	
// 	template <class Key, class T, class Compare, class Alloc>
// 	bool operator<  ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
// 		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
// 	}
	
// 	template <class Key, class T, class Compare, class Alloc>
// 	bool operator<= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
// 		return !(lhs > rhs);
// 	}
	
// 	template <class Key, class T, class Compare, class Alloc>
// 	bool operator>  ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
// 		return (rhs < lhs);
// 	}
	
// 	template <class Key, class T, class Compare, class Alloc>
// 	bool operator>= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
// 			return !(lhs < rhs);
// 	}
	
// 	template <class Key, class T, class Compare, class Alloc>
// 	void swap (map<Key,T,Compare,Alloc>& x, map<Key,T,Compare,Alloc>& y) {
// 		ft::swap(x._alloc, y._alloc);
// 		ft::swap(x._root, y._root);
// 		ft::swap(x._first, y._first);
// 		ft::swap(x._last, y._last);
// 		ft::swap(x._size, y._size);
// 	}
// }

// #endif





