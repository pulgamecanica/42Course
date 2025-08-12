#ifndef FT_DEQUE_HPP
#define FT_DEQUE_HPP

#include <limits>
#include "exception.hpp"
#include "utils/swap.hpp"
#include "utils/enable_if.hpp"
#include "utils/lexicographical_compare.hpp"
#include "iterators/deque_random_access_iterator.hpp"
#include "iterators/reverse_iterator.hpp"

namespace ft {

template <class T, class Alloc = std::allocator<T> >
class deque {  
public:
  typedef T                                         value_type;
  typedef Alloc                                     allocator_type;
  typedef typename allocator_type::reference        reference;
  typedef typename allocator_type::const_reference  const_reference;
  typedef typename allocator_type::pointer          pointer;
  typedef typename allocator_type::const_pointer    const_pointer;
  typedef std::size_t                               size_type;        // See 23.1
  typedef std::ptrdiff_t                            difference_type;  // See 23.1

  typedef ft::deque_random_access_iterator<T>       iterator;         // See 23.1
  typedef ft::deque_random_access_iterator<const T> const_iterator;   // See 23.1
  typedef ft::reverse_iterator<iterator>            reverse_iterator;
  typedef ft::reverse_iterator<const_iterator>      const_reverse_iterator;

private:
  static const size_type BLOCK_BYTES = 2048;
  static const size_type BLOCK_SIZE = BLOCK_BYTES / sizeof(T) > 0 ? BLOCK_BYTES / sizeof(T) : 1;
  static const size_type INITIAL_MAP_SIZE = 8;

  pointer* _map;
  size_type _map_size;
  size_type _start_block, _start_offset;
  size_type _end_block, _end_offset;
  allocator_type _alloc;
  std::allocator<pointer> _map_alloc;

  void reallocate_and_recenter() {
    size_type old_size = _map_size;
    size_type new_map_size = old_size * 2;
    pointer* new_map = _map_alloc.allocate(new_map_size);

    size_type used_blocks = _end_block - _start_block + 1;
    size_type new_start_block = (new_map_size - used_blocks) / 2;

    for (size_type i = 0; i < new_map_size; ++i)
      new_map[i] = 0;

    for (size_type i = 0; i < used_blocks; ++i) {
      size_type from_idx = _start_block + i;
      size_type to_idx = new_start_block + i;

      new_map[to_idx] = _map[from_idx];
    }

    // Must deallocate the old map un-used blocks as well [important!]
    for (size_type i = 0; i < old_size; ++i)
      if (i < _start_block || i > _end_block)
        _alloc.deallocate(_map[i], BLOCK_SIZE);

    _map_alloc.deallocate(_map, old_size);
    _map = new_map;
    _map_size = new_map_size;
    _end_block = new_start_block + used_blocks - 1;
    _start_block = new_start_block;
  }

  void ensure_back_capacity() {
    if (_end_block + 1 >= _map_size) {
      reallocate_and_recenter();
    }
    if (_end_offset == BLOCK_SIZE) {
      ++_end_block;
      if (_map[_end_block] == 0)
        _map[_end_block] = _alloc.allocate(BLOCK_SIZE);
      _end_offset = 0;
    }
  }

  void ensure_front_capacity() {
    if (_start_block == 0 && _start_offset == 0) {
      reallocate_and_recenter();
    }
    if (_start_offset == 0) {
      --_start_block;
      if (_map[_start_block] == 0)
        _map[_start_block] = _alloc.allocate(BLOCK_SIZE);
      _start_offset = BLOCK_SIZE;
    }
  }

  public:

  // 23.2.1.1 construct/copy/destroy:
  explicit deque(const Alloc& alloc = Alloc())
    : _map(0), _map_size(INITIAL_MAP_SIZE), _start_block(0), _start_offset(0),
        _end_block(0), _end_offset(0), _alloc(alloc), _map_alloc() {
    _map = _map_alloc.allocate(_map_size);
    for (size_type i = 0; i < _map_size; ++i)
      _map[i] = _alloc.allocate(BLOCK_SIZE);
  }
  
  explicit deque(size_type n, const T& value = T(), const Alloc& alloc = Alloc())
    : _map(0), _map_size(INITIAL_MAP_SIZE), _start_block(0), _start_offset(0),
      _end_block(0), _end_offset(0), _alloc(alloc), _map_alloc() {
    if (n > max_size())
      throw ft::out_of_range("deque: n exceeds max_size");
    _map = _map_alloc.allocate(_map_size);
    for (size_type i = 0; i < _map_size; ++i)
      _map[i] = _alloc.allocate(BLOCK_SIZE);
    for (size_type i = 0; i < n; ++i)
      push_back(value);
  }

  template <class InputIterator>
  deque(InputIterator first, InputIterator last, const Alloc& alloc = Alloc(),
      typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0)
    : _map(0), _map_size(INITIAL_MAP_SIZE), _start_block(0), _start_offset(0),
      _end_block(0), _end_offset(0), _alloc(alloc), _map_alloc() {
    _map = _map_alloc.allocate(_map_size);
    for (size_type i = 0; i < _map_size; ++i)
        _map[i] = _alloc.allocate(BLOCK_SIZE);
    for (; first != last; ++first)
        push_back(*first);
  }
  
  deque(const deque& x)
    : _map(0), _map_size(INITIAL_MAP_SIZE), _start_block(0), _start_offset(0),
      _end_block(0), _end_offset(0), _alloc(x._alloc), _map_alloc() {
    _map = _map_alloc.allocate(_map_size);
    for (size_type i = 0; i < _map_size; ++i)
      _map[i] = _alloc.allocate(BLOCK_SIZE);
    for (const_iterator it = x.begin(); it != x.end(); ++it)
      push_back(*it);
  }
  
  ~deque() {
    clear();
    for (size_type i = 0; i < _map_size; ++i)
      _alloc.deallocate(_map[i], BLOCK_SIZE);
    _map_alloc.deallocate(_map, _map_size);
  }
  
  deque& operator=(const deque& x) {
    if (this != &x) {
      clear();
      for (const_iterator it = x.begin(); it != x.end(); ++it)
        push_back(*it);
    }
    return *this;
  }
  
  template <class InputIterator>
  void assign(InputIterator first, InputIterator last,
              typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0) {
    clear();
    for (; first != last; ++first)
      push_back(*first);
  }

  void assign(size_type n, const T& t) {
    clear();
    for (size_type i = 0; i < n; i++)
      push_back(t);
  }
  
  allocator_type get_allocator() const { return _alloc; }
  
  // iterators:
  iterator begin() { return iterator(_map, _start_block, _start_offset); }
  const_iterator begin() const { return const_iterator(_map, _start_block, _start_offset); }
  iterator end() { return iterator(_map, _end_block, _end_offset); }
  const_iterator end() const { return const_iterator(_map, _end_block, _end_offset); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
  
  // 23.2.1.2 capacity:
  /**
   * Computes the number of elements in the deque.
   * 
   * The formula works by computing the total number of elements
   * from logical [begin(), end()) range.
   * It handles both cases when _end_block >= _start_block
   * and the rare case where _end_block < _start_block (e.g., misaligned front/back growth).
   *
   * FULL_BLOCKS = end_block - start_block
   * OFFSET_ELEMENTS = _end_offset - _start_offset
   * SIZE = (FULL_BLOCKS) * BLOCK_SIZE + OFFSET_ELEMENTS
   */
  size_type size() const {
    difference_type block_diff = static_cast<difference_type>(_end_block) - static_cast<difference_type>(_start_block);
    difference_type offset_diff = static_cast<difference_type>(_end_offset) - static_cast<difference_type>(_start_offset);
    difference_type result = block_diff * BLOCK_SIZE + offset_diff;
    return result >= 0 ? static_cast<size_type>(result) : 0;
  }
  
  size_type max_size() const {
    return _alloc.max_size();
  }
  
  void resize(size_type sz, T c = T()) {
    size_type current = size();
    if (sz < current) {
      for (size_type i = 0; i < current - sz; ++i)
        pop_back();
    } else if (sz > current) {
      for (size_type i = 0; i < sz - current; ++i)
        push_back(c);
    }
  }
  
  bool empty() const {
    return begin() == end();
  }
  
  // element access:
  reference operator[](size_type n) {
    size_type offset = _start_offset + n;
    size_type block = _start_block + offset / BLOCK_SIZE;
    size_type index = offset % BLOCK_SIZE;
    return _map[block][index];
  }
  
  const_reference operator[](size_type n) const {
    size_type offset = _start_offset + n;
    size_type block = _start_block + offset / BLOCK_SIZE;
    size_type index = offset % BLOCK_SIZE;
    return _map[block][index];
  }

  reference at(size_type n) {
      if (n >= size())
          throw ft::out_of_range("deque::at");
      return (*this)[n];
  }

  const_reference at(size_type n) const {
      if (n >= size())
          throw ft::out_of_range("deque::at");
      return (*this)[n];
  }

  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }

  reference back() {
    iterator tmp = end();
    --tmp;
    return *tmp;
  }

  const_reference back() const {
    const_iterator tmp = end();
    --tmp;
    return *tmp;
  }
  
  // 23.2.1.3 modifiers:
  void push_front(const T& x) {
    if (_start_offset == 0)
      ensure_front_capacity();
    --_start_offset;
    _alloc.construct(&_map[_start_block][_start_offset], x);
  }

  void push_back(const T& value) {
    ensure_back_capacity();
    _alloc.construct(&_map[_end_block][_end_offset], value);
    ++_end_offset;
    ensure_back_capacity();
  }
  
  iterator insert(iterator position, const T& x) {
    size_type index = position - begin();
    push_back(x);  // Insert at end to expand size
    for (size_type i = size() - 1; i > index; --i)
      (*this)[i] = (*this)[i - 1];
    (*this)[index] = x;
    return begin() + index;
  }

  void insert(iterator position, size_type n, const T& x) {
    if (n == 0) return;
    size_type index = position - begin();
    for (size_type i = 0; i < n; ++i)
      push_back(x);
    for (size_type i = size() - 1; i >= index + n; --i)
      (*this)[i] = (*this)[i - n];
    for (size_type i = 0; i < n; ++i)
      (*this)[index + i] = x;

  }

  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0) {

    if (first == last)
        return;

    for (; first != last; ++first)
        insert(position++, *first);

  }
  
  void pop_front() {
    if (empty()) return;

    _alloc.destroy(&_map[_start_block][_start_offset]);
    ++_start_offset;
    if (_start_offset == BLOCK_SIZE) {
        ++_start_block;
        _start_offset = 0;
    }
  }
  
  void pop_back() {
    if (empty()) return;

    if (_end_offset == 0) {
      --_end_block;
      _end_offset = BLOCK_SIZE;
    }
    --_end_offset;
    _alloc.destroy(&_map[_end_block][_end_offset]);
  }
  
  iterator erase(iterator position) {
    iterator next = position;
    ++next;
    for (iterator it = position; next != end(); ++it, ++next)
      *it = *next;
    pop_back();
    return position;
  }
  
  iterator erase(iterator first, iterator last) {
    iterator dst = first;
    iterator src = last;
    while (src != end()) {
      *dst = *src;
      ++dst;
      ++src;
    }
    while (dst != end())
      pop_back();
    return first;
  }
  
  void swap(deque& other) {
      ft::swap(_map, other._map);
      ft::swap(_map_size, other._map_size);
      ft::swap(_start_block, other._start_block);
      ft::swap(_start_offset, other._start_offset);
      ft::swap(_end_block, other._end_block);
      ft::swap(_end_offset, other._end_offset);
      ft::swap(_alloc, other._alloc);
      ft::swap(_map_alloc, other._map_alloc);
  }
  
  void clear() {
    while (!empty()) pop_back();
  }
};

// Relational operators
template <typename T, typename Alloc>
bool operator==(const deque<T, Alloc>& x, const deque<T, Alloc>& y) {
  if (x.size() != y.size()) return false;

  for (std::size_t i = 0; i < x.size(); ++i)
    if (!(x[i] == y[i])) return false;
  return true;
}

template <typename T, typename Alloc>
bool operator!=(const deque<T, Alloc>& x, const deque<T, Alloc>& y) {
  return !(x == y);
}

template <typename T, typename Alloc>
bool operator<(const deque<T, Alloc>& x, const deque<T, Alloc>& y) {
  return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <typename T, typename Alloc>
bool operator>(const deque<T, Alloc>& x, const deque<T, Alloc>& y) {
  return y < x;
}

template <typename T, typename Alloc>
bool operator<=(const deque<T, Alloc>& x, const deque<T, Alloc>& y) {
  return !(y < x);
}

template <typename T, typename Alloc>
bool operator>=(const deque<T, Alloc>& x, const deque<T, Alloc>& y) {
  return !(x < y);
}

template <typename T, typename Alloc>
void swap(deque<T, Alloc>& x, deque<T, Alloc>& y) {
  x.swap(y);
}

} // namespace ft

#endif // FT_DEQUE_HPP
