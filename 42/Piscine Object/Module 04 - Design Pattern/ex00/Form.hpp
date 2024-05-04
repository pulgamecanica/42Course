//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __FORM_HPP__
# define __FORM_HPP__

#include <iostream>
#include <vector>

enum class FormType {
    CourseFinished,
    NeedMoreClassRoom,
    NeedCourseCreation,
    SubscriptionToCourse
};

class Form {
    static const std::vector<std::string> FOMR_TYPE_STR;
    public:
        Form(FormType p_formType);
        virtual ~Form();
        virtual void execute() = 0;
        const std::string & getTypeName() const;
        const enum FormType & getType() const;
        bool isSigned() const;
    protected:
        bool signed_by_staff_;
    private:
        enum FormType type_;
};
std::ostream& operator<<(std::ostream&, const Form&);
#endif
