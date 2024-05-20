//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HEADMASTER_HPP__
# define __HEADMASTER_HPP__

#include "Staff.hpp"
#include "IMediator.hpp"

#include <iostream>
#include <vector>
#include <memory>

class Form;
class Student;
class Professor;
class Classroom;
class Secretary;

class Headmaster: public Staff, public IMediator {
  public:
  Headmaster(const std::string& name);
  ~Headmaster();
  Headmaster(const Headmaster&) = default;
  Headmaster& operator= (const Headmaster&) = default;
  void ReceiveForm(std::shared_ptr<Form> form) const;
  void Notify(std::shared_ptr<Person> sender, enum Notification notification) const;
  void MakeStudentAttendClass(std::shared_ptr<Student> student);
  void MakeProfessorTeachClass(std::shared_ptr<Professor> professor);
  void SetSecretary(std::shared_ptr<Secretary> secretary);

  private:
  bool HasSecretary() const;
  std::vector<std::shared_ptr<Form>> forms_to_validate_;
  std::weak_ptr<Secretary> secretary_;
};
std::ostream& operator<<(std::ostream&, const Headmaster&);
#endif
