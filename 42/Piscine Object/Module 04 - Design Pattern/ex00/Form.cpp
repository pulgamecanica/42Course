//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"

#include "Form.hpp"

const std::vector<std::string> Form::FOMR_TYPE_STR = {
    "CourseFinished",
    "NeedMoreClassRoom",
    "NeedCourseCreation",
    "SubscriptionToCours"
};

Form::Form(FormType p_formType): signed_by_staff_(false), type_(p_formType) {
    if (DEBUG)
        std::cout << YELLOW << "[Form] " << GREEN << "CREATE\t" << ENDC << "Form " << BLUE << getTypeName() << ENDC << std::endl;
}

Form::~Form() {
    if (DEBUG)
        std::cout << "Form " << *this << " destroyed" << std::endl;
}

const std::string & Form::getTypeName() const {
    return FOMR_TYPE_STR[(int)type_];
}

const enum FormType & Form::getType() const {
    return type_;
}

bool Form::isSigned() const {
    return signed_by_staff_;
}

std::ostream& operator<<(std::ostream& s, const Form& form) {
    s << BLUE << form.getTypeName() << ENDC << " Form";
    return (s);
}

