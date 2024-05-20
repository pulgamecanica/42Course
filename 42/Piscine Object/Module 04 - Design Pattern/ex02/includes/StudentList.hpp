//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STUDENTLIST_HPP__
# define __STUDENTLIST_HPP__

#include "Singleton.hpp"
#include "Student.hpp"

#include <iostream>

class StudentList: public Singleton<Student> {
  public:
  ~StudentList();
  static StudentList *GetInstance();
 
  private:
  StudentList();
  static StudentList * instance_;
};
std::ostream& operator<<(std::ostream&, const StudentList&);
#endif
