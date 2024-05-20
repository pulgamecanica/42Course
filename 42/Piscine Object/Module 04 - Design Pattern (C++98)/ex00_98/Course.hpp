//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COURSE_HPP__
# define __COURSE_HPP__

#include <iostream>
#include <memory>
#include <vector>

namespace SpringfieldElementary {

class Student;

class Professor;

class Course {
public:
    Course(std::string p_name, int capacity, int total_lessons_);
    ~Course();
    std::ostream& loadCourseInfo(std::ostream& s) const;
    void assign(std::shared_ptr<Professor> p_professor);
    void subscribe(std::shared_ptr<Student> p_student);
    void unsubscribe(std::shared_ptr<Student> p_student);
private:
    bool hasStudent(std::shared_ptr<Student> p_student) const;
    bool hasProfessor(std::shared_ptr<Professor> p_professor) const;
    std::string name_;
    std::shared_ptr<Professor> responsible_;
    std::vector<std::weak_ptr<Student> > students_;
    const int total_lessons_;
    const int capacity_;
};
std::ostream& operator<<(std::ostream&, const Course&);
}
#endif
