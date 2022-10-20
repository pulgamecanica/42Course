#ifndef RBT_HPP
# define RBT_HPP

# include <memory>
# include <functional>
# include "reverse_iterator.hpp"
# include "bt_iterator.hpp"
# include "node.hpp"

namespace ft
{
    template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T> >
    class RedBlackTree {
    public:
        // use rebind to change the T allocator to NodeBase<T> allocator
        typedef typename Alloc::template rebind<ft::Node<T> >::other   allocator_type;
        typedef T                                       value_type;
        typedef value_type&                             reference;
        typedef const value_type&                       const_reference;
        typedef value_type*                             pointer;
        typedef const value_type*                       const_pointer;
        typedef Compare                                 value_compare;
        typedef ft::BTIterator<T>                       iterator;
        typedef ft::BTIterator<const T>                 const_iterator;
        typedef ft::reverse_iterator<iterator>          reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>    const_reverse_iterator;
        typedef typename iterator::difference_type      difference_type;
        typedef typename allocator_type::size_type      size_type;
        typedef ft::Node<T>                             node_value;                    
        typedef node_value*                             node_pointer;

        RedBlackTree(
            const value_compare& cmp = value_compare(),
            const allocator_type& alloc = allocator_type()) :
            _alloc(alloc),
            _cmp(cmp),
            _root(NULL)
        {}

        RedBlackTree(const RedBlackTree& rhs) :
            _alloc(rhs._alloc),
            _cmp(rhs._cmp)
        {
            // _updateRoot(_copy(rhs._root));
        }

        virtual RedBlackTree& operator=(const RedBlackTree& rhs) {
            // if (this != &rhs) {
            //     this->clear();
            //     _updateRoot(_copy(rhs._root));
            // }
            (void)rhs;
            return (*this);
        }
        
        virtual ~RedBlackTree() {
            // this->clear();
        }


        /* Iterators */
        iterator begin() {
            return (this->minimum());
        }

        const_iterator begin() const {
            return (this->minimum());
        }

        iterator end() {
            return (_last);
        }

        const_iterator end() const {
            return (_last);
        }

        reverse_iterator rbegin() {
            return (this->end());
        }

        const_reverse_iterator rbegin() const {
            return (this->end());
        }

        reverse_iterator rend() {
            return (this->begin());
        }

        const_reverse_iterator rend() const {
            return (this->begin());
        }

        /* Capacity */
        bool empty() const {
            return (!_root);
        }

        size_type size() const {
            return (_root ? _root->size() : 0);
        }

        size_type max_size() const {
            return (_alloc.max_size());
        }


        /* Modifiers */
        virtual iterator insert(const value_type& val) {
            node_pointer p, q;
            node_pointer t = _create_node(val);
            // _root = t;
            t->color = B_RED;

            p = _root;
            q = NULL;
            if(!_root) {
               _root = t;
               t->parent = NULL;
            } else {
                while (p) {
                    q = p;
                    if(_cmp(Node<T>::getData(p), Node<T>::getData(t)))
                        p = p->right;
                    else
                        p = p->left;
                }
                t->parent = q;
                if(_cmp(Node<T>::getData(q), Node<T>::getData(t)))
                    q->right = t;
                else
                    q->left = t;
            }
            insertfix(t);
            return (t);
        }

        void insertfix(node_pointer t) {
            node_pointer u;

            if(_root == t) {
                t->color = B_BLACK;
                return;
            }
            while(t->parent && t->parent->color == B_RED) {
                node_pointer g = t->parent->parent;
                if (g->left == t->parent) {
                    if(g->right) {
                        u=g->right;
                        if(u->color == B_RED) {
                            t->parent->color = B_BLACK;
                            u->color = B_BLACK;
                            g->color = B_RED;
                            t = g;
                        }
                    } else {
                        if(t->parent->right==t) {
                             t = t->parent;
                             leftrotate(t);
                        }
                        t->parent->color = B_BLACK;
                        g->color=B_RED;
                        rightrotate(g);
                    }
                } else {
                    if(g->left != NULL) {
                        u = g->left;
                        if(u->color == B_RED) {
                            t->parent->color = B_BLACK;
                            u->color = B_BLACK;
                            g->color = B_RED;
                            t = g;
                        }
                    } else {
                        if(t->parent->left==t) {
                           t=t->parent;
                           rightrotate(t);
                        }
                        t->parent->color=B_BLACK;
                        g->color=B_RED;
                        leftrotate(g);
                    }
                }
                _root->color=B_BLACK;
            }
        }
        


        void leftrotate(node_pointer p) {
            if(!p->right)
                return ;
            else {
                node_pointer y = p->right;
                if(y->left) {
                    p->right = y->left;
                    y->left->parent = p;
                } else
                    p->right = NULL;
                if(p->parent)
                    y->parent = p->parent;
                if(!p->parent)
                    _root=y;
                else {
                    if(p == p->parent->left)
                        p->parent->left = y;
                    else
                        p->parent->right = y;
                }
                y->left = p;
                p->parent = y;
            }
        }

        void rightrotate(node_pointer p) {
            if(p->left == NULL)
                return ;
            else {
                node_pointer y = p->left;
                if(y->right) {
                    p->left = y->right;
                    y->right->parent = p;
                } else
                    p->left = NULL;
                if(p->parent)
                    y->parent = p->parent;
                if(!p->parent)
                    _root = y;
                else {
                    if(p == p->parent->left)
                        p->parent->left = y;
                    else
                        p->parent->right = y;
                }
                y->right = p;
                p->parent = y;
            }
        }















        void erase(const value_type& val) {
            // _erase(this->_root, val);
            (void)val;
        }

        void erase(const_iterator position) {
            // _erase(position._node);
            (void) position;
        }

        // swap method
        void swap(RedBlackTree& rhs) {
            std::swap(this->_root, rhs._root);
            std::swap(this->_last, rhs._last);
            std::swap(this->_cmp, rhs._cmp);
            std::swap(this->_alloc, rhs._alloc);
        }

        void clear() {
            // _clear(this->_root);
        }

        /* Observers */
        value_compare value_comp() const {
            return (_cmp);
        }

        allocator_type get_allocator() const {
            return (_alloc);
        }

        /* Operations */

        iterator find(const value_type& val) const {
            // return _find(this->_root, val);
            (void)val;
        }

        iterator find(const_iterator hint, const value_type& val) const {
            // if (hint != this->end() && _cmp(*hint, val)) // check if the hint is valid
            // {
            //     iterator max = hint._node->maximum();
            //     if (_cmp(val, *max)) // check if the value is under the hint._node subtree
            //         return _find(hint._node->right, val); // find in the right subtree
            //     return this->find(++max, val); // find in the upper subtree
            // }
            // return this->find(val);
            (void)val;
            (void)hint;
        }

        // iterator lower_bound(const value_type& val) const
        // {
        //     node_pointer node = _nil;
        //     _lower_bound(this->_root, val, node);
        //     return node;
        // }

        // iterator upper_bound(const value_type& val) const
        // {
        //     node_pointer node = _nil;
        //     _upper_bound(this->_root, val, node);
        //     return node;
        // }

        size_type height() const {
            if (!_root)
                return (0);
            return (_root->height());
        }

        size_type bheight() const {
            if (!_root)
                return (0);
            return (_root->bheight());
        }
        
        node_pointer minimum() const {
            if (!_root)
                return (_last);
            return (_root->min());
        }

        node_pointer maximum() const {
            if (!_root)
                return (_last);
            return (_root->max());
        }

        node_pointer root() const {
            return (_root);
        }

        void displayRBT() {
            std::cout << "Stats" << std::endl;
            std::cout << "Height: " << height() << " | BHeight: " << bheight() << std::endl;
            std::cout << "Min: " << Node<T>::getData(minimum()) << " | Max: " << Node<T>::getData(maximum()) << std::endl;
            Node<T>::display(_root);
        }

        protected:
        node_pointer _create_node(
            const value_type& val = value_type(),
            const node_pointer& parent = NULL,
            const node_pointer& left = NULL,
            const node_pointer& right = NULL
        ) {
            node_value *node = _alloc.allocate(1);
            _alloc.construct(node, val);
            // node->null = false;
            node->parent = parent ? parent : _last;
            node->left = left ? left : _last;
            node->right = right ? right : _last;
            return (node);
        }

        public:
        allocator_type _alloc;
        value_compare _cmp;
        node_pointer _root;
        node_pointer _last;
    };
    template <typename T, typename Compare, typename Alloc>
    void swap(RedBlackTree<T, Compare, Alloc>& lhs, RedBlackTree<T, Compare, Alloc>& rhs) {
        lhs.swap(rhs);
    }
}

#endif
