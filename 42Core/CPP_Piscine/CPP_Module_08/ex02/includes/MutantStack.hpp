//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __MUTANT_STACK_HPP__
# define __MUTANT_STACK_HPP__

#include <iostream>
#include <stack>

template <typename type>
class MutantStack: public std::stack<type> {
	public:
		MutantStack() {};
		MutantStack(const MutantStack& ms) {*this = ms;};
		~MutantStack() {};
		MutantStack& operator= (const MutantStack& ms) {
			std::stack<type>::operator=(ms);
        	return *this;
		};

		typedef typename std::stack<type>::container_type cont;
		typedef typename cont::iterator iterator;
		
		typename cont::iterator begin(void) {return std::stack<type>::c.begin();};
		typename cont::iterator end(void) {return std::stack<type>::c.end();};
};

#endif

