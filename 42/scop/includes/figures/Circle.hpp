//***************************//
//*Template by pulgamecanica*//
//***************************//
#ifndef __CIRCLE_HPP__
# define __CIRCLE_HPP__

#include <iostream>
#include "Figure.hpp"

namespace scop {
	class Circle: public virtual Figure {
		public:
			Circle(point p, int r);
			~Circle();
			int 					getRad() const;
	    virtual bool	isHovered (point & p) const;
	  private:
			int	r;
	};
	std::ostream&	operator<<(std::ostream&, const Circle&);
}
#endif