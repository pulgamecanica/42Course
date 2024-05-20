//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm>

#include "SecretarialOffice.hpp"
#include "HeadMaster.hpp"
#include "Secretary.hpp"

SecretarialOffice::SecretarialOffice() {
    ;
}

SecretarialOffice::~SecretarialOffice() {
    ;
}

bool SecretarialOffice::canEnter(Person* p_person) {
    return (dynamic_cast<HeadMaster *>(p_person) != nullptr || dynamic_cast<Secretary *>(p_person) != nullptr);
}

void SecretarialOffice::addForm(Form * p_form) {
    if (!hasForm(p_form))
        archived_forms_.push_back(p_form);
}

bool SecretarialOffice::hasForm(Form * p_form) {
    return std::find(archived_forms_.begin(), archived_forms_.end(),  p_form) != archived_forms_.end();
}

std::ostream& operator<<(std::ostream& s, const SecretarialOffice& so) {
    s << "SecretarialOffice: " << *(dynamic_cast<const Room *>(&so));
    return (s);
}

