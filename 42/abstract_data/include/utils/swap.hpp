#ifndef FT_SWAP_HPP
#define FT_SWAP_HPP

namespace ft {

template <typename T>
void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

} // namespace ft

#endif // FT_SWAP_HPP
