//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __MENU_HPP__
# define __MENU_HPP__

#include <iostream>
#include "GUIContainer.hpp"

/**
 * ################################################
 * #                                              #
 * #                      42                      #
 * #                     SCOP                     #
 * #                     ====                     #
 * #                 -------------                # 
 * #                |Select Model|                #
 * #                -------------                 #
 * #    ____    ____    ____     ____     ____    #
 * #   l   l   2   2   3   3    4   4    5   5    #
 * #   l___l   2___2   3___3    4___4    5___5    #
 * #                                              #
 * #                     ====                     #
 * #                   ---------                  # 
 * #                  |Settings|                  #
 * #                  ---------                   #
 * #                                              #
 * ################################################
 */

class Menu: public virtual GUIContainer {
	public:
		Menu();
		Menu(const Menu&);
		~Menu();
		Menu&	operator= (const Menu&); // const for safety... not super nesessary
		virtual void drawContainer() const;
	private:
		// std::map -> modelname: modelThumbnails
};

std::ostream&	operator<<(std::ostream&, const Menu&);

#endif

