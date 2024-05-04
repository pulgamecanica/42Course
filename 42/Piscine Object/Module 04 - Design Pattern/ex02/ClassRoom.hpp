//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CLASSROOM_HPP__
# define __CLASSROOM_HPP__

#include <iostream>

#include "Room.hpp"

class Course;
class ClassRoom: public Room {
    public:
        ClassRoom();
        ~ClassRoom();
        bool canEnter(Person* p_person);
    private:
        Course * current_course_;
};
std::ostream& operator<<(std::ostream&, const ClassRoom&);
#endif
