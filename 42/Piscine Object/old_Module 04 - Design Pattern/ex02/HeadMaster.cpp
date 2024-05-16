//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm>

#include "HeadMaster.hpp"

HeadMaster::HeadMaster(const std::string & name): Staff(name) {
    ;
}

HeadMaster::~HeadMaster() {
    archive();
}

bool HeadMaster::hasForm(Form * p_form) {
    return std::find(forms_to_validate_.begin(), forms_to_validate_.end(), p_form) != forms_to_validate_.end();
}

void HeadMaster::archive() {
    for (std::vector<Form *>::iterator i = forms_to_validate_.begin(); i != forms_to_validate_.end(); ++i)
        delete *i;
    forms_to_validate_.clear();
};

// Recieve and sign form
void HeadMaster::receiveForm(Form* p_form) {
    if (!hasForm(p_form)) {
        forms_to_validate_.push_back(p_form);
    }
}

std::ostream& operator<<(std::ostream& s, const HeadMaster& hm) {
    s << "HeadMaster: " << *(dynamic_cast<const Staff *>(&hm));
    return (s);
}

