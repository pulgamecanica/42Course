//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __EASY_FIND_HPP__
# define __EASY_FIND_HPP__

#include <iostream>
#include <algorithm>
#include <iterator>
#include <exception>

template <typename T>
void easy_find(T t, int num) {
//	std::<typename T<int>>::iterator location;
//	typename T::iterator it;

	typename T::iterator location;
	location = std::find( t.begin(), t.end(), num );
	if ( location != t.end() )
		std::cout << "First occurence at: " << (location - t.begin()) << std::endl;
	else
		throw std::exception();
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
