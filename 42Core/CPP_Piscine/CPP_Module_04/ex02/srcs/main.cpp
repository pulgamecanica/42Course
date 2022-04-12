//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "polymorphism.hpp"

int	main(void)
{
	srand(time(0));
	{
		const Animal* j = new Dog();
		const Animal* i = new Cat();
		delete j;//should not create a leak
		delete i;
	}
	{
		Animal*	array[10];
		for (int i = 0; i < 10; ++i)
		{
			if (i % 2)
				array[i] = new Dog;
			else
				array[i] = new Cat;
		}
		for (int i = 0; i < 10; ++i)
			delete (array[i]);
	}

	{
		const Animal* j = new Dog();
		const Animal* i = new Cat();
		std::cout << j->getType() << " " << std::endl;
		std::cout << i->getType() << " " << std::endl;
		i->makeSound();
		j->makeSound();
		delete(j);
		delete(i);
	}

	{
		const Cat catt;
		Cat c2;

		c2 = catt;
		catt.makeSound();
	}
	return (0);
}
