//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __GUICONTAINER_HPP__
# define __GUICONTAINER_HPP__

#include <iostream>

/** This class is a generic Graphical User Interface Container
 * It allows you to load figures to the GUI Object
 * Figures are as follow:
 * Circle
 * Rectangle (Rounded corners, or not)

 * Figures:
 * BorderColor
 * BackgroundColor
 * BackgroundImage
 * BackgroundText
 * isHovered()
 * getX()
 * getY()
 * area()
 * perimeter()
 * draw()
 * 
 * Rectangle:
 * getWidth()
 * getHeight()
 * 
 * Circle:
 * getRad()
 * 
 * Triangle:
 * getHeight()
 * getAbsSideAB()
 * getAbsSideBC()
 * getAbsSideCA()
 * getSideA()
 * getSideB()
 * getSideC()
 */
class GUIContainer {
	public:
		GUIContainer();
		GUIContainer(const GUIContainer&);
		~GUIContainer();
		GUIContainer&	operator= (const GUIContainer&); // const for safety... not super nesessary
		// virtual void drawContainer() const;
	private:
		int width;
		int height;
		std::string name;
};

std::ostream&	operator<<(std::ostream&, const GUIContainer&);

#endif

