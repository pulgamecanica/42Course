#ifndef BT_ITERATOR_HPP
# define BT_ITERATOR_HPP

# include "iterator_traits.hpp"
# include "node.hpp"


namespace ft
{
    /*
     * Helper templates to remove constant type.
     */
    template <typename T>
    struct remove_const
    {
        typedef T type;
    };

    template <typename T>
    struct remove_const<const T>
    {
        typedef T type;
    };

    /*
     * Binary Tree Iterator
     */
    template <typename T>
    class BTIterator {
    public:
        typedef typename remove_const<T>::type      value_type;
        typedef ptrdiff_t                           difference_type;
        typedef T*                                  pointer;
        typedef T&                                  reference;
        typedef std::bidirectional_iterator_tag     iterator_category;
        typedef Node<T>                             node_type;
        typedef node_type*                          node_pointer;
        
        BTIterator() {}
        BTIterator(const node_pointer& node) :
            node(node)
        {}
        
        BTIterator(const BTIterator& rhs) :
            node(rhs.node)
        {}

        virtual BTIterator& operator=(const BTIterator& rhs)
        {
            if (this != &rhs)
                node = rhs.node;
            return (*this);
        }

        virtual ~BTIterator() {}

        operator BTIterator<const value_type>() const {
            return BTIterator<const value_type>(node);
        }

        reference operator*() const {
            return static_cast<Node<T>*>(node)->data;
            // return &(Node::getData(node));
        }

        pointer operator->() const {
            // return &(Node::getData(node));
            return &static_cast<Node<T>*>(node)->data;
        }

        BTIterator& operator++() {
            node = node->successor();
            return *this;
        }

        BTIterator operator++(int) {
            BTIterator it(*this);
            ++(*this);
            return it;
        }

        BTIterator operator--() {
            node = node->predecessor();
            return *this;
        }

        BTIterator operator--(int) {
            BTIterator it(*this);
            --(*this);
            return it;
        }

        friend bool operator==(const BTIterator& lhs, const BTIterator& rhs) {
            return lhs.node == rhs.node;
        }

        friend bool operator!=(const BTIterator& lhs, const BTIterator& rhs) {
            return lhs.node != rhs.node;
        }

        node_pointer node;
    };
} // namespace ft

#endif
