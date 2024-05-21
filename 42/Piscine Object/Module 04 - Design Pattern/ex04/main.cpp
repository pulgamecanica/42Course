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

#include "ex04.inc"

template<typename T>
void createRoom() {
  RoomList::GetInstance()->add(std::shared_ptr<T>(new T()));
}

template<typename T>
void createStaff(const std::string & name) {
  auto staff = std::shared_ptr<T>(new T(name));
  staff->EnterOffice();
  StaffList::GetInstance()->add(staff);
}

void createStudent(const std::string & name) {
  auto stud = std::shared_ptr<Student>(new Student(name));
  StudentList::GetInstance()->add(stud);
}

template<typename S>
void setMediator() {
  auto mediator = std::dynamic_pointer_cast<Headmaster>(StaffList::GetInstance()->template getFirst<Headmaster>());
  if (!mediator) {
    std::cout << "Warning mediator not found, creating staff without mediator would be problematic for the Mediator Design!" << std::endl;
    exit(-1);
  }
  S * sl = S::GetInstance();
  for (size_t i = 0; i < sl->size(); ++i) {
    (*sl)[i]->SetMediator(mediator);
  }
}

int main(void)
{
  if (DEBUG)
      std::cout << "Debug ON!" << std::endl;

  std::cout << " * * * * * * INIT * * * * * * * *" << std::endl;
  // Create the school rooms!
  createRoom<SecretarialOffice>();
  createRoom<HeadmasterOffice>();
  createRoom<StaffRestRoom>();
  createRoom<Courtyard>();
  createRoom<Classroom>();
  createRoom<Courtyard>();
  // Create the school staff!
  createStaff<Secretary>("Juan");
  createStaff<Professor>("Karina");
  createStaff<Professor>("Grace");
  createStaff<Headmaster>("Conchita");
  // Create the school students!
  createStudent("Miguel");
  createStudent("RoRo");
  createStudent("Anabel");
  createStudent("Kada");
  createStudent("Maria");
  // Set mediators on the singletons 
  setMediator<StaffList>();
  setMediator<StudentList>();
  // Get the headmaster
  auto headmaster = std::dynamic_pointer_cast<Headmaster>(StaffList::GetInstance()->getFirst<Headmaster>());
  headmaster->SetObservers();
  std::cout << " * * * * * * * * Ring Bell * * * * * * * *" << std::endl;
  headmaster->RingBell();
  headmaster->RingBell();
  headmaster->RingBell();
  headmaster->RingBell();
  headmaster->RingBell();
  std::cout << " * * * * * * * * * * * * * * * *" << std::endl;
  return (0);
}
