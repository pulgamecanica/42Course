//***************************//
//*Template by pulgamecanica*//
//***************************//
#ifndef __FIGURE_HPP__
# define __FIGURE_HPP__

#include <iostream>

namespace scop {
	typedef struct  s_point {
	  float x;
	  float y;
	} point;
	class Figure {
		public:
			Figure(point pos); // Make constructor provate in order to make this object abstract
			~Figure();
			// virtual void	draw() const;
			// virtual float area() const;
			// virtual float perimeter() const;
			virtual bool	isHovered(point & pos) const;
			float 				getX() const;
			float 				getY() const;
		protected:
			const point pos;
			//BorderColor border_color;
			//BackgroundColor bg_color;
			//BackgroundImage bg_image;
			//BackgroundText bg_text;

	};
	std::ostream&	operator<<(std::ostream&, const Figure&);
}
#endif

