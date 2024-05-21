//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.hpp"
#include "Professor.hpp"
#include "Student.hpp"
#include "RoomList.hpp"
#include "Courtyard.hpp"
#include "HeadmasterOffice.hpp"
#include "SecretarialOffice.hpp"
#include "StaffRestRoom.hpp"

#include <vector>

#include "ex04.inc"

template<typename T>
void createRoom() {
RoomList::GetInstance()->add(std::shared_ptr<T>(new T()));
}

void printCourse(std::shared_ptr<Course> c) {
  if (!c) {
      std::cout << "Damn ...  no course" << std::endl;
  } else {
    std::cout << *c << std::endl;
  }
}

int main() {
  createRoom<SecretarialOffice>();
  createRoom<Courtyard>();
  createRoom<StaffRestRoom>();
  createRoom<HeadmasterOffice>();

  auto edna = std::shared_ptr<Professor>(new Professor("Edna Krabappel"));
  auto krusty = std::shared_ptr<Professor>(new Professor("Krusty the Clown"));
  auto moe = std::shared_ptr<Professor>(new Professor("Moe Szyslak"));
  auto martin = std::shared_ptr<Student>(new Student("Martin Prince"));
  auto bart = std::shared_ptr<Student>(new Student("Bart Simpson"));
  auto lisa = std::shared_ptr<Student>(new Student("Lisa Simpson"));
  auto milhouse = std::shared_ptr<Student>(new Student("Milhouse Van Houten"));
  auto nelson = std::shared_ptr<Student>(new Student("Nelson Muntz"));
  auto ralph = std::shared_ptr<Student>(new Student("Ralph Wiggum"));
  auto fat_tony = std::shared_ptr<Student>(new Student("Fat Tony"));

  School s;
  s.RecruteProfessor(edna);
  s.RecruteProfessor(krusty);
  s.RecruteStudent(martin);
  s.RecruteStudent(bart);
  s.RecruteStudent(lisa);
  s.RecruteStudent(milhouse);
  s.RecruteStudent(nelson);
  s.RecruteStudent(ralph);
  s.RecruteStudent(fat_tony);

  std::cout << s << " students:" << std::endl;
  for (auto& stud : s.GetStudents())
    if (!stud.expired())
      std::cout << " -> " << *stud.lock() << std::endl;
  std::cout << s << " professors:" << std::endl;
  for (auto& prof : s.GetProfessors())
    if (!prof.expired())
      std::cout << " -> " << *prof.lock() << std::endl;
  std::cout << "Secretary: " << *s.GetSecretary() << std::endl;
  std::cout << YELLOW << "- - - - - - - Live ... Action! - - - - - -" << ENDC << std::endl;
  std::cout << s << std::endl;
  s.RunDayRoutine();
  s.RecruteProfessor(moe);
  std::cout << YELLOW << "- - - - - - - - - - - - - - - - - - - - -" << ENDC << std::endl;
  std::cout << "Get some courses:" << std::endl;
  printCourse(s.GetCourse("Edna Krabappel's Course"));
  printCourse(s.GetCourse("Moe Szyslak's Course"));
  printCourse(s.GetCourse("Krusty the Clown's Course"));
  std::cout << YELLOW << "- - - - - - - - - - - - - - - - - - - - -" << ENDC << std::endl;
  s.RunDayRoutine();
  std::cout << YELLOW << "- - - - - - - - - - - - - - - - - - - - -" << ENDC << std::endl;
  return 0;
}