//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COURSE_HPP__
# define __COURSE_HPP__

#include <iostream>
#include <memory>
#include <vector>

class Student;
class Professor;

class Course: virtual public std::enable_shared_from_this<Course> {
  public:
  Course(const std::string& name, int total_lessons, int capacity);
  ~Course();
  Course(const Course&) = default;
  Course& operator= (const Course&) = default;
  std::ostream& LoadCourseInfo(std::ostream& s) const;
  void Subscribe(std::shared_ptr<Student> student);
  bool Graduate(std::shared_ptr<Student> student);
  void Assign(std::shared_ptr<Professor> professor);
  private:
  bool HasStudent(std::shared_ptr<Student> student) const;
  bool HasProfessor(std::shared_ptr<Professor> professor) const;
  const std::string name_;
  const int total_lessons_;
  const int capacity_;
  std::vector<std::weak_ptr<Student> > students_;
  std::weak_ptr<Professor> responsible_;
};
std::ostream& operator<<(std::ostream&, const Course&);

#endif // __COURSE_HPP__
