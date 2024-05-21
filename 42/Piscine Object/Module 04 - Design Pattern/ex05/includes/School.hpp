//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SCHOOL_HPP__
# define __SCHOOL_HPP__

#include "Course.hpp"
#include "Student.hpp"
#include "Headmaster.hpp"
#include "Secretary.hpp"

#include <iostream>
#include <memory>
#include <vector>

#include "StreamLogger.hpp"
#include "ConstHeader.hpp"


class School {
  public:
  School();
  ~School();
  School(const School&) = default;
  School& operator= (const School&) = default;
  void Log(const std::string & msg);
  void RunDayRoutine();
  void LaunchClasses();
  void RequestRingBell();
  void RecruteProfessor(std::shared_ptr<Professor> prof); 
  void RecruteStudent(std::shared_ptr<Student> stud);
  std::shared_ptr<Course> GetCourse(std::string p_name);
  std::vector<std::weak_ptr<Student>> GetStudents();
  std::vector<std::weak_ptr<Professor>> GetProfessors();
  const std::shared_ptr<Headmaster>& GetHeadmaster() const;
  const std::shared_ptr<Secretary>& GetSecretary() const;
  void GraduationCeremony();
  private:
  std::shared_ptr<Headmaster> headmaster_;
  std::shared_ptr<Secretary> secretary_;
  StreamLogger log_;
  ConstHeader header_;
  std::vector<std::weak_ptr<Professor>> professors_;
  std::vector<std::weak_ptr<Student>> students_;
  int day_;
};
std::ostream& operator<<(std::ostream&, const School&);
#endif
