//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "interfaces.hpp"

int	main(void)
{
	{
		AMateria *m1, *m2;

		Character c("Wolf");
		Character c2("Chicken");
		// m = new AMateria("Fire"); -> Triggers Error [Abstract class cannot be allocated]
		m1 = new Ice();
		m2 = new Cure();
		std::cout << *m1 << std::endl;
		std::cout << *m2 << std::endl;
		m1->use(c);
		m2->use(c);
		std::cout << c << std::endl;
		std::cout << c2 << std::endl;

		delete (m1);
		delete (m2);
	}
	{
		Character c("Dog");
		Character c2("Little Kid");
		std::cout << c << std::endl;
		c.equip(new Cure());
		c.equip(new Ice());
		c.equip(new Ice());
		c.equip(new Cure());
		c.unequip(3);
		c.unequip(2);
		c.use(1, c2);
		c.use(0, c2);
		AMateria::clean();
	}
	{
		IMateriaSource* src = new MateriaSource();
		src->learnMateria(new Ice());
		src->learnMateria(new Cure());
		ICharacter* me = new Character("me");
		AMateria* tmp;
		tmp = src->createMateria("ice");
		me->equip(tmp);
		tmp = src->createMateria("cure");
		me->equip(tmp);
		ICharacter* bob = new Character("bob");
		me->use(0, *bob);
		me->use(1, *bob);
		delete bob;
		delete me;
		delete src;
	}
	return (0);
}
