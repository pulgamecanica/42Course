//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "HeadmasterOffice.hpp"
#include "HeadMaster.hpp"

HeadmasterOffice::HeadmasterOffice() {
    ;
}

HeadmasterOffice::~HeadmasterOffice() {
    ;
}

bool HeadmasterOffice::canEnter(Person* p_person) {
    return (dynamic_cast<HeadMaster *>(p_person) != nullptr);
}

std::ostream& operator<<(std::ostream& s, const HeadmasterOffice& hmo) {
    s << "HeadmasterOffice: " << *(dynamic_cast<const Room *>(&hmo));
    return (s);
}

