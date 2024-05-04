//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SECRETARY_HPP__
# define __SECRETARY_HPP__

#include <iostream>

#include "Staff.hpp"
#include "Form.hpp"

class Secretary: public Staff {
    public:
        Secretary(const std::string & name);
        ~Secretary();
        Form* createForm(FormType p_formType);
        void archiveForm();
    private:
};
std::ostream& operator<<(std::ostream&, const Secretary&);
#endif
