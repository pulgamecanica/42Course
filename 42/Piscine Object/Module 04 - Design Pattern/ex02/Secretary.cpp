//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Secretary.hpp"
#include "CourseFinishedForm.hpp"
#include "NeedCourseCreationForm.hpp"
#include "NeedMoreClassRoomForm.hpp"
#include "SubscriptionToCourseForm.hpp"
#include "CourseList.hpp"
#include "StaffList.hpp"
#include "StudentList.hpp"
#include "HeadMaster.hpp"

Secretary::Secretary(const std::string & name):
    Staff(name) {
    ;
}

Secretary::~Secretary() {
    // std::cout << "Secretary" << " destroyed" << std::endl;
}

Form* Secretary::createForm(FormType p_formType, size_t course_index, size_t student_index) {
    switch (p_formType) {
        case FormType::CourseFinished : {
            if (CourseList::GetInstance()->size() < course_index)
                return nullptr ;
            return new CourseFinishedForm(&((*(CourseList::GetInstance()))[course_index]));
        }
        case FormType::NeedCourseCreation : {
            return new NeedCourseCreationForm("Rust for the Linux kernel", 9, 6);
        }
        case FormType::NeedMoreClassRoom : {
            return new NeedMoreClassRoomForm();
        }
        case FormType::SubscriptionToCourse : {
            if (CourseList::GetInstance()->size() < course_index || StudentList::GetInstance()->size() < student_index)
                return nullptr ;
            return new SubscriptionToCourseForm(&((*(CourseList::GetInstance()))[course_index]), &((*(StudentList::GetInstance()))[course_index]));
        }
        default : return nullptr;
    }
}

void Secretary::archiveForm() {
    HeadMaster * hm = dynamic_cast<HeadMaster *>(StaffList::GetInstance()->get<HeadMaster *>());
    if (hm != nullptr) {
        hm->archive();
    }
}

std::ostream& operator<<(std::ostream& s, const Secretary& sct) {
    s << "Secretary: " << *(dynamic_cast<const Staff *>(&sct));
    return s;
}
