//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Student.hpp"
#include "Course.hpp"
#include "Classroom.hpp"
#include "Professor.hpp"
#include "Headmaster.hpp"
#include "Secretary.hpp"
#include "SecretarialOffice.hpp"
#include "Form.hpp"
#include "CourseFinishedForm.hpp"
#include "NeedMoreClassRoomForm.hpp"
#include "NeedCourseCreationForm.hpp"
#include "SubscriptionToCourseForm.hpp"
#include "HeadmasterOffice.hpp"
#include "StaffRestRoom.hpp"
#include "Courtyard.hpp"
#include "CourseList.hpp"
#include "StudentList.hpp"
#include "StaffList.hpp"
#include "RoomList.hpp"

#include "ex01.inc"

void testSingletonCourses() {
  CourseList *cl = CourseList::GetInstance();
  std::vector<std::string> courses_strs = {"Biology", "Geography", "Mathematics", "English Lecture", "History"};
  for (auto course : courses_strs) {
    cl->add(std::shared_ptr<Course>(new Course(course, 9, 9)));
  }
}

void testSingletonStudents() {
  StudentList *sl = StudentList::GetInstance();
  std::vector<std::string> studs_strs = {"Billy", "Georgina", "Malinca", "Bora", "Tatiana"};
  for (auto stud : studs_strs) {
    sl->add(std::shared_ptr<Student>(new Student(stud)));
  }
}

void testSingletonStaff() {
  StaffList *sl = StaffList::GetInstance();
  std::vector<std::string> profs_strs = {"Conchis", "Teresa"};
  for (auto prof : profs_strs) {
    sl->add(
      std::dynamic_pointer_cast<Staff>(
        std::shared_ptr<Professor>(new Professor(prof))));
  }
  sl->add(
    std::dynamic_pointer_cast<Staff>(
      std::shared_ptr<Secretary>(new Secretary("Sara"))));
}

void testSingletonRoom() {
  RoomList *rl = RoomList::GetInstance();
  rl->add(
    std::dynamic_pointer_cast<Room>(
      std::shared_ptr<Classroom>(new Classroom())));
  rl->add(
    std::dynamic_pointer_cast<Room>(
      std::shared_ptr<SecretarialOffice>(new SecretarialOffice())));
  rl->add(
    std::dynamic_pointer_cast<Room>(
      std::shared_ptr<Courtyard>(new Courtyard())));
  rl->add(
    std::dynamic_pointer_cast<Room>(
      std::shared_ptr<HeadmasterOffice>(new HeadmasterOffice())));
}

template<typename S>
void printSingletons(S singleton) {
  for (size_t i = 0; i < singleton->size(); ++i)
    std::cout << i << ": " << *((*singleton)[i]) << std::endl;
  std::cout << *singleton << std::endl;
}

int main(void)
{
  if (DEBUG)
      std::cout << "Debug ON!" << std::endl;

  std::cout << " * * * * * * INIT * * * * * * * *" << std::endl;
  testSingletonCourses();
  testSingletonStudents();
  testSingletonStaff();
  testSingletonRoom();
  // Singletons
  CourseList *cl = CourseList::GetInstance();
  StudentList *sl = StudentList::GetInstance();
  StaffList *stl = StaffList::GetInstance();
  RoomList *rl = RoomList::GetInstance();
  printSingletons(cl);
  printSingletons(sl);
  printSingletons(stl);
  printSingletons(rl);
  std::cout << " * * * * * * * * * * * * * * * *" << std::endl;
  return (0);
}
