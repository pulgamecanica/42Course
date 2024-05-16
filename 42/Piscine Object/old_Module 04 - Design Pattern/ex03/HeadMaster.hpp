//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HEADMASTER_HPP__
# define __HEADMASTER_HPP__

#include <iostream>
#include <vector>

#include "Staff.hpp"
#include "Form.hpp"
#include "Mediator.hpp"
#include "Secretary.hpp"

class Person;
class HeadMaster: public Staff, public Mediator {
    public:
        HeadMaster(const std::string& name);
        ~HeadMaster();
        void setSecretary(Secretary * secretary);
        bool hasForm(Form *p_form);
        void receiveForm(Form *p_form);
        void archive();
        void notify(Person *sender, std::string event) const;
    private:
        std::vector<Form *> forms_to_validate_;
        Secretary * secretary_;
};
std::ostream& operator<<(std::ostream&, const HeadMaster&);
#endif
