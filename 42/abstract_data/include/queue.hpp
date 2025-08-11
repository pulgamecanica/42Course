#ifndef QUEUE_HPP
# define QUEUE_HPP

#include "utils/algorithm.hpp"       // make_heap, push_heap, pop_heap
#include "utils/less.hpp"
#include "deque.hpp"
#include "vector.hpp"      // default container

namespace ft {

template <class T, class Cont = ft::deque<T> >
class queue {
public:
  typedef Cont                           container_type;
  typedef typename Cont::value_type      value_type;
  typedef typename Cont::size_type       size_type;

  explicit queue(const container_type& c_ = container_type()) : c(c_) {}
  virtual ~queue() {}

  bool empty() const            { return c.empty(); }
  size_type size() const        { return c.size();  }

  value_type&       front()       { return c.front(); }
  const value_type& front() const { return c.front(); }

  value_type&       back()        { return c.back();  }
  const value_type& back() const  { return c.back();  }

  void push(const value_type& val) { c.push_back(val); }
  void pop()                       { c.pop_front();    }

  template <class U, class V>
  friend bool operator==(const queue<U, V>& x, const queue<U, V>& y);
  template <class U, class V>
  friend bool operator!=(const queue<U, V>& x, const queue<U, V>& y);
  template <class U, class V>
  friend bool operator<(const queue<U, V>& x, const queue<U, V>& y);
  template <class U, class V>
  friend bool operator<=(const queue<U, V>& x, const queue<U, V>& y);
  template <class U, class V>
  friend bool operator>(const queue<U, V>& x, const queue<U, V>& y);
  template <class U, class V>
  friend bool operator>=(const queue<U, V>& x, const queue<U, V>& y);

protected:
  container_type c;
};

template <class T, class Cont>
bool operator==(const queue<T, Cont>& x, const queue<T, Cont>& y) { return x.c == y.c; }

template <class T, class Cont>
bool operator!=(const queue<T, Cont>& x, const queue<T, Cont>& y) { return x.c != y.c; }

template <class T, class Cont>
bool operator<(const queue<T, Cont>& x, const queue<T, Cont>& y)  { return x.c <  y.c; }

template <class T, class Cont>
bool operator<=(const queue<T, Cont>& x, const queue<T, Cont>& y) { return x.c <= y.c; }

template <class T, class Cont>
bool operator>(const queue<T, Cont>& x, const queue<T, Cont>& y)  { return x.c >  y.c; }

template <class T, class Cont>
bool operator>=(const queue<T, Cont>& x, const queue<T, Cont>& y) { return x.c >= y.c; }


/******************************************************************************************/
/*                                    PRIORITY QUEUE                                      */
/******************************************************************************************/

template <class T,
          class Container = ft::vector<T>,
          class Compare   = ft::less<typename Container::value_type> >
class priority_queue {
public:
    typedef Container                       container_type;
    typedef typename Container::value_type  value_type;
    typedef typename Container::size_type   size_type;
    typedef Compare                         value_compare;

    explicit priority_queue(const Compare& comp = Compare(),
                            const Container& cont = Container())
        : c(cont), comp(comp)
    {
        // Build heap from existing container
        ft::make_heap(c.begin(), c.end(), comp);
    }

    template <class InputIt>
    priority_queue(InputIt first, InputIt last,
                   const Compare& comp = Compare(),
                   const Container& cont = Container())
        : c(cont), comp(comp)
    {
        c.insert(c.end(), first, last);
        ft::make_heap(c.begin(), c.end(), comp);
    }

    virtual ~priority_queue() {}

    bool empty() const      { return c.empty(); }
    size_type size() const  { return c.size();  }

    const value_type& top() const { return c.front(); }

    void push(const value_type& value) {
        c.push_back(value);
        ft::push_heap(c.begin(), c.end(), comp);
    }

    void pop() {
        // Move the top to the end, then pop it.
        ft::pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }

protected:
    container_type c;
    value_compare  comp;
};

} // namespace ft

#endif
