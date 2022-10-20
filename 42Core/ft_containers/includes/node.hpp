/*
* https://d3c33hcgiwev3.cloudfront.net/_806d02702c594a8d442f4e96711a454c_33BalancedSearchTrees.pdf?Expires=1666137600&Signature=Qu4RDWxVAxUIfA77dCMNQart~jB6E7N1JCvbgPkgyGgirdJtHgey2WuSyCHTBeoxexZRWKgN43USd6Zj9W6eoVtGHKgAit8Tc0HxLY8DaVrem-KABqPYE-NuahnNuBzOV64VoeZF0~TJav8NbuqocgPz7JsMBex5EL8DXu2-30c_&Key-Pair-Id=APKAJLTNE6QMUY6HBC5A
* https://www.coders-hub.com/2015/07/red-black-tree-rb-tree-using-c.html
*/

#ifndef NODE_HPP
# define NODE_HPP

# define B_RED false
# define B_BLACK true

namespace ft {
    template <typename T>
    struct Node {

        Node() : parent(NULL), left(NULL), right(NULL), color(B_RED) {}

        Node(const T &data, Node *parent = NULL, Node *left = NULL, Node *right = NULL) : data(data) {
            this->parent = parent;
            this->left = left;
            this->right = right;
            this->color = B_RED;
        }

        Node(const Node<T> &rhs) : data(rhs.data) {
            this->parent = rhs.parent;
            this->left = rhs.left;
            this->right = rhs.right;
        }

        virtual ~Node() {

        }

        T getData() {
            return (data);
        }

        static T getData(Node *node) {
            return (static_cast<Node<T>*>(node)->data);
        }

        /* NULL links are black */
        bool isRed(Node x) {
            if (x == NULL)
                return false;
            return x.color == B_RED;
        }

        /* Travell left */
        Node *min() {
            Node *node = this;

            while (node->left && node->left->parent)
                node = node->left;
            return (node);
        }

        /*
        const Node* minimum() const
        {
            return const_cast<Node*>(this->minimum());
        }
        */

        /* Travell right */
        Node *max() {
            Node *node = this;

            while (node->right && node->right->parent)
                node = node->right;
            return (node);
        }

        /*
        const Node* maximum() const
        {
            return const_cast<Node*>(this->maximum());
        }
        */

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
        *
        * * * * * * * * * * * * * * * * * * * * * *
        *
        * * * * * * * * * * * * * * * * * * * * * */

        // void set_parent_child(Node *node) {
        //     if (this->parent->parent)
        //     {
        //         if (this->parent->left == this)
        //             this->parent->left = node;
        //         else
        //             this->parent->right = node;
        //     }
        // }

        /*  * * * * * * * * * * * * * * * * * * * *
        *
        *              /
        *            (p)
        *           /   \     (x) <- New
        * Left -> (l)   (r)
        *        /   \
        *
        * * * * * * * * * * * * * * * * * * * * * *
        *
        *               (p)
        *              /   \
        *            (l)   (r)  
        *           /   \
        *  New -> (x)
        *        /   \
        *
        * * * * * * * * * * * * * * * * * * * * * */
        void put_left(Node *node) {
            this->left = node;
            node->parent = this;
        }

        /*  * * * * * * * * * * * * * * * * * * * *
        *
        *            \
        *            (p)           (x) <- New
        *           /   \     
        *         (l)   (r) <- Rigth
        *              /   \
        *
        * * * * * * * * * * * * * * * * * * * * * *
        *
        *               (p)
        *              /   \
        *            (l)   (r)
        *                 /   \
        *                     (x) <- New
        *                    /   \
        *
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
        // Node *successor() {
        //     Node* node = this;

        //     if (node->right->parent)
        //         return (node->right->min());
        //     while (node->parent->parent && node == node->parent->right)
        //         node = node->parent;
        //     return (node->parent);
        // }

        /*
         * Get the next bigger number
         * - If the node is a red black tree child, then:
         *   - For the left node, the successor is the first parent.
         *   - For the middle node, the the successor is the second parent.
         *   - For the right node, the successor is the next max node.
         * - If the node is not a red black tree the next successor is just the next max.
        */
        // Node *predecessor() {
        //     Node* node = this;

        //     if (node->left->parent)
        //         return (node->left->max());
        //     while (node->parent->parent && node == node->parent->left)
        //         node = node->parent;
        //     return (node->parent);
        // }
        
        static void display(Node *p){
            if(p!=NULL) {
                std::cout<<"\n\t NODE: ";
                std::cout<<"\n Key: "<<p->data;
                std::cout<<"\n Colour: ";
                if(p->color==B_BLACK)
                    std::cout<<"Black";
                else
                    std::cout<<"Red";
                if(p->parent!=NULL)
                    std::cout<<"\n Parent: "<<p->parent->data;
                else
                    std::cout<<"\n There is no parent of the node.  ";
                if(p->right!=NULL)
                    std::cout<<"\n Right Child: "<<p->right->data;
                else
                    std::cout<<"\n There is no right child of the node.  ";
                if(p->left!=NULL)
                    std::cout<<"\n Left Child: "<<p->left->data;
                else
                    std::cout<<"\n There is no left child of the node.  ";
                std::cout<<std::endl;
                if(p->left) {
                    std::cout<<"\n\nLeft:\n";
                    display(p->left);
                }
                /*else
                cout<<"\nNo Left Child.\n";*/
                if(p->right) {
                    std::cout<<"\n\nRight:\n";
                    display(p->right);
                }
            /*else
                cout<<"\nNo Right Child.\n"*/
            }
        }

        void print_node_info() {
            std::cout<<"\n\t FOUND NODE: ";
            std::cout<<"\n Key: "<<data;
            std::cout<<"\n Colour: ";
            if(color=='b')
                std::cout<<"Black";
            else
                std::cout<<"Red";
            if(parent!=NULL)
                std::cout<<"\n Parent: "<<parent->data;
            else
                std::cout<<"\n There is no parent of the node.  ";
            if(right!=NULL)
                std::cout<<"\n Right Child: "<<right->data;
            else
                std::cout<<"\n There is no right child of the node.  ";
            if(left!=NULL)
                std::cout<<"\n Left Child: "<<left->data;
            else
                std::cout<<"\n There is no left child of the node.  ";
            std::cout<<std::endl;
        }
        T data;
        Node *parent, *left, *right;
        bool color;
    };
    template <typename T>
    std::ostream&   operator<<(std::ostream& s, const Node<T>& node) {
        return (s << node.data);
    }
}

#endif
