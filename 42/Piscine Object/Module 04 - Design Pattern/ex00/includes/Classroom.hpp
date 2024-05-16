//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CLASSROOM_HPP__
# define __CLASSROOM_HPP__

#include <iostream>

#include "Room.hpp"

class Classroom : public Room {
  public:
  Classroom();
  ~Classroom();
  Classroom(const Classroom&) = default;
  Classroom& operator= (const Classroom&) = default;
};
std::ostream& operator<<(std::ostream&, const Classroom&);

#endif  // __CLASSROOM_HPP__
