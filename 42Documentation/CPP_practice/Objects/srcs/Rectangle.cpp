#include "figures.hpp"

void	Rectangle::set(point p, int width, int height, char fill) {
	w = width;
	h = height;
	tlc = p;
	filled = fill != 0;
	chr = fill;
}

bool	Rectangle::belongs_to(point p) {
	if (filled)
		return (p.x >= tlc.x && p.x <= tlc.x + w && p.y >= tlc.y && p.y <= tlc.y + h);
	else
                return (p.x == tlc.x || p.x == tlc.x + w || p.y == tlc.y || p.y == tlc.y + h);
}

char	Rectangle::get_chr(void)
{
	if (chr == 0)
		return ('|');
	return (chr);
}
