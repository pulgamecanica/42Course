#ifndef FT_MAP_HPP
#define FT_MAP_HPP

#include <memory>
#include "utility.hpp"
#include "utils/less.hpp"
#include "iterators/reverse_iterator.hpp"


#include "utils/rbt.hpp"
#include "iterators/bt_iterator.hpp"
#include "iterators/iterator_traits.hpp"
#include "iterators/reverse_iterator.hpp"
#include "utils/lexicographical_compare.hpp"
#include "utility.hpp"
#include "utils/algorithm.hpp"

namespace ft {

template < class Key, class Value, class Compare = std::less<Key>, class Alloc = std::allocator<ft::pair<const Key,Value> > >
class map : public virtual RedBlackTree<const Key, Value, ft::pair<const Key, Value>, Compare, Alloc> {
	public:
		typedef RedBlackTree<const Key, Value, ft::pair<const Key, Value>, Compare, Alloc>	BTBase;
		using typename	BTBase::allocator_type;
		using typename	BTBase::btnode;
		using typename	BTBase::const_iterator;
		using typename	BTBase::const_reverse_iterator;
		using typename	BTBase::const_pointer;
		using typename	BTBase::const_reference;
		using typename	BTBase::difference_type;
		using typename	BTBase::iterator;
		using typename	BTBase::key_compare;
		typedef Key			key_type;
		// using typename	BTBase::key_type;
		using typename	BTBase::mapped_type;
		using typename	BTBase::node_pointer;
		using typename	BTBase::reference;
		using typename	BTBase::reverse_iterator;
		using typename	BTBase::pointer;
		using typename	BTBase::size_type;
		using typename	BTBase::value_type;

		explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
				: BTBase(comp, alloc) {}

		template <class InputIterator>
		map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(),
					typename ft::check_type<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0)
				: BTBase(comp, alloc)
		{ insert(first, last); }

		map (const map& x) : BTBase() { insert(x.begin(), x.end()); }

		virtual	~map() {}

		map& operator= (const map& x) {
			if (this != &x) {
				this->clear();
				this->_alloc = x._alloc;
				this->_cmp = x._cmp;
				insert(x.begin(), x.end());
			}
			return (*this);
		}

		mapped_type& at(const key_type& key) {
			iterator it = this->find(key);
			if (it == BTBase::end())
				throw std::out_of_range("map::at:  key not found");
			return it->second;
		}
		const mapped_type& at(const key_type& key) const {
			const_iterator it = this->find(key);
			if (BTBase::end() == it )
				throw std::out_of_range("map::at:  key not found");
			return it->second;
		}

		mapped_type& operator[](const key_type& key) {
			return insert(ft::make_pair(key, mapped_type())).first->second;
		}

		ft::pair<iterator, bool> insert(const value_type& val) {
			if (this->empty())
				return (ft::make_pair(iterator(BTBase::insert_root(val)), true));
			node_pointer it(this->_root);
			while (it) {
				if (key_compare()(val.first, it->data.first)) {
					if (it->left && it->left != this->_first)
						it = it->left;
					else
						return ft::make_pair(iterator(BTBase::insert_left(it, val)), true);
				}
				else if (key_compare()(it->data.first, val.first)) {
					if (it->right && it->right != this->_last)
						it = it->right;
					else
						return ft::make_pair(iterator(BTBase::insert_right(it, val)), true);
				}
				else break ;
			}
			return ft::make_pair(iterator(it), false);
		}

		iterator insert(iterator position, const value_type& val) {
			(void)position;
			return insert(val).first;
		}

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last, typename ft::check_type<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0) {
			while (first != last) {
				insert(*first);
				++first;
			}
		}
	};

	template <class Key, class T, class Compare, class Alloc>
	bool operator== ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
		return (lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}
	
	template <class Key, class T, class Compare, class Alloc>
	bool operator!= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
		return !(lhs == rhs);	
	}
	
	template <class Key, class T, class Compare, class Alloc>
	bool operator<  ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}
	
	template <class Key, class T, class Compare, class Alloc>
	bool operator<= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
		return !(lhs > rhs);
	}
	
	template <class Key, class T, class Compare, class Alloc>
	bool operator>  ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
		return (rhs < lhs);
	}
	
	template <class Key, class T, class Compare, class Alloc>
	bool operator>= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs ) {
			return !(lhs < rhs);
	}
	
	template <class Key, class T, class Compare, class Alloc>
	void swap (map<Key,T,Compare,Alloc>& x, map<Key,T,Compare,Alloc>& y) {
		ft::swap(x._alloc, y._alloc);
		ft::swap(x._root, y._root);
		ft::swap(x._first, y._first);
		ft::swap(x._last, y._last);
		ft::swap(x._size, y._size);
	}
}

#endif






// namespace ft {

// template <class Key, class T, class Compare = ft::less<Key>,
//           class Alloc = std::allocator<ft::pair<const Key, T> > >
// class map {
// public:
//   typedef Key                                   key_type;
//   typedef T                                     mapped_type;
//   typedef ft::pair<const Key, T>                value_type;
//   typedef Compare                               key_compare;
//   typedef Alloc                                 allocator_type;
//   typedef typename Alloc::reference             reference;
//   typedef typename Alloc::const_reference       const_reference;
//   typedef ft::reverse_iterator<iterator>        iterator; // See 23.1
//   typedef ft::reverse_iterator<iterator>        const_iterator; // See 23.1
//   typedef std::size_t                           size_type; // See 23.1
//   typedef std::ptrdiff_t                        difference_type;// See 23.1
//   typedef typename Alloc::pointer               pointer;
//   typedef typename Alloc::const_pointer         const_pointer;
//   typedef ft::reverse_iterator<iterator>        reverse_iterator;
//   typedef ft::reverse_iterator<const_iterator>  const_reverse_iterator;

//   class value_compare {
//     friend class map;
//   protected:
//     Compare comp;
//     value_compare(Compare c) : comp(c) {}
//   public:
//     bool operator()(const value_type& x, const value_type& y) const {
//       return comp(x.first, y.first);
//     }
//   };


//   // 23.3.1.1 construct/copy/destroy:
//   explicit map(const Compare& comp = Compare(), const Alloc& = Alloc());

//   template <class InputIterator>
//   map(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Alloc& = Alloc());
  
//   map(const map<Key,T,Compare,Alloc>& x);
  
//   ~map();
  
//   map<Key,T,Compare,Alloc>&
  
//   operator=(const map<Key,T,Compare,Alloc>& x);
  
//   // iterators:
//   iterator begin();
  
//   const_iterator begin() const;
  
//   iterator end();
  
//   const_iterator end() const;
  
//   reverse_iterator rbegin();
  
//   const_reverse_iterator rbegin() const;
  
//   reverse_iterator rend();
  
//   const_reverse_iterator rend() const;
  
//   // capacity:
//   bool empty() const;
  
//   size_type size() const;
  
//   size_type max_size() const;
  
//   // 23.3.1.2 element access:
//   T& operator[](const key_type& x);
  
//   // modifiers:
//   ft::pair<iterator, bool> insert(const value_type& x);
  
//   iterator insert(iterator position, const value_type& x);
  
//   template <class InputIterator>
  
//   void insert(InputIterator first, InputIterator last);
  
//   void erase(iterator position);
  
//   size_type erase(const key_type& x);
  
//   void erase(iterator first, iterator last);
  
//   void swap(map<Key,T,Compare,Alloc>&);
  
//   void clear();
  
//   // observers:
//   key_compare key_comp() const;
  
//   value_compare value_comp() const;

//   // 23.3.1.3 map operations:
//   iterator find(const key_type& x);
  
//   const_iterator find(const key_type& x) const;
  
//   size_type count(const key_type& x) const;
  
//   iterator lower_bound(const key_type& x);
  
//   const_iterator lower_bound(const key_type& x) const;
  
//   iterator upper_bound(const key_type& x);
  
//   const_iterator upper_bound(const key_type& x) const;
  
//   ft::pair<iterator,iterator> equal_range(const key_type& x);
  
//   ft::pair<const_iterator,const_iterator> equal_range(const key_type& x) const;
// };

// } // namespace ft

// #endif // FT_MAP_HPP