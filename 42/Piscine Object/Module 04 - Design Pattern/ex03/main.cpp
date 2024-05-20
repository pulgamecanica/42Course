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

#include "ex03.inc"


int main(void)
{
  if (DEBUG)
      std::cout << "Debug ON!" << std::endl;
  std::cout << " * * * * * * INIT * * * * * * * *" << std::endl;
  std::shared_ptr<SecretarialOffice> secretarial_office = std::shared_ptr<SecretarialOffice>(new SecretarialOffice());
  std::shared_ptr<Secretary> juan = std::shared_ptr<Secretary>(new Secretary("Juan"));
  std::shared_ptr<Professor> karina = std::shared_ptr<Professor>(new Professor("Karina"));
  std::shared_ptr<Professor> grace = std::shared_ptr<Professor>(new Professor("Grace"));
  std::shared_ptr<Course> chemestry = std::shared_ptr<Course>(new Course("Organic Chemestry", 2, 3));
  std::shared_ptr<Headmaster> conchita = std::shared_ptr<Headmaster>(new Headmaster("Conchita"));
  
  // Try to see what happens if the secretarial office is not available :O
  juan->SetCurrentRoom(secretarial_office); // This would be problematic, the forms are not save fproperly and the courses get forgotten!
  
  chemestry->Assign(karina);

  conchita->SetSecretary(juan);

  juan->SetMediator(conchita);

  std::vector<std::shared_ptr<Professor> > profs;
  profs.push_back(karina);
  profs.push_back(grace);

  for (auto prof : profs)
    prof->SetMediator(conchita);
  std::cout << " * * * * * * * * Make Professors Attend their classes * * * * * * * *" << std::endl;
  for (int i = 0; i < 5; ++i)
    for (auto prof : profs)
      conchita->MakeProfessorTeachClass(prof);
  
  std::cout << " * * * * * * * * Make Students Attend Courses And Professors teach * * * * * * * *" << std::endl;
  std::vector<std::shared_ptr<Student>> students;
  students.push_back(std::shared_ptr<Student>(new Student("Miguel")));
  students.push_back(std::shared_ptr<Student>(new Student("RoRo")));
  students.push_back(std::shared_ptr<Student>(new Student("Anabel")));
  students.push_back(std::shared_ptr<Student>(new Student("Kada")));
  
  for (auto stud : students)
    stud->SetMediator(conchita);

  for (int i = 0; i < 5; ++i) {
    for (auto stud : students)
      conchita->MakeStudentAttendClass(stud);
    for (auto prof : profs)
      conchita->MakeProfessorTeachClass(prof);
  }
  std::cout << " * * * * * * * * * * * * * * * *" << std::endl;
  return (0);
}
