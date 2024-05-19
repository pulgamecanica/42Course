//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HEADMASTEROFFICE_HPP__
# define __HEADMASTEROFFICE_HPP__

#include "Room.hpp"

#include <iostream>

class HeadmasterOffice: public Room {
  public:
  HeadmasterOffice();
  ~HeadmasterOffice();
  HeadmasterOffice(const HeadmasterOffice&) = default;
  HeadmasterOffice& operator= (const HeadmasterOffice&) = default;

  private:
};
std::ostream& operator<<(std::ostream&, const HeadmasterOffice&);
#endif
