//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __IHEADER_HPP__
# define __IHEADER_HPP__

#include <iostream>

class IHeader {
    public:
        virtual ~IHeader() {}
        virtual const std::string header() = 0;
    private:
};
#endif
