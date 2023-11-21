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
			~Figure();
			virtual void	draw() const;
			// virtual float area() const;
			// virtual float perimeter() const;
			float 				getX() const;
			float 				getY() const;
		protected:
			const point pos;
			Figure(point pos); // Make constructor protected = abstract object
			//BorderColor border_color;
			//BackgroundColor bg_color;
			//BackgroundImage bg_image;
			//BackgroundText bg_text;
		private:
			// In order to disable this function from being called
			virtual bool	isHovered(point & pos) const;

	};
	std::ostream&	operator<<(std::ostream&, const Figure&);
}
#endif

