#ifndef FT_DEQUE_HPP
#define FT_DEQUE_HPP

#include <limits>

#include "exception.hpp"
#include "utils/swap.hpp"
#include "utils/enable_if.hpp"
#include "iterators/deque_random_access_iterator.hpp"
#include "iterators/reverse_iterator.hpp"


#include <iostream>


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
  typedef std::size_t                               size_type;            // See 23.1
  typedef std::ptrdiff_t                            difference_type;      // See 23.1

  typedef ft::deque_random_access_iterator<T>             iterator;       // See 23.1
  typedef  ft::deque_random_access_iterator<const T>      const_iterator; // See 23.1
  typedef ft::reverse_iterator<iterator>            reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

private:
  static const size_type BLOCK_SIZE = 64;
  static const size_type INITIAL_MAP_SIZE = 8;

  pointer* _map;
  size_type _map_size;
  size_type _start_block, _start_offset;
  size_type _end_block, _end_offset;
  allocator_type _alloc;
  std::allocator<pointer> _map_alloc;

  void debug_state(const char* tag = "") const {
    // std::cout << "[DEBUG " << tag << "] "
    //           << "map=" << _map
    //           << ", _map_size=" << _map_size
    //           << ", _start_block=" << _start_block
    //           << ", _start_offset=" << _start_offset
    //           << ", _end_block=" << _end_block
    //           << ", _end_offset=" << _end_offset
    //           << std::endl;

    // for (size_type i = 0; i < _map_size; ++i) {
    //   std::cout << "  block[" << i << "] = " << _map[i] << '\n';
    // }
    // std::cout << "[******************************]" << '\n';
  }

  void reallocate_and_recenter() {
    size_type old_size = _map_size;
    size_type new_map_size = old_size * 2;
    pointer* new_map = _map_alloc.allocate(new_map_size);

    size_type used_blocks = _end_block - _start_block + 1;
    size_type new_start_block = (new_map_size - used_blocks) / 2;

    for (size_type i = 0; i < new_map_size; ++i)
        new_map[i] = _alloc.allocate(BLOCK_SIZE);

    for (size_type i = 0; i < used_blocks; ++i) {
      size_type from_idx = _start_block + i;
      size_type to_idx = new_start_block + i;

      // Move the pointer (we’re not moving contents, just ownership)
      _alloc.deallocate(new_map[to_idx], BLOCK_SIZE); // Free what we just allocated
      new_map[to_idx] = _map[from_idx];
      // new_map[new_start_block + i] = _map[_start_block + i];
    }
    _map_alloc.deallocate(_map, _map_size);
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
    debug_state("[CONST 1] Initial");
  }
  
  explicit deque(size_type n, const T& value = T(), const Alloc& alloc = Alloc())
    : _map(0), _map_size(INITIAL_MAP_SIZE), _start_block(0), _start_offset(0),
      _end_block(0), _end_offset(0), _alloc(alloc), _map_alloc() {
    _map = _map_alloc.allocate(_map_size);
    for (size_type i = 0; i < _map_size; ++i)
      _map[i] = _alloc.allocate(BLOCK_SIZE);
    for (size_type i = 0; i < n; ++i)
      push_back(value);
    debug_state("[CONST 2] Initial");
  }

  template <class InputIterator>
  deque(InputIterator first, InputIterator last, const Alloc& alloc = Alloc())
    : _map(0), _map_size(INITIAL_MAP_SIZE), _start_block(0), _start_offset(0),
      _end_block(0), _end_offset(0), _alloc(alloc), _map_alloc() {
    _map = _map_alloc.allocate(_map_size);
    for (size_type i = 0; i < _map_size; ++i)
      _map[i] = _alloc.allocate(BLOCK_SIZE);
    for (; first != last; ++first)
      push_back(*first);
    debug_state("[CONST 3] Initial");
  }
  
  deque(const deque<T, Alloc>& x)
    : _map(0), _map_size(INITIAL_MAP_SIZE), _start_block(0), _start_offset(0),
      _end_block(0), _end_offset(0), _alloc(x._alloc), _map_alloc() {
    _map = _map_alloc.allocate(_map_size);
    for (size_type i = 0; i < _map_size; ++i)
      _map[i] = _alloc.allocate(BLOCK_SIZE);
    for (const_iterator it = x.begin(); it != x.end(); ++it)
      push_back(*it);
    debug_state("[CONST 4] Initial");
  }
  
  ~deque() {
    clear();
    for (size_type i = 0; i < _map_size; ++i)
      _alloc.deallocate(_map[i], BLOCK_SIZE);
    _map_alloc.deallocate(_map, _map_size);
  }
  
  deque& operator=(const deque& x) {
    debug_state("[operator=] Initial");
    if (this != &x) {
      clear();
      for (const_iterator it = x.begin(); it != x.end(); ++it)
        push_back(*it);
    }
    debug_state("[operator=] After");
    return *this;
  }
  
  template <class InputIterator>
  void assign(InputIterator first, InputIterator last,
              typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0) {
    debug_state("[assign 1] Initial");
    clear();
    for (; first != last; ++first)
      push_back(*first);
    debug_state("[assign 2] After");
  }

  void assign(size_type n, const T& t) {
    debug_state("[assign 2] Initial");
    clear();
    for (size_type i = 0; i < n; i++)
      push_back(t);
    debug_state("[assign 1] After");
  }
  
  allocator_type get_allocator() const {
    return _alloc;
  }
  
  // iterators:
  iterator begin() {
    return iterator(_map, _start_block, _start_offset);
  }
  
  const_iterator begin() const {
    return const_iterator(_map, _start_block, _start_offset);
  }
  
  iterator end() {
    return iterator(_map, _end_block, _end_offset);
  }
  
  const_iterator end() const {
    return const_iterator(_map, _end_block, _end_offset);
  }
  
  reverse_iterator rbegin() {
    return reverse_iterator(end());
  }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() {
    return reverse_iterator(begin());
  }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }
  
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
    // if (sz == 0) {
    //     _start_block = _map_size / 2;
    //     _end_block = _start_block;
    //     _start_offset = _end_offset = 0;
    // }
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

  reference front() {
    return *begin();
  }

  const_reference front() const {
    return *begin();
  }

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
    debug_state("Before push_front");
    if (_start_offset == 0)
      ensure_front_capacity();
    --_start_offset;
    _alloc.construct(&_map[_start_block][_start_offset], x);
    ensure_back_capacity();
    debug_state("After push_front");
  }

  void push_back(const T& value) {
    debug_state("[PUSH_BACK] Initial");
    ensure_back_capacity();
    _alloc.construct(&_map[_end_block][_end_offset], value);
    ++_end_offset;
    ensure_back_capacity();
    debug_state("[PUSH_BACK] After");
  }
  
  iterator insert(iterator position, const T& x) {
    debug_state("[INSERT1 ] Initial");
    size_type index = position - begin();
    push_back(x);  // Insert at end to expand size
    for (size_type i = size() - 1; i > index; --i)
      (*this)[i] = (*this)[i - 1];
    (*this)[index] = x;
    debug_state("[INSERT 1] After");
    return begin() + index;
  }

  void insert(iterator position, size_type n, const T& x) {
    debug_state("[INSERT 2] Initial");

    if (n == 0) return;
    size_type index = position - begin();
    for (size_type i = 0; i < n; ++i)
      push_back(x);
    for (size_type i = size() - 1; i >= index + n; --i)
      (*this)[i] = (*this)[i - n];
    for (size_type i = 0; i < n; ++i)
      (*this)[index + i] = x;
    debug_state("[INSERT 2] After");

  }

  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0) {
    debug_state("[INSERT 3] Initial");

    if (first == last)
        return;

    for (; first != last; ++first)
        insert(position++, *first);
    debug_state("[INSERT 3] After");

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
  return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
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
