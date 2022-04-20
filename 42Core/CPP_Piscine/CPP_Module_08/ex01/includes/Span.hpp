//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SPAN_HPP__
# define __SPAN_HPP__

#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>
#include <limits>


class Span {
	unsigned int n;
	std::vector<int> v;
	public:
		Span();
		Span(int);
		Span(const Span&);
		~Span();
		Span&	operator= (const Span&);
		void addNumber(int);
		unsigned int getN(void) const;
		void addNumbers(std::vector<int> array);
		void printVec(void) const;
        int shortestSpan(void) const;
        int longestSpan(void) const;
};

std::ostream&	operator<<(std::ostream&, const Span&);

#endif

/*
# define BLACK	"\033[0;30m"
# define RED	"\033[0;31m"
# define GREEN	"\033[0;32m"
# define YELLOW	"\033[0;33m"
# define BLUE	"\033[0;34m"
# define WHITE	"\033[0;37m"
# define ENDC	"\033[0m"
*/
