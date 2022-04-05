//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HARL_HPP__
# define __HARL_HPP__

#include <string>
#include <iostream>

class Harl {
	static void debug( void );
	static void info( void );
	static void warning( void );
	static void error( void );
	public:
		void	complain( std::string level );
};

#endif

