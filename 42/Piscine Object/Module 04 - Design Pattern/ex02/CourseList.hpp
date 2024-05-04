//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COURSELIST_HPP__
# define __COURSELIST_HPP__

#include <iostream>
#include <mutex>

#include "Singleton.hpp"
#include "Course.hpp"

class CourseList: public Singleton<Course, CourseList> {
    public:
        CourseList();
        ~CourseList();
};
std::ostream& operator<<(std::ostream&, const CourseList&);
#endif
