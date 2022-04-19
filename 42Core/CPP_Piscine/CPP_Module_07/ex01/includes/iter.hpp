//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ITER_HPP__
# define __ITER_HPP__

#include <iostream>

template <class type>
bool isEven(type num)
{
	std::cout << "IS EVEN? " <<  (num % 2 ? "NO" : "YES") << std::endl;
	return (num % 2 == 0);
}

template <class type>
void iter(type* t1, int len, void (*f)(type&))
{
	if (len <= 0)
		return ;
	for (int i = 0; i < len; i++)
		f(t1[i]);
}

template <typename type, typename func>
void iter(type *arr, int len, func f)
{
	if (len <= 0)
		return ;
    for (int i = 0; i < len; i++)
        f(arr[i]);
}

/*
# define BLACK	"\033[0;30m"
# define RED	"\033[0;31m"
# define GREEN	"\033[0;32m"
# define YELLOW	"\033[0;33m"
# define BLUE	"\033[0;34m"
# define WHITE	"\033[0;37m"
# define ENDC	"\033[0m"
*/

#endif
