//***************************//
//*Template by pulgamecanica*//
//***************************//
#ifndef __RECTANGLE_HPP__
# define __RECTANGLE_HPP__

#include <iostream>
#include "Figure.hpp"

namespace scop {
	class Rectangle: public virtual Figure {
		public:
			Rectangle(point p, int width, int height);
			~Rectangle();
	  	int getWidth() const;
	  	int getHeight() const;
	    virtual bool	isHovered (point & p) const;
	  private:
	  	int width;
	  	int height;
	};
	std::ostream&	operator<<(std::ostream&, const Rectangle&);
}
#endif

