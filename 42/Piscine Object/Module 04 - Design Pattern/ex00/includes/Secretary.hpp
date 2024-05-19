//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SECRETARY_HPP__
# define __SECRETARY_HPP__

#include "Staff.hpp"
#include "Form.hpp"

#include <iostream>
#include <memory>

class SecretarialOffice;

class Secretary: public Staff {
  public:
  explicit Secretary(const std::string & name);
  ~Secretary();
  Secretary(const Secretary&) = default;
  Secretary& operator= (const Secretary&) = default;
  std::shared_ptr<Form> CreateForm(FormType p_formType);
  void ArchiveForm(std::shared_ptr<Form>& form);
};
std::ostream& operator<<(std::ostream&, const Secretary&);
#endif
