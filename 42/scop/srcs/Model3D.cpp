//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Model3D.hpp"

Model3D::Model3D() {
	// TODO (default constructor)
}

Model3D::~Model3D() {
	std::cout << "Model3D" << " destroyed" << std::endl;
	// TODO (destructor)
}

// Model3D::Model3D(const Model3D& param) {
// 	// TODO (copy constructor)
// 	(void)param;
// }

// Model3D& Model3D::operator= (const Model3D& param) {
// 	// TODO (Assignment operatior)
// 	// std::swap()
// 	(void)param;
// 	return (*this);
// }

std::ostream& operator<<(std::ostream& s, const Model3D& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

