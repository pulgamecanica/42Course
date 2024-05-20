//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SECRETARIALOFFICE_HPP__
# define __SECRETARIALOFFICE_HPP__

#include "Room.hpp"

#include <iostream>

class Form;

class SecretarialOffice: public Room {
  public:
  SecretarialOffice();
  ~SecretarialOffice();
  SecretarialOffice(const SecretarialOffice&) = default;
  SecretarialOffice& operator= (const SecretarialOffice&) = default;
  void ArchiveForm(std::shared_ptr<Form>& form);

  private:
  std::vector<std::shared_ptr<Form>> archived_forms_;
};
std::ostream& operator<<(std::ostream&, const SecretarialOffice&);
#endif
