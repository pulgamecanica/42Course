//***************************//
//*Template by pulgamecanica*//
//***************************//
#include "Triangle.hpp"

using namespace scop;

Triangle::Triangle(point & a, point & b, point & c): Figure(a), a(a), b(b), c(c) {
	/*
	 * Given three points a(x1, y1), b(x2, y2), c(x3, y3):
	 *             | x1 y1 1 |
   * Area = .5 * | x2 y2 1 |
   *             | x3 y3 1 |
   */
	area = (((a.x - c.x) * (b.y - c.y)) - ((b.x - c.x) * (a.y - c.y))) / 2.0f;

	// s = pre_s1*(pre_s2*px + pre_s3*py)
	_pre_s1 = 1 / (2 * area);
	_pre_s2 = a.y * c.x - a.x * c.y + (c.y - a.y);
	_pre_s3 = a.x - c.x;

	// t = pre_t1*(pre_t2*px + pre_t3*py)
	_pre_t1 = 1 / (2 * area);
	_pre_t2 = a.x * b.y - a.y * b.x + (a.y - b.y);
	_pre_t3 = b.x - a.x;

}

Triangle::~Triangle() {
	std::cout << *this << " destroyed" << std::endl;
}

const point Triangle::getA() const { return (a); }
const point Triangle::getB() const { return (b); }
const point Triangle::getC() const { return (c); }

bool	Triangle::isHovered (point & p) const {
	float s, t;

	s = _pre_s1 * (_pre_s2 * p.x + _pre_s3 * p.y);
	t = _pre_t1 * (_pre_t2 * p.x + _pre_t3 * p.y);
	return (s >= 0 && t >= 0 && 1 - s - t >= 0);
}

void	Triangle::draw() const {
	int max_x;
	int max_y;

	max_x = (a.x > b.x) ? ((a.x > c.x) ? a.x : c.x) : (b.x > c.x) ? b.x : c.x; 
	max_y = (a.y > b.y) ? ((a.y > c.y) ? a.y : c.y) : (b.y > c.y) ? b.y : c.y; 

	for (int i = max_y - 1; i >= 0; --i) {
      for (int j = 0; j < max_x; ++j) {
          scop::point p{(float)j, (float)i};
          std::cout << (isHovered(p) ? "." : " " ); 
      }
      std::cout << std::endl;
  }
}


std::ostream& scop::operator<<(std::ostream& s, const Triangle& param) {
	s << "Triangle" << " A[" << param.getA().x << "," << param.getA().y << "]" << " B[" << param.getB().x << "," << param.getB().y << "]" << " C[" << param.getC().x << "," << param.getC().y << "]";
	return (s);
}



