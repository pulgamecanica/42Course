#include "figures.hpp"

Circle::Circle(point p, int radius, char fill) {
	r = radius;
	center = p;
	filled = fill != 0;
	chr = fill;
}

Circle::~Circle()
{
	// delete malloc variables....
}

bool	Circle::belongs_to(point p) {
	if (filled)
		return (((p.x - center.x) * (p.x - center.x)) +
			((p.y - center.y) * (p.y - center.y)) <= r*r);
	else
		return (((p.x - center.x) * (p.x - center.x)) +
                        ((p.y - center.y) * (p.y - center.y)) == r*r);
}

char	Circle::get_chr(void)
{
	if (chr == 0)
		return ('\\');
	return (chr);
}
