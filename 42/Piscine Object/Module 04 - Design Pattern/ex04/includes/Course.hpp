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
  Course(const std::string& name, unsigned lessons_to_graduate, unsigned capacity);
  ~Course();
  Course(const Course&) = default;
  Course& operator= (const Course&) = default;
  std::ostream& LoadCourseInfo(std::ostream& s) const;
  void Subscribe(std::shared_ptr<Student> student);
  bool Graduate(std::shared_ptr<Student> student);
  void Assign(std::shared_ptr<Professor> professor);
  void EndCourse();
  void GiveClass();
  std::shared_ptr<Professor> GetResponsible() const;
  unsigned LessonsLeft() const;
  bool IsFinished() const;
  private:
  bool HasStudent(std::shared_ptr<Student> student) const;
  bool HasProfessor(std::shared_ptr<Professor> professor) const;
  const std::string name_;
  unsigned lessons_to_graduate_;
  const unsigned capacity_;
  bool finished_;
  std::vector<std::weak_ptr<Student> > students_;
  std::weak_ptr<Professor> responsible_;
};
std::ostream& operator<<(std::ostream&, const Course&);

#endif // __COURSE_HPP__
