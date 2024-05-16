//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"
#include "Professor.hpp"
#include "Course.hpp"

namespace SpringfieldElementary {

Professor::~Professor() {
    ;
}

std::shared_ptr<Course> Professor::getCourse() const {
    return current_course_;
}

void Professor::setCourse(std::shared_ptr<Course> p_course) {
    current_course_ = p_course;
}

std::ostream & operator<<(std::ostream & s, const Professor & p) {
    s << BLUE << "[Professor] " << ENDC << *(dynamic_cast<const Staff*>(&p));
    return s;
}

}
