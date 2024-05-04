//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STUDENT_HPP__
# define __STUDENT_HPP__

#include <iostream>
#include <map>

#include "ClassRoom.hpp"
#include "Person.hpp"

class Course;
class Student: public Person {
    public:
        Student(const std::string & name);
        ~Student();
        bool hasCourse(Course * p_course) const;
        void printStudentProgram() const;
        void addCourse(Course * p_course);
        void attendClass(ClassRoom* p_classroom);
        void exitClass();
        void graduate(Course* p_course);
        int getCourseAttendance(Course * p_course);
    private:
        std::map<Course*, int> courses_; // The value refers to the student attendance
};
std::ostream& operator<<(std::ostream&, const Student&);
#endif
