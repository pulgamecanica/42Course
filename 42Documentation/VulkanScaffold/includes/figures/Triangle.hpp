//***************************//
//*Template by pulgamecanica*//
//***************************//
#ifndef __TRIANGLE_HPP__
# define __TRIANGLE_HPP__

#include <iostream>
#include "Figure.hpp"

namespace scop {
	class Triangle: public virtual Figure {
		public:
			Triangle(point & a, point & b, point & c);
			~Triangle();
	    virtual bool	isHovered (point & p) const;
			virtual void	draw() const;
 			const point getA() const;
 			const point getB() const;
 			const point getC() const;
		private:
			point a;
			point b;
			point c;
			float area;
			float _pre_s1;
			float _pre_s2;
			float _pre_s3;
			float _pre_t1;
			float _pre_t2;
			float _pre_t3;
	};

	std::ostream&	operator<<(std::ostream&, const Triangle&);
}
#endif

