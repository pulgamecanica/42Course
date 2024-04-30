//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CONSTHEADER_HPP__
# define __CONSTHEADER_HPP__

#include <iostream>

#include "IHeader.hpp"

class ConstHeader: public IHeader {
    public:
        ConstHeader(const std::string header_str);
        ~ConstHeader();
        virtual const std::string header();
    private:
        const std::string header_str_;
};
#endif
