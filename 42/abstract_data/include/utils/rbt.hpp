// https://www.coursera.org/learn/algorithms-part1/lecture/GZe13/red-black-bsts
// https://cplusplus.com/reference/map/map/value_comp/
// https://kubokovac.eu/gnarley-trees/Redblack.html
// https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/
// https://www.cs.swarthmore.edu/~adanner/cs41/f08/LLRB.pdf
// https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
// https://www.coders-hub.com/2015/07/red-black-tree-rb-tree-using-c.html

#ifndef RBT_HPP
# define RBT_HPP

# include <memory>
# include <functional>
# include "../iterators/bt_iterator.hpp"
# include "../iterators/reverse_iterator.hpp"
# include "../utility.hpp"
# include "node.hpp"
# include "swap.hpp"

namespace ft {
template < class Key, class Value, class NodeContents, class Compare = std::less<Key>, class Alloc = std::allocator<ft::pair<const Key,Value> > >
class RedBlackTree {

public:
  typedef Key                                 key_type;
  typedef Value                               mapped_type;
  typedef NodeContents                        value_type;
  typedef Compare                             key_compare;
  typedef Alloc                               allocator_type;
  typedef value_type&                         reference;
  typedef const value_type&                   const_reference;
  typedef value_type*                         pointer;
  typedef const value_type*                   const_pointer;
  typedef ptrdiff_t                           difference_type;
  typedef typename allocator_type::size_type  size_type;

  typedef BTIterator<value_type, pointer, reference, Compare >              iterator;
  typedef BTIterator<value_type, const_pointer, const_reference, Compare >  const_iterator;
  typedef ft::reverse_iterator<iterator>                                    reverse_iterator;
  typedef ft::reverse_iterator<const_iterator>                              const_reverse_iterator;

  typedef BTNode<value_type, key_compare> btnode;
  typedef btnode*                         node_pointer;
public:
  /*
    * value compare Class took from: https://cplusplus.com/reference/map/map/value_comp/
    * "in C++98, it is required to inherit binary_function<value_type,value_type,bool>"
    */
  class value_compare {
    friend class RedBlackTree;
  protected:
    Compare comp;
    explicit value_compare (Compare c) : comp(c) {}  // constructed with map's comparison object
  public:
    bool operator() (const value_type& tmp, const value_type& y) const {
      return comp(tmp.first, y.first);
    }
  };

protected:
  allocator_type  _alloc;
  key_compare     _cmp;
  node_pointer    _root;
  node_pointer    _last;
  node_pointer    _first;
  size_type       _size;

public:
  explicit RedBlackTree(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
    : _alloc(alloc), _cmp(comp), _root(NULL), _last(NULL), _first(NULL), _size(0) {
    _initialize();
  }
  
  RedBlackTree(const RedBlackTree& tmp)
    : _alloc(tmp._alloc), _cmp(tmp._cmp), _root(NULL), _last(NULL), _first(NULL), _size(0) {
    _initialize();
  }
  
  virtual ~RedBlackTree() {
    clear();
    if (_first)
      delete _first;
    if (_last)
      delete _last;
  }

  RedBlackTree& operator=(const RedBlackTree& rbt) {
      if (this != &rbt) {
          clear();
          _alloc = rbt._alloc;
          _cmp = rbt._cmp;
          insert(rbt.begin(), rbt.end());
      }
      return (*this);
  }

  /* Capacity */
  bool                empty()const    { return (_size == 0); }
  size_type           size()const     { return (_size); }
  virtual size_type   max_size()const { return _alloc.max_size() / 2; }

  void clear() {
      if (!empty()) {
          _clear(_root);
          _link_first_last();
      }
  }

  /* Iterators */
  iterator                begin()         { return iterator(_first->parent); }
  const_iterator          begin()const    { return const_iterator(_first->parent); }
  iterator                end()           { return iterator(_last); }
  const_iterator          end()const      { return const_iterator(_last); }
  reverse_iterator        rbegin()        { return reverse_iterator(_last->parent); }
  const_reverse_iterator  rbegin()const   { return const_reverse_iterator(_last->parent); }
  reverse_iterator        rend()          { return reverse_iterator(_first); }
  const_reverse_iterator  rend()const     { return const_reverse_iterator(_first); }

  /* Modifiers */
  virtual void erase(iterator position) {
      node_pointer erase = findbyiterator(position);
      if (erase == _last)
          return ;
      RedBlackDelete(erase);
      delete(erase);
      --_size;
  }

  virtual size_type erase(const key_type& k) {
      iterator it = find(k);
      if (it == end())
          return 0;
      erase(it);
      return 1;
  }

  virtual void erase(iterator first, iterator last) {
      iterator tmp = first;
      while (tmp != last) {
          tmp++;
          erase(first);
          first = tmp;
      }
  }

  void swap(RedBlackTree& x) {
      ft::swap(_size, x._size);
      ft::swap(_first, x._first);
      ft::swap(_root, x._root);
      ft::swap(_last, x._last);
      ft::swap(_alloc, x._alloc);
  }

  /* Observers */
  key_compare     key_comp() const { return _cmp; }
  value_compare   value_comp() const { return value_compare(_cmp); }

  /* Operations */
  virtual iterator find(const key_type& k) {
      btnode *it(_root);
      while (it && it != _first && it != _last) {
          if (itemcompare(k, it->data))
              it = it->left;
          else if (itemcompare(it->data, k))
              it = it->right;
          else return iterator(it);
      }
      return end();
  }

  virtual const_iterator find(const key_type& k) const {
      btnode *it(_root);
      while (it && it != _first && it != _last) {
          if (itemcompare(k, it->data))
              it = it->left;
          else if (itemcompare(it->data, k))
              it = it->right;
          else return const_iterator(it);
      }
      return end();
  }

  virtual size_type count(const key_type& k) const {
      const_iterator  it = begin();
      size_type       count = 0;

      while (it != end()) {
          if (itemcompare(k, *it) == false && itemcompare(*it, k) == false)
              ++count;
          ++it;
      }
      return count;
  }

  virtual iterator lower_bound(const key_type& k) {
      iterator    it = begin(), ite = end();
      while (it != ite) {
          if (itemcompare(*it, k) == false)
              break ;
          ++it;
      }
      return it;
  }

  virtual const_iterator lower_bound(const key_type& k) const {
      const_iterator  it = begin(), ite = end();
      while (it != ite) {
          if (itemcompare(*it, k) == false)
              break ;
          ++it;
      }
      return it;
  }

  virtual iterator upper_bound(const key_type& k) {
      iterator    it = begin(), ite = end();
      while (it != ite) {
          if (itemcompare(k, *it))
              break ;
          ++it;
      }
      return it;
  }

  virtual const_iterator upper_bound(const key_type& k) const {
      const_iterator it = begin(), ite = end();
      while (it != ite) {
          if (itemcompare(k, *it))
              break ;
          ++it;
      }
      return it;
  }
  
  virtual ft::pair<const_iterator,const_iterator> equal_range (const key_type& k) const {
      return ft::make_pair(const_iterator(lower_bound(k)), const_iterator(upper_bound(k)));
  }

  virtual ft::pair<iterator,iterator> equal_range (const key_type& k) {
      return ft::make_pair(iterator(lower_bound(k)), iterator(upper_bound(k)));
  }

  void displayRBT() {
      // if (!empty()) {
      //     std::cout << "Size: " << _root->size() << " | Height: " << _root->height() << " | BHeight: " << _root->bheight() << std::endl;
      //     std::cout << "Min: " <<  _root->min()->getData() << " | Max: " << _root->max()->getData() << std::endl;
      //     _root->display();
      // }
  }
// Miscelaneous
  protected:
      inline bool itemcompare(const key_type& k, const ft::pair<Key, Value>& p) const {
          return key_comp()(k, p.first);
      }
      
      inline bool itemcompare(const ft::pair<Key, Value>& p, const key_type& k) const {
          return key_comp()(p.first, k);
      }
      
      inline bool itemcompare(const key_type& k1, const key_type& k2) const {
          return key_comp()(k1, k2);
      }
      
      inline bool itemcompare(const ft::pair<Key, Value>& p1, const ft::pair<Key, Value>& p2) const {
          return key_comp()(p1.first, p2.first);
      }
      
      virtual btnode *findbyiterator(iterator position) {
          btnode *it(_root);
          while (it && it != _first && it != _last) {
              if (itemcompare(*position, it->data))
                  it = it->left;
              else if (itemcompare(it->data, *position))
                  it = it->right;
              else return (it);
          }
          return _last;
      }
      
      void initmap() {
          _first = new btnode();
          _last = new btnode();
          link_outer();
          _size = 0;
      }

      btnode *insert_root(const value_type& val) {
          _root = new btnode(val, B_BLACK);
          ++_size;
          _first->parent = _last->parent = _root;
          _root->left = _first;
          _root->right = _last;
          return _root;
      }

      void erase_root() {
          if (_size == 1) {
              delete _root;
              link_outer();
              return ;
          }       
      }

      btnode *insert_left(btnode *it, const value_type& val = value_type()) {
          btnode *insert = new btnode(val);
          insert->parent = it;
          insert->left = it->left;
          if (it->left)
              it->left->parent = insert;
          it->left = insert;
          ++_size;
            fixRedBlackViolations(insert);
          return insert;
      }

      btnode *insert_right(btnode *it, const value_type& val = value_type()) {
          btnode *insert = new btnode(val);
          insert->parent = it;
          insert->right = it->right;
          if (it->right)
              it->right->parent = insert;
          it->right = insert;
          ++_size;
            fixRedBlackViolations(insert);
          return insert;
      }
      
      void link_outer() {
          _first->parent = _last;
          _last->parent = _first;
      }

      void left_rotation(btnode *x) {
          btnode *y = x->right;
          x->right = y->left;
          if (x->right)
              x->right->parent = x;
          y->parent = x->parent;
          if (y->parent == 0)
              _root = y;
          else if (x == y->parent->left)
              y->parent->left = y;
          else
              y->parent->right = y;
          y->left = x;
          x->parent = y;
      }

      void right_rotation(btnode *x) {
          btnode *y = x->left;
          x->left = y->right;
          if (x->left)
              x->left->parent = x;
          y->parent = x->parent;
          if (y->parent == 0)
              _root = y;
          else if (x == y->parent->left)
              y->parent->left = y;
          else
              y->parent->right = y;
          y->right = x;
          x->parent = y;
      }
      bool is_validnode(btnode *check) {
          return (check && check != _first && check != _last);
      }
      void fixRedBlackViolations(btnode *z) {
          while (z != _root && z->color == B_RED && z->parent->color == B_RED) {
              btnode *parent = z->parent;
              btnode *grandpa = parent->parent;
              if (parent == grandpa->left) {
                  btnode *uncle = grandpa->right;
                  if (is_validnode(uncle) && uncle->color == B_RED) {
                      grandpa->color = B_RED;
                      parent->color = B_BLACK;
                      uncle->color = B_BLACK;
                      z = grandpa;
                  }
                  else if (z == parent->right) {
                      left_rotation(parent);
                      z = parent;
                      parent = z->parent;
                  }
                  else {
                      right_rotation(grandpa);
                      ft::swap(parent->color, grandpa->color);
                  }
              }
              else if (parent == grandpa->right) {
                  btnode *uncle = grandpa->left;

                  if (is_validnode(uncle) && uncle->color == B_RED) {
                      grandpa->color = B_RED;
                      parent->color = B_BLACK;
                      uncle->color = B_BLACK;
                      z = grandpa;
                  }
                  else if (z == parent->left) {
                      right_rotation(parent);
                      z = parent;
                      parent = z->parent;
                  }
                  else {
                      left_rotation(grandpa);
                      ft::swap(parent->color, grandpa->color);
                      z = parent;
                  }
              }
          }
          _root->color = B_BLACK;
      }

      void RedBlackTrspl(btnode *u, btnode *v) {
          if (u && u->parent == 0)
              _root = v;
          else if (u == u->parent->left)
              u->parent->left = v;
          else
              u->parent->right = v;
          if (v)
              v->parent = u->parent;
      }
      btnode*    farleft(btnode *x) {
          while (x->left)
              x = x->left;
          return x;
      }
      void RedBlackDelete(btnode *z) {
          btnode *y(z);
          btnode *x;
          bool  y_orignal_color = y->color;
          if (z->left == 0) {
              x = z->right;
              RedBlackTrspl(z, z->right);
          }
          else if (z->right == 0) {
              x = z->left;
              RedBlackTrspl(z, z->left);
          }
          else {
              y = farleft(z->right);
              y_orignal_color = y->color;
              x = y->right;
              if (y->parent == z) {
                  if (x) {
                      x->parent = y;
                  }
              }
              else {
                  RedBlackTrspl(y, y->right);
                  y->right = z->right;
                  y->right->parent = y;
              }
              RedBlackTrspl(z, y);
              y->left = z->left;
              y->left->parent = y;
              y->color = z->color;
          }
          if (y_orignal_color == B_BLACK) {
              RedBlackDeleteFixup(x);
          }
      }
      void RedBlackDeleteFixup(btnode *x) {
          if (x == 0)
              return ;
          while (x != _root && x->color == B_BLACK) {
              if (x == x->parent->left) {
                  btnode *w = x->parent->right;
                  if (w->color == B_RED) {
                      w->color = B_BLACK;
                      x->parent->color = B_RED;
                      left_rotation(x->parent);
                      w = x->parent->right;
                  }
                  if (w->left->color == B_BLACK && w->right->color == B_BLACK) {
                      w->color = B_RED;
                      x = x->parent;
                  }
                  else {
                      if (w->right->color == B_BLACK) {
                          w->left->color = B_BLACK;
                          w->color = B_RED;
                          right_rotation(w);
                          w = x->parent->right;
                      }
                      else {
                          w->color = x->parent->color;
                          x->parent->color = B_BLACK;
                          w->right->color = B_BLACK;
                          left_rotation(x->parent);
                          x = _root;
                      }
                  }
              }
              else {
                  btnode *w = x->parent->left;
                  if (w->color == B_RED) {
                      w->color = B_BLACK;
                      x->parent->color = B_RED;
                      right_rotation(x->parent);
                      w = x->parent->left;
                  }
                  if (w->right->color == B_BLACK && w->left->color == B_BLACK) {
                      w->color = B_RED;
                      x = x->parent;
                  }
                  else {
                      if (w->left->color == B_BLACK) {
                          w->right->color = B_BLACK;
                          w->color = B_RED;
                          left_rotation(w);
                          w = x->parent->left;
                      }
                      else {
                          w->color = x->parent->color;
                          x->parent->color = B_BLACK;
                          w->left->color = B_BLACK;
                          right_rotation(x->parent);
                          x = _root;
                      }
                  }
              }
          }
          x->color = B_BLACK;
      }

      void _initialize() {
          _first = new btnode();
          _last = new btnode();
          _link_first_last();
      }

      void _link_first_last() {
          _first->parent = _last;
          _last->parent = _first;
      }

      void _clear(node_pointer pos) {
          if (!pos)
              return ;
          _clear(pos->left);
          _clear(pos->right);
          if (pos != _first && pos != _last) {
              delete pos;
              pos = NULL;
              --_size;
          }
      }

};

template <typename T, typename Compare, typename Alloc>
void swap(RedBlackTree<T, Compare, Alloc>& lhs, RedBlackTree<T, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}
}

#endif