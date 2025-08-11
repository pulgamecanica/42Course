#ifndef STACK_HPP
# define STACK_HPP

#include "vector.hpp"

namespace ft {

template <class T, class Cont = ft::vector<T> >
class stack {
public:
    typedef Cont container_type;
    typedef typename container_type::value_type value_type;
    typedef typename container_type::size_type size_type;

    explicit stack(const container_type &c_ = container_type()) : c(c_) {}
    virtual ~stack() {}

    bool empty() const { return c.empty(); }
    const value_type &top() const { return c.back(); }
    value_type &top() { return c.back(); }
    void pop() { c.pop_back(); }
    void push(const value_type &val) { c.push_back(val); }
    size_type size() const { return c.size(); }

    template <class U, class V>
    friend bool operator==(const stack<U, V> &x, const stack<U, V> &y);
    template <class U, class V>
    friend bool operator!=(stack<U, V> const &x, stack<U, V> const &y);
    template <class U, class V>
    friend bool operator<(stack<U, V> const &x, stack<U, V> const &y);
    template <class U, class V>
    friend bool operator<=(stack<U, V> const &x, stack<U, V> const &y);
    template <class U, class V>
    friend bool operator>(stack<U, V> const &x, stack<U, V> const &y);
    template <class U, class V>
    friend bool operator>=(stack<U, V> const &x, stack<U, V> const &y);

protected:
    container_type c;
};

template <class T, class Cont>
bool operator==(const stack<T, Cont> &x, const stack<T, Cont> &y)
{ return (x.c == y.c);}

template <class T, class Cont>
bool operator!=(const stack<T, Cont> &x, const stack<T, Cont> &y)
{ return (x.c != y.c); }

template <class T, class Cont>
bool operator<(const stack<T, Cont> &x, const stack<T, Cont> &y)
{ return (x.c < y.c); }

template <class T, class Cont>
bool operator<=(const stack<T, Cont> &x, const stack<T, Cont> &y)
{ return (x.c <= y.c); }

template <class T, class Cont>
bool operator>(const stack<T, Cont> &x, const stack<T, Cont> &y)
{ return (x.c > y.c); }

template <class T, class Cont>
bool operator>=(const stack<T, Cont> &x, const stack<T, Cont> &y)
{ return (x.c >= y.c); }

}

#endif