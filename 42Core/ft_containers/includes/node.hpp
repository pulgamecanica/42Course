/*
* https://d3c33hcgiwev3.cloudfront.net/_806d02702c594a8d442f4e96711a454c_33BalancedSearchTrees.pdf?Expires=1666137600&Signature=Qu4RDWxVAxUIfA77dCMNQart~jB6E7N1JCvbgPkgyGgirdJtHgey2WuSyCHTBeoxexZRWKgN43USd6Zj9W6eoVtGHKgAit8Tc0HxLY8DaVrem-KABqPYE-NuahnNuBzOV64VoeZF0~TJav8NbuqocgPz7JsMBex5EL8DXu2-30c_&Key-Pair-Id=APKAJLTNE6QMUY6HBC5A
* https://www.coders-hub.com/2015/07/red-black-tree-rb-tree-using-c.html
*/

#ifndef NODE_HPP
# define NODE_HPP

# define B_RED false
# define B_BLACK true

namespace ft {
    template <typename T, class C>
    class Node {
    public:
        typedef T           value_type;
        typedef T&          value_reference;
        typedef const T&    const_reference;
        typedef T*          node_pointer;
        typedef const T*    const_node_pointer;
        typedef C           key_compare;
        
        explicit Node(value_type const& val = value_type(), bool color = B_RED) :
            data(val), parent(NULL), left(NULL), right(NULL), color(color) {}
        
        Node(const Node& node) :
            data(node.data), parent(node.parent), left(node.left), right(node.right), color(B_RED) {}


        Node&   operator=(const Node& x) {
            if (this != &x) {
                this->data = x.data;
                this->parent = x.parent;
                this->left = x.left;
                this->right = x.right;
                this->color = x.color;
            }
            return *this;
        }

        virtual ~Node() {}

        value_reference getData() { return (data); }

        bool operator==(const Node& other) { return (this->data == other.data); }
        bool operator!=(const Node& other) { return (this->data != other.data); }
        bool operator>(const Node& other)  { return (this->data > other.data); }
        bool operator<(const Node& other)  { return (this->data < other.data); }
        bool operator<=(const Node& other) { return (this->data <= other.data); }
        bool operator>=(const Node& other) { return (this->data >= other.data); }

        // static T getData(Node *node) {
        //     return (static_cast<Node<T>*>(node)->data);
        // }

        /* NULL links are black */
        bool isRed(Node x) {
            if (x == NULL)
                return false;
            return x.color == B_RED;
        }
       
        size_t size() const {
            size_t size(1);
            if (this->left && this->left->parent)
                size += this->left->size();
            if (this->right && this->right->parent)
                size += this->right->size();
            return (size);
        }

        size_t height() const {
            if (!this->parent)
                return (1);
            return (std::max(this->left->height(), this->right->height()) + 1);
        }

        size_t bheight() const {
            if (!this->parent)
                return (1);
            size_t height = (this->color == B_BLACK) ? 1 : 0;
            height = std::max(height, this->left->bheight());
            height = std::max(height, this->right->bheight());
            return (height);
        }


        /*  * * * * * * * * * * * * * * * * * * * *
        *              /
        *            (p)
        *           /   \     (x) <- New
        * Left -> (l)   (r)
        *        /   \
        * * * * * * * * * * * * * * * * * * * * * *
        *               (p)
        *              /   \
        *            (l)   (r)  
        *           /   \
        *  New -> (x)
        *        /   \
        * * * * * * * * * * * * * * * * * * * * * */
        void put_left(Node *node) {
            this->left = node;
            node->parent = this;
        }
        /*  * * * * * * * * * * * * * * * * * * * *
        *            \
        *            (p)           (x) <- New
        *           /   \     
        *         (l)   (r) <- Rigth
        *              /   \
        * * * * * * * * * * * * * * * * * * * * * *
        *               (p)
        *              /   \
        *            (l)   (r)
        *                 /   \
        *                     (x) <- New
        *                    /   \
        * * * * * * * * * * * * * * * * * * * * * */
        void put_right(Node *node) {
            this->right = node;
            node->parent = this;
        }
        /*
         * Get the next smaller number
         * - If the node is a red black tree child, then:
         *   - For the right node, the successor is the first parent.
         *   - For the middle node, the the successor is the second parent.
         *   - For the left node, the successor is the next min node.
         * - If the node is not a red black tree the next successor is just the next min.
         */
        Node *successor() {
            Node* node = this;

            if (node && node->right && node->right->parent)
                return (node->right->min());
            while (node && node->parent && node->parent->parent && node == node->parent->right)
                node = node->parent;
            return (node->parent);
        }
        /*
         * Get the next bigger number
         * - If the node is a red black tree child, then:
         *   - For the left node, the successor is the first parent.
         *   - For the middle node, the the successor is the second parent.
         *   - For the right node, the successor is the next max node.
         * - If the node is not a red black tree the next successor is just the next max.
         */
        Node *predecessor() {
            Node* node = this;

            if (node && node->left && node->left->parent)
                return (node->left->max());
            while (node && node->parent && node->parent->parent && node == node->parent->left)
                node = node->parent;
            return (node->parent);
        }
        
        void display() {
            display(this);
        }

        void display(Node *p){
            if(p!=NULL) {
                p->print_node_info();
                if(p->left) {
                    std::cout << "\n\nLeft:\n";
                    display(p->left);
                }
                if(p->right) {
                    std::cout << "\n\nRight:\n";
                    display(p->right);
                }
            }
        }

        void print_node_info() {
            std::cout << "\n\t NODE: ";
            std::cout << "\n Key: "<<data;
            std::cout << "\n Color: " << ((color) ? "Black" : "Red");
            if(parent != NULL)
                std::cout << "\n Parent: " << parent->data;
            else
                std::cout << "\n There is no parent of the node.  ";
            if(right != NULL)
                std::cout << "\n Right Child: " << right->data;
            else
                std::cout << "\n There is no right child of the node.  ";
            if(left != NULL)
                std::cout << "\n Left Child: " << left->data;
            else
                std::cout << "\n There is no left child of the node.  ";
            std::cout << std::endl;
        }

        /* Travell left */
        Node *min() {
            Node *node = this;

            while (node->left && node->left->parent)
                node = node->left;
            return (node);
        }

        /* Travell right */
        Node *max() {
            Node *node = this;

            while (node->right && node->right->parent)
                node = node->right;
            return (node);
        }

        T data;
        Node *parent, *left, *right;
        bool color;
    };

    template <typename T, class C>
    std::ostream&   operator<<(std::ostream& s, const Node<T, C>& node) {
        return (s << node.data);
    }
}

#endif
