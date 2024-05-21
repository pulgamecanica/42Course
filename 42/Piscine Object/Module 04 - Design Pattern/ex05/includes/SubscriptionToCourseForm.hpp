//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SUBSCRIPTIONTOCOURSEFORM_HPP__
# define __SUBSCRIPTIONTOCOURSEFORM_HPP__

#include "Form.hpp"

#include <iostream>
#include <memory>

class Student;
class Course;

class SubscriptionToCourseForm: public Form {
  public:
  SubscriptionToCourseForm();
  ~SubscriptionToCourseForm();
  SubscriptionToCourseForm(const SubscriptionToCourseForm&) = default;
  SubscriptionToCourseForm& operator= (const SubscriptionToCourseForm&) = default;
  void FillCourseAndStudent(std::shared_ptr<Course> course, std::shared_ptr<Student> student);
  void Execute();
  unsigned Errors();
  const std::vector<std::string> ErrorsList();
  
  private:
  bool HasCourse();
  bool HasStudent();
  std::shared_ptr<Course> course_;
  std::shared_ptr<Student> student_;
};
std::ostream& operator<<(std::ostream&, const SubscriptionToCourseForm&);
#endif
