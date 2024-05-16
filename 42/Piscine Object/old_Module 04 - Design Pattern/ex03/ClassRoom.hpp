//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CLASSROOM_HPP__
# define __CLASSROOM_HPP__

#include <iostream>

#include "Room.hpp"
#include "Course.hpp"

class Course;
class ClassRoom: public Room {
    public:
        ClassRoom();
        ~ClassRoom();
        void assignCourse(Course* p_course);
        bool canEnter(Person* p_person);
        Course * getCourse();
    private:
        Course * current_course_;
};
std::ostream& operator<<(std::ostream&, const ClassRoom&);
#endif
