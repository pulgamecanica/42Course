//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __PROFESSOR_HPP__
# define __PROFESSOR_HPP__

#include <iostream>

#include "Staff.hpp"

namespace SpringfieldElementary {

class Course;

class Professor: public Staff {
    public:
        using Staff::Staff; // Inherit constructors
        ~Professor();
        std::shared_ptr<Course> getCourse() const;
        void setCourse(std::shared_ptr<Course> p_course);
    private:
        std::shared_ptr<Course> current_course_;
};
std::ostream & operator<<(std::ostream &, const Professor &);
}
#endif
