#include "bsp.hpp"


static Fixed area(Point const a, Point const b, Point const c)
{
	Fixed	res(
   		(a.get_x() * (b.get_y() - c.get_y())
   		+ b.get_x() * (c.get_y() - a.get_y())
   		+ c.get_x() * (a.get_y() - b.get_y()))
   		/ Fixed(2.0f));
	if (res < 0)
		return (Fixed(res * -1));
	return (res);
}

bool bsp(Point const a, Point const b, Point const c, Point const point) {
	Fixed real_area = area(a, b, c);
	Fixed area1 = area(a, b, point);
	Fixed area2 = area(a, point, c);
	Fixed area3 = area(point, b, c);

	return(real_area == area1 + area2 + area3);
}