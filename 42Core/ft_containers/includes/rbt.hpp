// https://en.cppreference.com/w/cpp/utility/functional/less

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
            clear();
        }


        /* Iterators */
        iterator begin() { return (this->minimum()); }

        const_iterator begin() const { return (this->minimum()); }

        iterator end() { return (this->maximum()); }

        const_iterator end() const { return (this->maximum()); }

        reverse_iterator rbegin() { return (this->end()); }

        const_reverse_iterator rbegin() const { return (this->end()); }

        reverse_iterator rend() { return (this->begin()); }

        const_reverse_iterator rend() const { return (this->begin()); }

        /* Capacity */
        bool empty() const { return (!_root); }

        size_type size() const { return (_root ? _root->size() : 0); }

        size_type max_size() const { return (_alloc.max_size()); }

        /* Modifiers */
        virtual iterator insert(const value_type& val) {
            node_pointer tmp1, tmp2;
            node_pointer node = _create_node(val);
            node->color = B_RED;

            tmp1 = _root;
            tmp2 = NULL;
            if(empty()) {
                _root = node;
                node->parent = NULL;
            } else {
                while (tmp1) {
                    tmp2 = tmp1;
                    if(_cmp(Node<T>::getData(tmp1), Node<T>::getData(node)))
                        tmp1 = tmp1->right;
                    else
                        tmp1 = tmp1->left;
                }
                node->parent = tmp2;
                if(_cmp(Node<T>::getData(tmp2), Node<T>::getData(node)))
                    tmp2->right = node;
                else
                    tmp2->left = node;
            }
            insertfix(node);
            return (node);
        }

        void insertfix(node_pointer node) {
            node_pointer tmp;

            if(_root == node) {
                node->color = B_BLACK;
                return;
            }
            while(node->parent && node->parent->color == B_RED) {
                node_pointer sub_tmp = node->parent->parent;
                if (sub_tmp->left == node->parent) {
                    if(sub_tmp->right) {
                        tmp=sub_tmp->right;
                        if(tmp->color == B_RED) {
                            node->parent->color = B_BLACK;
                            tmp->color = B_BLACK;
                            sub_tmp->color = B_RED;
                            node = sub_tmp;
                        }
                    } else {
                        if(node->parent->right==node) {
                             node = node->parent;
                             leftrotate(node);
                        }
                        node->parent->color = B_BLACK;
                        sub_tmp->color = B_RED;
                        rightrotate(sub_tmp);
                    }
                } else {
                    if(sub_tmp->left != NULL) {
                        tmp = sub_tmp->left;
                        if(tmp->color == B_RED) {
                            node->parent->color = B_BLACK;
                            tmp->color = B_BLACK;
                            sub_tmp->color = B_RED;
                            node = sub_tmp;
                        }
                    } else {
                        if(node->parent->left==node) {
                           node=node->parent;
                           rightrotate(node);
                        }
                        node->parent->color=B_BLACK;
                        sub_tmp->color = B_RED;
                        leftrotate(sub_tmp);
                    }
                }
                _root->color=B_BLACK;
            }
        }

        void erase(const value_type& val) {
            if(empty()) {
                return ;
            }
            node_pointer tmp1, y, q;

            tmp1 = find(val);
            y = NULL;
            q = NULL;
            
            if(!tmp1)
                return ;
            if(tmp1->left == NULL || tmp1->right == NULL)
                y=tmp1;
            else
                y = successor(tmp1);
            if(y->left)
                q = y->left;
            else {
                if(y->right)
                    q = y->right;
                else
                q = NULL;
            }
            if(q)
                q->parent=y->parent;
            if(!y->parent)
                _root = q;
            else {
                if(y == y->parent->left)
                    y->parent->left = q;
                else
                y->parent->right = q;
            }
            if(y != tmp1) {
                tmp1->color = y->color;
                tmp1->data = y->data;
            }
            if(y->color == B_BLACK)
                delfix(q);
            _destroy_node(tmp1);
        }

        void delfix(node_pointer p) {
            node_pointer s;
            while(p != _root && p->color == B_BLACK) {
                if(p->parent->left == p) {
                    s = p->parent->right;
                    if(s->color == B_RED) {
                        s->color = B_BLACK;
                        p->parent->color = B_RED;
                        leftrotate(p->parent);
                        s=p->parent->right;
                    }
                    if(s->right->color == B_BLACK && s->left->color == B_BLACK) {
                        s->color = B_RED;
                        p = p->parent;
                    } else {
                        if(s->right->color == B_BLACK) {
                            s->left->color = B_BLACK;
                            s->color = B_RED;
                            rightrotate(s);
                            s = p->parent->right;
                        }
                        s->color = p->parent->color;
                        p->parent->color = B_BLACK;
                        s->right->color = B_BLACK;
                        leftrotate(p->parent);
                        p = _root;
                    }
                } else { 
                    s = p->parent->left;
                    if(s->color == B_RED) {
                        s->color = B_BLACK;
                        p->parent->color = B_RED;
                        rightrotate(p->parent);
                        s = p->parent->left;
                    }
                    if(s->left->color == B_BLACK && s->right->color == B_BLACK) {
                        s->color = B_RED;
                        p = p->parent;
                    } else {
                        if(s->left->color == B_BLACK) {
                        s->right->color = B_BLACK;
                        s->color = B_RED;
                        leftrotate(s);
                        s = p->parent->left;
                        }
                        s->color = p->parent->color;
                        p->parent->color = B_BLACK;
                        s->left->color = B_BLACK;
                        rightrotate(p->parent);
                        p = _root;
                    }
                }
                p->color = B_BLACK;
                _root->color = B_BLACK;
            }
        }

        node_pointer successor(node_pointer p) {
            node_pointer y = NULL;
            if(p->left) {
                y = p->left;
                while(y->right)
                    y = y->right;
            } else {
                y = p->right;
                while(y->left)
                    y = y->left;
            }
            return y;
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

        void erase(const_iterator position) {
            erase(position.node);
        }

        // swap method
        void swap(RedBlackTree& rhs) {
            std::swap(this->_root, rhs._root);
            std::swap(this->_cmp, rhs._cmp);
            std::swap(this->_alloc, rhs._alloc);
        }

        void clear() {
            _clear(_root);
        }

        void _clear(node_pointer node) {
            if (empty() || !node)
                return ;
            _clear(node->left);
            _clear(node->right);
            _destroy_node(node);
            node = NULL;
        }

        /* Observers */
        value_compare value_comp() const {
            return (_cmp);
        }

        allocator_type get_allocator() const {
            return (_alloc);
        }

        /* Operations */
        node_pointer find(const value_type& val) const {
            node_pointer p =_root;
            while (p) {
                if(p->data == val) {
                    return (p);
                }
                if(_cmp(p->data, val))
                    p = p->right;
                else
                    p = p->left;
            }
            return (NULL);
        }

        size_type height() const {
            if (empty())
                return (0);
            return (_root->height());
        }

        size_type bheight() const {
            if (empty())
                return (0);
            return (_root->bheight());
        }
        
        node_pointer minimum() const {
            if (empty())
                return (NULL);
            return (_root->min());
        }

        node_pointer maximum() const {
            if (empty())
                return (NULL);
            return (_root->max());
        }

        node_pointer root() const { return (_root); }

        void displayRBT() {
            std::cout << "Size: " << _root->size() << " | Height: " << _root->height() << " | BHeight: " << _root->bheight() << std::endl;
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
            node->parent = parent ? parent : NULL;
            node->left = left ? left : NULL;
            node->right = right ? right : NULL;
            return (node);
        }

        void _destroy_node(node_pointer node) {
            if (empty() || !node)
                return ;
            _alloc.destroy(node);
            // _alloc.deallocate(node, 1);
        }

        public:
        allocator_type _alloc;
        value_compare _cmp;
        node_pointer _root;
    };
    template <typename T, typename Compare, typename Alloc>
    void swap(RedBlackTree<T, Compare, Alloc>& lhs, RedBlackTree<T, Compare, Alloc>& rhs) {
        lhs.swap(rhs);
    }
}

#endif
