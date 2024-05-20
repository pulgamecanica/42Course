//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "SchoolComponent.hpp"

// If initialized without a headmaster, look for the first headmaster in the stafflist
// If no headmaster is available, the class will throw a runtime error, must always have a mediator
// This is a desition I choose, because I want to make the design work in this way
SchoolComponent::SchoolComponent(HeadMaster * hm): hm_(hm) {
    // if  (hm == nullptr) {
    //     throw std::runtime_error("Could not find a mediator.");
    // }
}

SchoolComponent::~SchoolComponent() {
    ;
}
