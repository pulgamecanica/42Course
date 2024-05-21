//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __PROFESSOR_HPP__
# define __PROFESSOR_HPP__

#include "Staff.hpp"
#include "IObserver.hpp"

#include <iostream>
#include <memory>

class Course;

class Professor: public Staff, public IObserver {
  public:
  explicit Professor(const std::string& name);
  ~Professor();
  Professor(const Professor&) = default;
  Professor& operator= (const Professor&) = default;
  void Update(enum Event e);
  void EnterOffice();
  void SetCourse(std::shared_ptr<Course> course);
  void RemoveCourse();
  void Teach();
  std::shared_ptr<Course> GetCurrentCourse() const;
  
  private:
  bool NeedCourse() const;
  bool HasClassroom() const;
  std::weak_ptr<Course> current_course_;
};
std::ostream& operator<<(std::ostream&, const Professor&);
#endif
