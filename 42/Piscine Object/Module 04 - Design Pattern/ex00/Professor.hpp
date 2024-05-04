//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __PROFESSOR_HPP__
# define __PROFESSOR_HPP__

#include <iostream>

#include "Staff.hpp"

class Course;
class Professor: public Staff {
    public:
        Professor(const std::string & name);
        ~Professor();
    private:
        Course* current_room_;
};

std::ostream& operator<<(std::ostream&, const Professor&);

#endif
