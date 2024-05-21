//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COURTYARD_HPP__
# define __COURTYARD_HPP__

#include "Room.hpp"

#include <iostream>

class Courtyard: public Room {
  public:
  Courtyard();
  ~Courtyard();
  Courtyard(const Courtyard&) = default;
  Courtyard& operator= (const Courtyard&) = default;
  bool CanEnter(std::shared_ptr<Person> person);

  private:
};
std::ostream& operator<<(std::ostream&, const Courtyard&);
#endif
