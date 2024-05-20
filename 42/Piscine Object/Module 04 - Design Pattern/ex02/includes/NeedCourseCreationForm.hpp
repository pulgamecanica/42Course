//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __NEEDCOURSECREATIONFORM_HPP__
# define __NEEDCOURSECREATIONFORM_HPP__

#include "Form.hpp"

#include <iostream>
#include <memory>

class Course;

class NeedCourseCreationForm: public Form {
  public:
  NeedCourseCreationForm();
  ~NeedCourseCreationForm();
  NeedCourseCreationForm(const NeedCourseCreationForm&) = default;
  NeedCourseCreationForm& operator= (const NeedCourseCreationForm&) = default;
  std::shared_ptr<Course> GetCourse();
  void FillCourse(std::string name, unsigned total_lessons, unsigned capacity);
  void Execute();
  unsigned Errors();
  const std::vector<std::string> ErrorsList();

  private:
  bool HasName();
  bool ValidCapacity();
  bool ValidLessons();
  std::shared_ptr<Course> course_;
  std::string name_;
  unsigned capacity_;
  unsigned total_lessons_;
};
std::ostream& operator<<(std::ostream&, const NeedCourseCreationForm&);
#endif
