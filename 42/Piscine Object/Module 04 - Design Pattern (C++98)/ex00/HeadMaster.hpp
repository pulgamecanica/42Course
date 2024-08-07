//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HEADMASTER_HPP__
# define __HEADMASTER_HPP__

#include <iostream>
#include <vector>

#include "Staff.hpp"

class HeadMaster: public Staff {
    public:
        HeadMaster(const std::string & name);
        ~HeadMaster();
        bool hasForm(Form * p_form);
        void receiveForm(Form* p_form);
    private:
        std::vector<Form *> forms_to_validate_;
};
std::ostream& operator<<(std::ostream&, const HeadMaster&);
#endif
