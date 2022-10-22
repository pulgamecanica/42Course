#ifndef SET_HPP
# define SET_HPP

# include "rbt.hpp"

namespace ft {

template < class Key, class Compare = std::less<Key>, class Alloc = std::allocator<Key> >
	class set : public virtual RedBlackTree<const Key, const Key, const Key, Compare, Alloc>  {
	public:
		typedef RedBlackTree<const Key, const Key, const Key, Compare, Alloc>	Base;
		typedef Key					key_type;
		typedef Key					value_type;
		typedef	Compare				key_compare;
		typedef	Compare				value_compare;
		typedef Alloc				allocator_type;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef value_type*			pointer;
		typedef	const value_type*	const_pointer;
		using typename				Base::btnode;
		using typename				Base::node_pointer;
		using typename				Base::iterator;
		using typename				Base::const_iterator;
		using typename				Base::reverse_iterator;
		using typename				Base::const_reverse_iterator;
		using typename				Base::difference_type;
		using typename				Base::size_type;

		explicit set(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
				: Base(comp, alloc) {
		}
		template <class InputIterator>
		set (InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type(),
					typename ft::check_type<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0)
				: Base(comp, alloc) {
			this->insert(first, last);
		}
		set (const set& x) : Base() {
			this->insert(x.begin(), x.end());
		}
		virtual	~set() { }

		set& operator= (const set& x) {
			if (this != &x) {
				this->clear();
				this->_alloc = x._alloc;
				this->_cmp = x._cmp;
				this->insert(x.begin(), x.end());
			}
			return (*this);
		}
		virtual size_type	max_size() const {
			return this->_alloc.max_size() / sizeof(pointer);
		}

		ft::pair<iterator, bool> insert(const value_type& val) {
			if (this->empty())
				return (std::make_pair(iterator(Base::insert_root(val)), true));
			node_pointer it(this->_root);
			while (it) {
				if (this->key_comp()(val, it->data)) {
					if (it->left && it->left != this->_first)
						it = it->left;
					else
						return std::make_pair(iterator(Base::insert_left(it, val)), true);
				}
				else if (key_compare()(it->data, val)) {
					if (it->right && it->right != this->_last)
						it = it->right;
					else
						return std::make_pair(iterator(Base::insert_right(it, val)), true);
				}
				else break ;
			}
			return std::make_pair(iterator(it), false);
		}

		iterator insert(iterator position, const value_type& val) {
			(void)position;
			return this->insert(val).first;
		}

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last, typename ft::check_type<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0) {
			while (first != last) {
				this->insert(*first);
				++first;
			}
		}
	};
}

#endif
