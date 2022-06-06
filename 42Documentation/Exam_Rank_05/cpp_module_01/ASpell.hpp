#ifndef ASPELL_HPP
# define ASPELL_HPP

#include <iostream>
#include <string>

class ASpell {
	protected:
		std::string _name;
		std::string _effects;

	public:
		ASpell();
		ASpell(const std::string &, const std::string &);
		ASpell(const ASpell &);
		ASpell& operator = (const ASpell &);
		virtual ~ASpell();
		const std::string & getName() const;
		const std::string & getEffects() const;
		virtual ASpell * clone(const ASpell &) = 0;
};

#endif
