//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __NEEDCOURSECREATIONFORM_HPP__
# define __NEEDCOURSECREATIONFORM_HPP__

#include <iostream>

#include "Form.hpp"
#include "Course.hpp"

class NeedCourseCreationForm: public Form {
    public:
        NeedCourseCreationForm();
        ~NeedCourseCreationForm();
        void execute();
        void fill(const std::string & course_name, int number_of_class_to_graduate, int course_capacity);
        Course *getCourse();
    private:
        std::string course_name_;
        int number_of_class_to_graduate_;
        int course_capacity_;
        Course * new_course_;
};
#endif
