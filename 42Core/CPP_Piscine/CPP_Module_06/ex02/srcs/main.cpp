//***************************//
//*Template by pulgamecanica*//
//***************************//
#include <stdlib.h>
#include <time.h>
#include "ex02.hpp"

class Base {
	public:
		virtual ~Base() {};
};

class A: public Base {};

class B: public Base {};

class C: public Base {};

Base* generate(void) {
	int num = rand() % 3;
	switch(num) {
		case 0:
			std::cout << "Generated A\n";
			return (new A);
		case 1:
			std::cout << "Generated B\n";
			return (new B);
		case 2:
			std::cout << "Generated C\n";
			return (new C);
	}
	return (NULL);
}

void identify(Base* p)
{
	A *a;
	B *b;
	C *c;

	a = dynamic_cast<A *>(p);
	b = dynamic_cast<B *>(p);
	c = dynamic_cast<C *>(p);
	if (a != NULL)
		std::cout << "'A' class detected" << std::endl;
	else if (b != NULL)
		std::cout << "'B' class detected" << std::endl;
	else if (c != NULL)
		std::cout << "'C' class detected" << std::endl;
}

void identify(Base& p) {
	identify(&p);
}



int	main(void)
{
	srand(time(NULL));
	{
		for (int i = 0; i < 5; i++)
		{
			std::cout << "Generation #" << i + 1 << std::endl;
			Base *b = generate();
			std::cout << "Detecting from Pointer\n";
			identify(b);
			std::cout << "Detecting from Object\n";
			identify(*b);
			std::cout << "**************************" << std::endl;
		}
	}
	return (0);
}
