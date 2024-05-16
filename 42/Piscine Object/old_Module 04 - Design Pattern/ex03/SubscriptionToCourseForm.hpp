//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SUBSCRIPTIONTOCOURSEFORM_HPP__
# define __SUBSCRIPTIONTOCOURSEFORM_HPP__

#include <iostream>

#include "Form.hpp"
#include "Student.hpp"
#include "Course.hpp"

class SubscriptionToCourseForm: public Form {
    public:
        SubscriptionToCourseForm();
        ~SubscriptionToCourseForm();
        void execute();
        void fill(Course * course, Student * student);
    private:
        Student * student_;
        Course * course_;
};
#endif
