#ifndef FT_VECTOR_HPP
#define FT_VECTOR_HPP

#include <limits>
#include "exception.hpp"
#include "utils/algorithm.hpp"
#include "utils/swap.hpp"
#include "utils/enable_if.hpp"
#include "utils/lexicographical_compare.hpp"
#include "iterators/vector_random_access_iterator.hpp"
#include "iterators/reverse_iterator.hpp"

namespace ft {

template <typename T, typename Alloc = std::allocator<T> >
class vector {
public:
  typedef T                                         value_type;
  typedef Alloc                                     allocator_type;
  typedef typename allocator_type::reference        reference;
  typedef typename allocator_type::const_reference  const_reference;
  typedef typename allocator_type::pointer          pointer;
  typedef typename allocator_type::const_pointer    const_pointer;
  typedef std::size_t                               size_type;
  typedef std::ptrdiff_t                            difference_type;

  typedef ft::vector_random_access_iterator<T>        iterator;
  typedef ft::vector_random_access_iterator<const T>  const_iterator;
  typedef ft::reverse_iterator<iterator>              reverse_iterator;
  typedef ft::reverse_iterator<const_iterator>        const_reverse_iterator;

private:
  allocator_type _alloc;
  pointer        _data;
  size_type      _size;
  size_type      _capacity;

  void destroy_elements() {
      for (size_type i = 0; i < _size; ++i)
        _alloc.destroy(_data + i);
  }

  void uninitialized_copy(pointer dest, const_pointer src, size_type count) {
      for (size_type i = 0; i < count; ++i)
        _alloc.construct(dest + i, src[i]);
  }

  void ensure_capacity(size_type min_capacity) {
    if (min_capacity > _capacity)
        reserve(_capacity == 0 ? min_capacity : ft::max(_capacity * 2, min_capacity));
  }

public:
  explicit vector(const allocator_type& alloc = allocator_type())
    : _alloc(alloc), _data(NULL), _size(0), _capacity(0) {}

  vector(size_type n, const value_type& val = value_type(),
          const allocator_type& alloc = allocator_type())
    : _alloc(alloc), _data(NULL), _size(n), _capacity(n) {
    if (n > max_size())
      throw ft::out_of_range("vector: n exceeds max_size");
    _data = _alloc.allocate(n);
    for (size_type i = 0; i < n; ++i)
      _alloc.construct(_data + i, val);
  }

  template <typename InputIterator>
  vector(InputIterator first, InputIterator last,
          const allocator_type& alloc = allocator_type(),
          typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0)
    : _alloc(alloc), _data(NULL), _size(0), _capacity(0) {
    for (; first != last; ++first)
      push_back(*first);
  }

  vector(const vector& x)
    : _alloc(x._alloc), _data(NULL), _size(x._size), _capacity(x._capacity) {
    _data = _alloc.allocate(_capacity);
    uninitialized_copy(_data, x._data, _size);
  }

  ~vector() {
    destroy_elements();
    if (_data)
      _alloc.deallocate(_data, _capacity);
  }

  vector& operator=(const vector& x) {
    if (this != &x) {
      vector tmp(x);
      swap(tmp);
    }
    return *this;
  }

  // Iterators
  iterator begin() { return iterator(_data); }
  const_iterator begin() const { return const_iterator(_data); }
  iterator end() { return iterator(_data + _size); }
  const_iterator end() const { return const_iterator(_data + _size); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // Capacity
  size_type size() const { return _size; }
  size_type max_size() const { return _alloc.max_size(); }
  size_type capacity() const { return _capacity; }
  bool empty() const { return _size == 0; }

  void resize(size_type n, value_type val = value_type()) {
    if (n < _size) {
      for (size_type i = n; i < _size; ++i)
        _alloc.destroy(_data + i);
    } else if (n > _size) {
      reserve(n);
      for (size_type i = _size; i < n; ++i)
        _alloc.construct(_data + i, val);
    }
    _size = n;
  }

  void reserve(size_type n) {
    if (n <= _capacity) return;
    pointer new_data = _alloc.allocate(n);
    for (size_type i = 0; i < _size; ++i)
      _alloc.construct(new_data + i, _data[i]);
    destroy_elements();
    if (_data)
      _alloc.deallocate(_data, _capacity);
    _data = new_data;
    _capacity = n;
  }

  // Element access
  reference operator[](size_type n) { return _data[n]; }
  const_reference operator[](size_type n) const { return _data[n]; }

  reference at(size_type n) {
    if (n >= _size)
      throw ft::out_of_range("vector::at");
    return _data[n];
  }

  const_reference at(size_type n) const {
    if (n >= _size)
      throw ft::out_of_range("vector::at");
    return _data[n];
  }

  reference front() { return _data[0]; }
  const_reference front() const { return _data[0]; }
  reference back() { return _data[_size - 1]; }
  const_reference back() const { return _data[_size - 1]; }

  // Modifiers
  template <class InputIterator>
  void assign(InputIterator first, InputIterator last,
              typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0) {
    clear();
    for (; first != last; ++first)
      push_back(*first);
  }

  void assign(size_type n, const value_type& val) {
    clear();
    reserve(n);
    for (_size = 0; _size < n; ++_size)
      _alloc.construct(_data + _size, val);
  }

  iterator insert(iterator position, const value_type& val) {
    size_type index = position - begin();
    ensure_capacity(_size + 1);
    for (size_type i = _size; i > index; --i) {
      _alloc.construct(_data + i, _data[i - 1]);
      _alloc.destroy(_data + i - 1);
    }
    _alloc.construct(_data + index, val);
    ++_size;
    return iterator(_data + index);
  }

  void insert(iterator position, size_type n, const value_type& val) {
    if (n == 0) return;
    size_type index = position - begin();
    ensure_capacity(_size + n);
    for (size_type i = _size; i > index; --i) {
      _alloc.construct(_data + i + n - 1, _data[i - 1]);
      _alloc.destroy(_data + i - 1);
    }
    for (size_type i = 0; i < n; ++i)
      _alloc.construct(_data + index + i, val);
    _size += n;
  }

  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0) {
      size_type index = position - begin();

      vector temp(first, last);
      size_type n = temp.size();
      if (n == 0) return;

      ensure_capacity(_size + n);

      for (size_type i = _size; i > index; --i) {
          _alloc.construct(_data + i + n - 1, _data[i - 1]);
          _alloc.destroy(_data + i - 1);
      }

      for (size_type i = 0; i < n; ++i)
          _alloc.construct(_data + index + i, temp[i]);

      _size += n;
  }

  iterator erase(iterator position) {
    size_type index = position - begin();
    _alloc.destroy(_data + index);
    for (size_type i = index; i < _size - 1; ++i) {
      _alloc.construct(_data + i, _data[i + 1]);
      _alloc.destroy(_data + i + 1);
    }
    --_size;
    return iterator(_data + index);
  }

  iterator erase(iterator first, iterator last) {
    if (first == last) return first;

    size_type start = first - begin();
    size_type count = last - first;

    // Move elements left
    for (size_type i = start; i + count < _size; ++i) {
        _alloc.destroy(_data + i);
        _alloc.construct(_data + i, _data[i + count]);
    }

    // Destroy remaining trailing objects
    for (size_type i = _size - count; i < _size; ++i)
        _alloc.destroy(_data + i);

    _size -= count;
    return iterator(_data + start);
  }

  void push_back(const value_type& val) {
    ensure_capacity(_size + 1);
    _alloc.construct(_data + _size, val);
    ++_size;
  }

  void pop_back() {
    if (_size > 0) {
      --_size;
      _alloc.destroy(_data + _size);
    }
  }

  void clear() {
    destroy_elements();
    _size = 0;
  }

  void swap(vector& other) {
    ft::swap(_alloc, other._alloc);
    ft::swap(_data, other._data);
    ft::swap(_size, other._size);
    ft::swap(_capacity, other._capacity);
  }

  allocator_type get_allocator() const { return _alloc; }
};

// Non-member swap
template <typename T, typename Alloc>
void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
  x.swap(y);
}

// Relational operators
template <typename T, typename Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  if (lhs.size() != rhs.size()) return false;
  for (std::size_t i = 0; i < lhs.size(); ++i)
    if (!(lhs[i] == rhs[i])) return false;
  return true;
}

template <typename T, typename Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <typename T, typename Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(rhs < lhs);
}

template <typename T, typename Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return rhs < lhs;
}

template <typename T, typename Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(lhs < rhs);
}

} // namespace ft

#endif // FT_VECTOR_HPP
