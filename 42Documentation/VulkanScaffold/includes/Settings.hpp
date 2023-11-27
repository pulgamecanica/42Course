//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SETTINGS_HPP__
# define __SETTINGS_HPP__

#include <iostream>
#include "GUIContainer.hpp"

class Settings: public virtual GUIContainer {
	public:
		Settings();
		// Settings(const Settings&);
		~Settings();
		// Settings&	operator= (const Settings&); // const for safety... not super nesessary
	private:
		// Font
		// FontSize
		// BackgroundID
		// BackgroundColor
		// Iterations
		// ZoomIncrease
		// Show Settings
		// Show FPS
		// Show Time
		// Show Axis
		//
		// Enable debug txt file?
};

std::ostream&	operator<<(std::ostream&, const Settings&);

#endif

