//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "SchoolComponent.hpp"
#include "HeadMaster.hpp"
#include "Mediator.hpp"

SchoolComponent::SchoolComponent(Mediator * mediator): mediator_(mediator) {
    ;
}

// SchoolComponent::~SchoolComponent() {
//     ;
// }

void SchoolComponent::set_mediator(Mediator *mediator) {
    this->mediator_ = mediator;
}
