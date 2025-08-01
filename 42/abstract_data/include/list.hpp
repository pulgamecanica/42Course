#ifndef FT_LIST_HPP
#define FT_LIST_HPP

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

  typedef ft::list_random_access_iterator          iterator;                            // See 23.1
  typedef ft::list_random_access_iterator          const_iterator;                      // See 23.1
  typedef std::reverse_iterator<iterator>           reverse_iterator;
  typedef std::reverse_iterator<const_iterator>     const_reverse_iterator;

  // 23.2.2.1 construct/copy/destroy:
  explicit list(const Alloc& = Alloc());

  explicit list(size_type n, const T& value = T(), const Alloc& = Alloc());

  template <class InputIterator>
  list(InputIterator first, InputIterator last, const Alloc& = Alloc());

  list(const list& x);

  ~list();

  list& operator=(const list& x);

  template <class InputIterator>
  void assign(InputIterator first, InputIterator last);

  void assign(size_type n, const T& t);

  allocator_type get_allocator() const;

  // iterators:

  iterator begin();

  const_iterator begin() const;

  iterator end();

  const_iterator end() const;

  reverse_iterator rbegin();

  const_reverse_iterator rbegin() const;

  reverse_iterator rend();

  const_reverse_iterator rend() const;


  // 23.2.2.2 capacity:

  bool empty() const;

  size_type size() const;

  size_type max_size() const;

  void resize(size_type sz, T c = T());

  // element access:

  reference front();

  const_reference front() const;

  reference back();

  const_reference back() const;

  // 23.2.2.3 modifiers:

  void push_front(const T& x);

  void pop_front();

  void push_back(const T& x);

  void pop_back();

  iterator insert(iterator position, const T& x);

  void insert(iterator position, size_type n, const T& x);

  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last);

  iterator erase(iterator position);

  iterator erase(iterator position, iterator last);

  void swap(list&);

  void clear();


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
