//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COURSEFINISHEDFORM_HPP__
# define __COURSEFINISHEDFORM_HPP__

#include <iostream>

#include "Form.hpp"
#include "Course.hpp"

class CourseFinishedForm: public Form {
    public:
        CourseFinishedForm();
        ~CourseFinishedForm();
        void execute();
        void fill(Course *c);
    private:
        Course *course_;
};
#endif
