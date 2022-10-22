// https://cplusplus.com/reference/map/map/
// https://cplusplus.com/reference/map/map/map/

#ifndef MAP_HPP
# define MAP_HPP

#include "rbt.hpp"
#include "iterator.hpp"
#include "iterator_traits.hpp"
#include "reverse_iterator.hpp"
#include "lexicographical_compare.hpp"
#include "pair.hpp"
#include "rbt.hpp"
#include "equal.hpp"

# ifndef LINUX
#  define LINUX 0
# endif


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
		using typename	BTBase::key_type;
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
		i_swap(x._alloc, y._alloc);
		i_swap(x._root, y._root);
		i_swap(x._first, y._first);
		i_swap(x._last, y._last);
		i_swap(x._size, y._size);
	}

}

#endif
