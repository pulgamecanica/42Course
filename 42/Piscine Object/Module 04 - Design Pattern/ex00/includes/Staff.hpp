//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STAFF_HPP__
# define __STAFF_HPP__

#include "Person.hpp"

#include <iostream>

class Staff: public Person {
  public:
  explicit Staff(const std::string & name);
  virtual ~Staff() = 0;
  Staff(const Staff&) = default;
  Staff& operator= (const Staff&) = default;
  virtual void Sign() const;
};
std::ostream& operator<<(std::ostream&, const Staff&);

#endif // __STAFF_HPP__
