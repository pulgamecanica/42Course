#ifndef TARGET_HPP
# define TARGET_HPP

#include <iostream>
#include <string>
#include "ASpell.hpp"

class ATarget {
	protected:
		std::string _type;
	public:
		ATarget();
		ATarget(const std::string &);
		ATarget(const ATarget &);
		ATarget& operator = (const ATarget &);
		virtual ~ATarget();
		const std::string & getType() const;
		virtual ATarget * clone(const ATarget &) = 0;
		void	getHitBySpell(const ASpell &);
};

#endif
