//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Secretary.hpp"
#include "CourseFinishedForm.hpp"
#include "NeedCourseCreationForm.hpp"
#include "NeedMoreClassRoomForm.hpp"
#include "SubscriptionToCourseForm.hpp"
#include "HeadMaster.hpp"
#include "StaffList.hpp"

Secretary::Secretary(const std::string & name):
    Staff(name) {
    ;
}

Secretary::~Secretary() {
    // std::cout << "Secretary" << " destroyed" << std::endl;
}

Form* Secretary::createForm(FormType p_formType) {
    switch (p_formType) {
        case FormType::CourseFinished : return new CourseFinishedForm();
        case FormType::NeedCourseCreation : return new NeedCourseCreationForm();
        case FormType::NeedMoreClassRoom : return new NeedMoreClassRoomForm();
        case FormType::SubscriptionToCourse : return new SubscriptionToCourseForm();
        default : return nullptr;
    }
}

void Secretary::archiveForms() {
    HeadMaster * hm = dynamic_cast<HeadMaster *>(StaffList::GetInstance()->get<HeadMaster *>());
    if (hm != nullptr) {
        hm->archive();
    }
}

std::ostream& operator<<(std::ostream& s, const Secretary& sct) {
    s << "Secretary: " << *(dynamic_cast<const Staff *>(&sct));
    return s;
}
