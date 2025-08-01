#ifndef FT_LIST_HPP
#define FT_LIST_HPP

#include <limits>
#include "utils/node.hpp"
#include "utils/enable_if.hpp"
#include "iterators/list_bidirectional_iterator.hpp"
#include "iterators/reverse_iterator.hpp"

namespace ft {

template <class T, class Alloc = std::allocator<T> >
class list {
public:
  typedef T                                         value_type;
  typedef Alloc                                     allocator_type;
  typedef typename allocator_type::reference        reference;
  typedef typename allocator_type::const_reference  const_reference;
  typedef typename allocator_type::pointer          pointer;
  typedef typename allocator_type::const_pointer    const_pointer;
  typedef std::size_t                               size_type;        // See 23.1
  typedef std::ptrdiff_t                            difference_type;  // See 23.1

  typedef ft::list_bidirectional_iterator<T>        iterator;         // See 23.1
  typedef ft::list_bidirectional_iterator<const T>  const_iterator;   // See 23.1
  typedef ft::reverse_iterator<iterator>            reverse_iterator;
  typedef ft::reverse_iterator<const_iterator>      const_reverse_iterator;

private:
  typedef ft::Node<T>                   Node;
  typedef typename std::allocator<Node> node_allocator_type;

  Node*               _head;     // sentinel node (circular linked list)
  size_type           _size;
  allocator_type      _alloc;
  node_allocator_type _node_alloc;

  Node* _create_node(const T& val = T()) {
    Node* n = _node_alloc.allocate(1);
    _node_alloc.construct(n, Node(val));
    return n;
  }

  void _destroy_node(Node* node) {
    _node_alloc.destroy(node);
    _node_alloc.deallocate(node, 1);
  }

  void _link_between(Node* prev, Node* node, Node* next) {
    if (!node) return;
    node->prev = prev;
    node->next = next;
    if (prev) prev->next = node;
    if (next) next->prev = node;
  }

  Node* _insert_before(Node* pos, const T& value) {
    Node* node = _create_node(value);
    _link_between(pos->prev, node, pos);
    ++_size;
    return node;
  }

  Node* _insert_after(Node* pos, const T& value) {
    Node* node = _create_node(value);
    _link_between(pos, node, pos->next);
    ++_size;
    return node;
  }

  void _insert_node_before(Node* pos, Node* node) {
    _link_between(pos->prev, node, pos);
    ++_size;
  }

  void _insert_node_after(Node* pos, Node* node) {
    _link_between(pos, node, pos->next);
    ++_size;
  }

  void _init_empty_list() {
    _head = _create_node(); // sentinel node
    _head->next = _head;
    _head->prev = _head;
  }

public:
  // 23.2.2.1 construct/copy/destroy:
  explicit list(const Alloc& alloc = Alloc())
    : _head(NULL), _size(0), _alloc(alloc), _node_alloc(node_allocator_type()) {
    _init_empty_list();
  }

  explicit list(size_type n, const T& value = T(), const Alloc& alloc = Alloc())
    : _head(NULL), _size(0), _alloc(alloc), _node_alloc(node_allocator_type()) {
    _init_empty_list();
    if (n > max_size())
      throw std::length_error("list: n exceeds max_size");
    for (size_type i = 0; i < n; ++i)
      push_back(value);
  }

  template <class InputIterator>
  list(InputIterator first, InputIterator last,
        const Alloc& alloc = Alloc(),
        typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0)
    : _head(NULL), _size(0), _alloc(alloc), _node_alloc(node_allocator_type()) {
    _init_empty_list();
    for (; first != last; ++first)
      push_back(*first); 
  }

  list(const list& x)
    : _head(NULL), _size(0), _alloc(x.alloc), _node_alloc(node_allocator_type()) {
    _init_empty_list();
    for (const_iterator it = x.begin(); it != x.end(); ++it)
      push_back(*it);
  }

  ~list() {
    clear();
    _destroy_node(_head);
  }

  list& operator=(const list& x);

  template <class InputIterator>
  void assign(InputIterator first, InputIterator last);

  void assign(size_type n, const T& t);

  allocator_type get_allocator() const;

  // iterators:

  iterator begin() { return iterator(_head->next); }
  const_iterator begin() const { return const_iterator(_head->next); }
  iterator end()   { return iterator(_head); }
  const_iterator end()   const { return const_iterator(_head); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }


  // 23.2.2.2 capacity:

  bool empty() const { return _size == 0; };

  size_type size() const { return _size; }

  size_type max_size() const { return _alloc.max_size(); }

  void resize(size_type sz, T c = T());

  // element access:

  reference front() { return *begin(); }

  const_reference front() const { return *begin(); }

  reference back() { return *(--end()); }

  const_reference back() const { return *(--end()); }

  // 23.2.2.3 modifiers:

  void push_front(const T& x) {
    _insert_after(_head, x);
  }

  void pop_front();

  void push_back(const T& x) {
    _insert_before(_head, x);
  }

  void pop_back();

  iterator insert(iterator position, const T& x);

  void insert(iterator position, size_type n, const T& x);

  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last);

  iterator erase(iterator position);

  iterator erase(iterator position, iterator last);

  void swap(list&);

  void clear() {
    Node* cur = _head->next;
    while (cur != _head) {
      Node* next = cur->next;
      _destroy_node(cur);
      cur = next;
    }
    _head->next = _head;
    _head->prev = _head;
    _size = 0;
  }

  // 23.2.2.4 list operations:

  void splice(iterator position, list& x);

  void splice(iterator position, list& x, iterator i);

  void splice(iterator position, list& x, iterator first, iterator last);

  void remove(const T& value);

  template <class Predicate>
  void remove_if(Predicate pred);

  void unique();

  template <class BinaryPredicate>
  void unique(BinaryPredicate binary_pred);

  void merge(list& x);

  template <class Compare> void merge(list& x, Compare comp);
  void sort();

  template <class Compare> void sort(Compare comp);
  void reverse();
};

template <class T, class Alloc>
bool operator==(const list<T,Alloc>& x, const list<T,Alloc>& y);

template <class T, class Alloc>
bool operator< (const list<T,Alloc>& x, const list<T,Alloc>& y);

template <class T, class Alloc>
bool operator!=(const list<T,Alloc>& x, const list<T,Alloc>& y);

template <class T, class Alloc>
bool operator> (const list<T,Alloc>& x, const list<T,Alloc>& y);

template <class T, class Alloc>
bool operator>=(const list<T,Alloc>& x, const list<T,Alloc>& y);

template <class T, class Alloc>
bool operator<=(const list<T,Alloc>& x, const list<T,Alloc>& y);

// specialized algorithms:
template <class T, class Alloc>
void swap(list<T,Alloc>& x, list<T,Alloc>& y);

} // namespace ft

#endif // FT_LIST_HPP