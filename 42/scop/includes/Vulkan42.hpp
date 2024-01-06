//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __VULKAN42_HPP__
# define __VULKAN42_HPP__

#include <iostream>

class Vulkan42 {
	public:
		Vulkan42();
		Vulkan42(const Vulkan42&);
		~Vulkan42();
		Vulkan42&	operator= (const Vulkan42&); // const for safety... not super nesessary
};

std::ostream&	operator<<(std::ostream&, const Vulkan42&);

#endif

