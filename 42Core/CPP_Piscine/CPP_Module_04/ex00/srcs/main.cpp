//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "polymorphism.hpp"

int	main(void)
{
	
	{
		Animal	a1;
		Animal	a2;
		Animal	*cat = new Cat();

		std::cout << a1 << std::endl;
		std::cout << a2 << std::endl;
		std::cout << *cat << std::endl;
		a1.makeSound();
		a2.makeSound();
		cat->makeSound();
		delete(cat);
	}

	{
		const Animal* meta = new Animal();
		const Animal* j = new Dog();
		const Animal* i = new Cat();
		std::cout << j->getType() << " " << std::endl;
		std::cout << i->getType() << " " << std::endl;
		i->makeSound();
		j->makeSound();
		meta->makeSound();
		delete(meta);	
		delete(j);	
		delete(i);	
	}

	{
		const Animal* a = new Animal();
		const WrongAnimal* wa = new WrongAnimal();
		const Animal* c = new Cat();
		const WrongAnimal* wc = new WrongCat();
		std::cout << a->getType() << " " << std::endl;
		std::cout << wa->getType() << " " << std::endl;
		std::cout << c->getType() << " " << std::endl;
		std::cout << wc->getType() << " " << std::endl;
		a->makeSound();
		wa->makeSound();
		c->makeSound();
		wc->makeSound();

		delete(a);
		delete(wa);
		delete(c);
		delete(wc);
	}

	return (0);
}
