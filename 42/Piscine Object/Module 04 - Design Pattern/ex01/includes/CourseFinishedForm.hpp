//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COURSEFINISHEDFORM_HPP__
# define __COURSEFINISHEDFORM_HPP__

#include "Form.hpp"

#include <iostream>
#include <memory>

class Course;

class CourseFinishedForm: public Form {
  public:
  CourseFinishedForm();
  ~CourseFinishedForm();
  CourseFinishedForm(const CourseFinishedForm&) = default;
  CourseFinishedForm& operator= (const CourseFinishedForm&) = default;
  void FillCourse(std::shared_ptr<Course> course);
  void Execute();
  unsigned Errors();
  const std::vector<std::string> ErrorsList();
  
  private:
    bool ValidCourse() const;
    bool CourseCanFinish() const;
    std::weak_ptr<Course> course_;
};
std::ostream& operator<<(std::ostream&, const CourseFinishedForm&);
#endif
