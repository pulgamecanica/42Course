//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HEADMASTER_HPP__
# define __HEADMASTER_HPP__

#include "Staff.hpp"

#include <iostream>
#include <vector>
#include <memory>

class Form;

class Headmaster: public Staff {
  public:
  Headmaster(const std::string& name);
  ~Headmaster();
  Headmaster(const Headmaster&) = default;
  Headmaster& operator= (const Headmaster&) = default;
  void ReceiveForm(std::shared_ptr<Form>& form);

  private:
  std::vector<std::shared_ptr<Form>> forms_to_validate_;
};
std::ostream& operator<<(std::ostream&, const Headmaster&);
#endif
