//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __WHATEVER_HPP__
# define __WHATEVER_HPP__

template <class type>
void swap(type &a, type &b)
{
    type tmp;

    tmp = a;
    a = b;
    b = tmp;
}

template <class type>
type min(type a, type b)
{
    return (a < b ? a : b);
}

template <class type>
type max(type a, type b)
{
    return (a > b ? a : b);
}
#endif

