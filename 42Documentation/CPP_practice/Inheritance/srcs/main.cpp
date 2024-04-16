//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <vector>

#include "Inheritance.hpp"

#include "Base.hpp"
// #include "Middle.hpp"
// #include "VMiddle.hpp"
#include "A.hpp"
#include "VA.hpp"

int	main(void)
{
	std::cout << "************************" << std::endl;
	{
		Base * b;

		b = new A;

		b->msg();
		
		delete(b);
	}
	std::cout << "************************" << std::endl;
	{ // V Version
		Base * b;

		b = new VA;

		b->msg();
		
		delete(b);
	}
	std::cout << "************************" << std::endl;
	{
		std::vector<Base *> v;

		v.push_back(new VA);
		v.push_back(new A);

		v.back()->msg();
		delete v.back();
		v.pop_back();
		v.back()->msg();
		delete v.back();

		v.clear();
	}
	std::cout << "************************" << std::endl;
	return (0);
}
