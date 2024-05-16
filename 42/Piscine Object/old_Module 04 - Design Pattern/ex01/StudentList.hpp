//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STUDENTLIST_HPP__
# define __STUDENTLIST_HPP__

#include <iostream>

#include "Singleton.hpp"
#include "Student.hpp"

class StudentList: public Singleton<Student, StudentList> {
    public:
        StudentList();
        ~StudentList();
    private:
};
std::ostream& operator<<(std::ostream&, const StudentList&);
#endif
