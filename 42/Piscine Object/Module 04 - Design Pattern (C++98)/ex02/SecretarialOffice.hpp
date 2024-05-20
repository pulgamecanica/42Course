//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SECRETARIALOFFICE_HPP__
# define __SECRETARIALOFFICE_HPP__

#include <iostream>
#include <vector>

#include "Room.hpp"
#include "Form.hpp"

class SecretarialOffice: public Room {
    public:
        SecretarialOffice();
        ~SecretarialOffice();
        void addForm(Form * p_form);
        bool hasForm(Form * p_form);
        bool canEnter(Person* p_person);
    private:
        std::vector<Form*> archived_forms_;
};
std::ostream& operator<<(std::ostream&, const SecretarialOffice&);
#endif
