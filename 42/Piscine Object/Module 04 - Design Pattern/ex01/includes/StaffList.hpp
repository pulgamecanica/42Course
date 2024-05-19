//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STAFFLIST_HPP__
# define __STAFFLIST_HPP__

#include "Singleton.hpp"
#include "Staff.hpp"

#include <iostream>


class StaffList: public Singleton<Staff> {
  public:
  ~StaffList();
  static StaffList *GetInstance();
 
  private:
  StaffList();
  static StaffList * instance_;
};
std::ostream& operator<<(std::ostream&, const StaffList&);
#endif
