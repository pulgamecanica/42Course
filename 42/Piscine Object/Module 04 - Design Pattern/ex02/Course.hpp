//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COURSE_HPP__
# define __COURSE_HPP__

#include <iostream>
#include <vector>

class Student;
class Professor;
class Course {
    public:
        Course(const std::string & name, int number_of_class_to_graduate = 9, int course_capacity = 42);
        ~Course();
        std::ostream& loadCourseInfo(std::ostream& s) const;
        bool hasStudent(Student * s) const;
        void assign(Professor* p_professor);
        void subscribe(Student* p_student);
        void finish();
    private:
        const std::string name_;
        Professor* responsable_;
        std::vector<Student *> students_;
        int number_of_class_to_graduate_;
        unsigned course_capacity_;
};
std::ostream& operator<<(std::ostream&, const Course&);
#endif


/**
 * this is a nice courese where students can subscribe and belong to a course
 * the events can happen at any time, the bell can ring at any given time, this can be random maybe
 * 
 * The system should be able to manage a school and all the different persons
 * 
 * Persons can be students or professors, or whatever LMAO LMAO LMAO 
 * 
 * Should there be a database? I don't know this is a good question the subject is not supper clear
 * 
 * What should we do in case of fire? I don't know the subject doesn't state it?
 * 
 * 
 * 
 * 
 * 
 * 
 */
