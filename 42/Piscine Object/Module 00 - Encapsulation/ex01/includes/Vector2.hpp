//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __VECTOR2_HPP__
# define __VECTOR2_HPP__

#include <iostream>

class Vector2 {
	public:
		Vector2(float x, float y);
		~Vector2();
		float x() const;
		float y() const;
		bool operator==(const Vector2& lhs) const;
	private:
		float x_;
		float y_;
};
bool operator==(Vector2 &lhs, Vector2 &rhs);
std::ostream&	operator<<(std::ostream&, const Vector2&);

#endif

