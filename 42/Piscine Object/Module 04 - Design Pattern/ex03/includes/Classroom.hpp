//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CLASSROOM_HPP__
# define __CLASSROOM_HPP__

#include <iostream>
#include <memory>

#include "Room.hpp"

class Course;

class Classroom : public Room {
  public:
  Classroom();
  ~Classroom();
  Classroom(const Classroom&) = default;
  Classroom& operator=(const Classroom&) = default;
  void SetCourse(std::shared_ptr<Course> course);
  void RemoveCourse();
  bool CanEnter(std::shared_ptr<Person> person);
  private:
  std::weak_ptr<Course> current_course_;
};
std::ostream& operator<<(std::ostream&, const Classroom&);

#endif  // __CLASSROOM_HPP__
