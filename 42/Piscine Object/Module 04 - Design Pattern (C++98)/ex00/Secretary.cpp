//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Secretary.hpp"

Secretary::Secretary(const std::string & name):
    Staff(name) {
}

Secretary::~Secretary() {
    // std::cout << "Secretary" << " destroyed" << std::endl;
}

Form* Secretary::createForm(FormType p_formType) {
    switch (p_formType) {
        // case CourseFinished : return new CourseFinishedForm();
        // case NeedMoreClassRoom : return new NeedCourseCreationForm();
        // case NeedCourseCreation : return new NeedCourseCreationForm();
        // case SubscriptionToCourse : return new SubscriptionToCourse();
        default : return nullptr;
    }
}

void Secretary::archiveForm() {
    std::cout << "Achieve the Form" << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Secretary& sct) {
    s << "Secretary: " << *(dynamic_cast<const Staff *>(&sct));
    return s;
}
