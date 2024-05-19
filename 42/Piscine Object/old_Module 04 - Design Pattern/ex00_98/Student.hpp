//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STUDENT_HPP__
# define __STUDENT_HPP__

#include <iostream>
#include <memory>
#include <vector>
#include "Person.hpp"
// #include "Classroom.hpp"

namespace SpringfieldElementary {

class Course;

class Student: public Person {
    public:
        using Person::Person; 
        ~Student();
        std::shared_ptr<Student> shared_from_this();
        // void attendClass(Classroom* p_classroom);
        void exitClass();
        void graduate(std::shared_ptr<Course> p_course);
        void addCourse(std::shared_ptr<Course> p_course);
    private:
        bool hasCourse(std::shared_ptr<Course> p_course) const;
        std::vector<std::weak_ptr<Course> > courses_;
};
std::ostream& operator<<(std::ostream&, const Student&);
}
#endif
