//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STUDENT_HPP__
# define __STUDENT_HPP__


#include "Person.hpp"

#include <iostream>
#include <vector>
#include <memory>

class Course;
class Classroom;

class Student : public Person {
  public:
  explicit Student(const std::string& name);
  ~Student();
  Student(const Student&) = default;
  Student& operator= (const Student&) = default;
  void AddCourse(std::shared_ptr<Course> course);
  void Graduate(std::shared_ptr<Course> course);
  void AttendClass();
  void ExitClass();

  private:
  std::vector<std::weak_ptr<Course>> courses_;
};
std::ostream& operator<<(std::ostream&, const Student&);

#endif  // STUDENT_HPP_
