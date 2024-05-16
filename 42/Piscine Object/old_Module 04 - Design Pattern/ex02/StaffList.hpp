//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STAFFLIST_HPP__
# define __STAFFLIST_HPP__

#include <iostream>

#include "Singleton.hpp"
#include "Staff.hpp"

class StaffList: public Singleton<Staff, StaffList> {
    public:
        StaffList();
        ~StaffList();
    private:
};
std::ostream& operator<<(std::ostream&, const StaffList&);
#endif
