//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COURSELIST_HPP__
# define __COURSELIST_HPP__

#include "Singleton.hpp"
#include "Course.hpp"

#include <iostream>

class CourseList: public Singleton<Course> {
  public:
  ~CourseList();
  static CourseList *GetInstance();

  private:
  CourseList();
  static CourseList * instance_;
};
std::ostream& operator<<(std::ostream&, const CourseList&);

#endif
