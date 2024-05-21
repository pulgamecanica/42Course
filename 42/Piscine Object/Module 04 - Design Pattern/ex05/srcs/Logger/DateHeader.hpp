//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __DATEHEADER_HPP__
# define __DATEHEADER_HPP__

#include <iostream>

#include "IHeader.hpp"

class DateHeader: public IHeader {
    public:
        DateHeader();
        ~DateHeader();
        const std::string header();
};
#endif
