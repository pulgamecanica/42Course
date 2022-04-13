//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __AMATERIA_HPP__
# define __AMATERIA_HPP__

#include <string>
#include <iostream>

class ICharacter;

class AMateria {
	static AMateria** dropped;
	protected:
		std::string const type;
	public:
		AMateria(std::string const & type);
		virtual ~AMateria();
		std::string const & getType() const;
		void drop();
		virtual AMateria* clone() const = 0;
		virtual void use(ICharacter& target);
		static void clean();
};

std::ostream&	operator<<(std::ostream&, const AMateria&);

#endif
