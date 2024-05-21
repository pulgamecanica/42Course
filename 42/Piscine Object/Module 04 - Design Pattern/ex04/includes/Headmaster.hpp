//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HEADMASTER_HPP__
# define __HEADMASTER_HPP__

#include "Staff.hpp"
#include "IMediator.hpp"
#include "Bell.hpp"

#include <iostream>
#include <vector>
#include <memory>

class Form;
class Student;
class Professor;
class Classroom;
class Secretary;

/**
 * Headmaster has a bell he can ring
 * The bell is a subject which can be observed by students and professors
 **/
class Headmaster: public Staff, public IMediator {
  public:
  Headmaster(const std::string& name);
  ~Headmaster();
  Headmaster(const Headmaster&) = default;
  Headmaster& operator= (const Headmaster&) = default;
  void EnterOffice();
  void ReceiveForm(std::shared_ptr<Form> form) const;
  void Notify(std::shared_ptr<Person> sender, enum Notification notification) const;
  void MakeStudentAttendClass(std::shared_ptr<Student> student);
  void MakeProfessorTeachClass(std::shared_ptr<Professor> professor);
  void SetSecretary();
  void SetObservers();
  void RingBell();

  private:
  bool HasSecretary() const;
  std::vector<std::shared_ptr<Form>> forms_to_validate_;
  std::weak_ptr<Secretary> secretary_;
  Bell bell_;
};
std::ostream& operator<<(std::ostream&, const Headmaster&);
#endif
