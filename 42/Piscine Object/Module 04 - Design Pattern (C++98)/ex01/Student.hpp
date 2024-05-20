//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STUDENT_HPP__
# define __STUDENT_HPP__

#include <iostream>
#include <vector>

#include "ClassRoom.hpp"
#include "Person.hpp"

class Course;
class Student: public Person {
    public:
        Student(const std::string & name);
        ~Student();
        void attendClass(ClassRoom* p_classroom);
        void exitClass();
        void graduate(Course* p_course);
    private:
        std::vector<Course*> courses_;
        ClassRoom * current_classroom_;
};
std::ostream& operator<<(std::ostream&, const Student&);
#endif
