//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __MODEL3D_HPP__
# define __MODEL3D_HPP__

#include <iostream>
#include "GUIContainer.hpp"

class Model3D {
	public:
		Model3D();
		Model3D(const Model3D&);
		~Model3D();
		Model3D&	operator= (const Model3D&); // const for safety... not super nesessary
};

std::ostream&	operator<<(std::ostream&, const Model3D&);

#endif

