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

#include "ex00.inc"


void testIndependence() {
  // Course exists, members get destroyed without warning
  std::cout << YELLOW " - - - - - - - - - - - - - -  - - -" << ENDC << std::endl;
  {
    std::shared_ptr<Course> c = std::shared_ptr<Course>(new Course("Maths", 10, 9));
    {
      std::cout << "Inside the scope, create Course members" << std::endl;
      std::shared_ptr<Student> s1 = std::make_shared<Student>("Camila");
      std::shared_ptr<Student> s2 = std::make_shared<Student>("Lauren");
      std::shared_ptr<Professor> p = std::make_shared<Professor>("Gabo");
      c->Subscribe(s1);
      c->Subscribe(s1); // double subscribe 
      c->Assign(p);
      c->Assign(p); // double assignment
      c->Subscribe(s2);
      c->Subscribe(s2); // double subscribe
      std::cout << *c << std::endl;
    }
    std::cout << "Outside the scope Course members were destroyed" << std::endl;
    std::cout << *c << std::endl;
  }
  std::cout << YELLOW " - - - - - - - - - - - - - -  - - -" << ENDC << std::endl;
  {
    std::shared_ptr<Student> s1 = std::shared_ptr<Student>(new Student("Camila")); // Avoid destructor message, don't use copy
    std::shared_ptr<Student> s2 = std::shared_ptr<Student>(new Student("Lauren"));
    std::shared_ptr<Professor> p = std::shared_ptr<Professor>(new Professor("Gabo"));
    {
      std::cout << "Inside the scope, assign members" << std::endl;
      std::shared_ptr<Course> c = std::shared_ptr<Course>(new Course("Maths", 10, 9));
      c->Subscribe(s1);
      c->Subscribe(s1); // double subscribe 
      c->Assign(p);
      c->Assign(p); // double assignment
      c->Subscribe(s2);
      c->Subscribe(s2); // double subscribe
    }
    std::cout << "Outside the scope Course was destroyed" << std::endl;
    std::cout << *s1 << std::endl;
    std::cout << *s2 << std::endl;
    std::cout << *p << std::endl;
  }
  std::cout << YELLOW " - - - - - - - - - - - - - -  - - -" << ENDC << std::endl;
}

template<typename T>
void CheckSharedNullptr(std::shared_ptr<T> elem) {
  if (elem)
    std::cout << *elem << std::endl;
  else
    std::cout << "SharedPtr is nullptr! " << std::endl;
}


// void CourseCheck(std::shared_ptr<Course> course) {
//   if (course)
//     std::cout << *course << std::endl;
//   else
//     std::cout << "Course is nullptr! " << std::endl;
// }

int main(void)
{
  if (DEBUG)
      std::cout << "Debug ON!" << std::endl;
  testIndependence();

  std::cout << " * * * * * * INIT * * * * * * * *" << std::endl;
  // Rooms
  std::shared_ptr<StaffRestRoom> rest_room = std::shared_ptr<StaffRestRoom>(new StaffRestRoom());
  std::shared_ptr<Courtyard> courtyard = std::shared_ptr<Courtyard>(new Courtyard());

  // Courses 
  std::shared_ptr<Course> bio = std::shared_ptr<Course>(new Course("Biology", 10, 9));
  std::shared_ptr<Course> geo = std::shared_ptr<Course>(new Course("Geography", 1, 2));
  std::shared_ptr<Course> math = std::shared_ptr<Course>(new Course("Mathematics", 10, 5));
  std::shared_ptr<Course> eng = std::shared_ptr<Course>(new Course("English Lecture", 6, 6));
  std::shared_ptr<Course> hist = std::shared_ptr<Course>(new Course("History", 7, 3));
  // Professors
  std::shared_ptr<Professor> lola = std::shared_ptr<Professor>(new Professor("Dolores"));
  std::shared_ptr<Professor> gabo = std::shared_ptr<Professor>(new Professor("Gabriel"));
  std::shared_ptr<Professor> fab = std::shared_ptr<Professor>(new Professor("Fabricio"));
  std::shared_ptr<Professor> karina = std::shared_ptr<Professor>(new Professor("Karina Lumbreras"));
  lola->SetCurrentRoom(rest_room);
  gabo->SetCurrentRoom(rest_room);
  fab->SetCurrentRoom(rest_room);
  karina->SetCurrentRoom(rest_room);
  // Students
  std::shared_ptr<Student> daivid = std::shared_ptr<Student>(new Student("David"));
  std::shared_ptr<Student> pulga = std::shared_ptr<Student>(new Student("Pulga"));
  std::shared_ptr<Student> ines = std::shared_ptr<Student>(new Student("Ines"));
  // Put students on the courtyard
  daivid->SetCurrentRoom(courtyard);
  pulga->SetCurrentRoom(courtyard);
  ines->SetCurrentRoom(courtyard);
  // Assign Teachers
  bio->Assign(karina);
  geo->Assign(lola);
  math->Assign(gabo);
  hist->Assign(fab);
  /**
   * Fab was assigned to another course,
   * making the old one deprecated,
   * the current course of Fabricio is this one
   * even though he is also assigned to the old one
   **/
  eng->Assign(fab);

  // Subscribe students to their classes [daivid, pulga & ines]
  bio->Subscribe(daivid);
  bio->Subscribe(daivid); // Subscribe twice, no effect -> warning
  bio->Subscribe(pulga);
  bio->Subscribe(ines);

  geo->Subscribe(daivid);
  geo->Subscribe(pulga); // What?
  geo->Subscribe(ines); // Max limit reached, no effect

  math->Subscribe(pulga);

  eng->Subscribe(daivid);
  eng->Subscribe(ines);

  hist->Subscribe(daivid);

  std::cout << " * * * * * * * * * * * * * * * *" << std::endl;

  std::cout << *bio << std::endl << *geo << std::endl << *math << std::endl << *eng << std::endl << *hist << std::endl;
  std::cout << *lola << std::endl << *gabo << std::endl << *fab << std::endl << *karina << std::endl;
  std::cout << *daivid << std::endl << *pulga << std::endl << *ines << std::endl;
  
  std::cout << " * * * * * * BOUROCRACY * * * * * *" << std::endl;
  std::shared_ptr<SecretarialOffice> so = std::shared_ptr<SecretarialOffice>(new SecretarialOffice());
  std::shared_ptr<Classroom> classroom = std::shared_ptr<Classroom>(new Classroom());
  Secretary juan("Juan");
  std::shared_ptr<Form> form_course_finished = juan.CreateForm(FormType::CourseFinished);
  std::shared_ptr<Form> form_need_more_classroom = juan.CreateForm(FormType::NeedMoreClassRoom);
  std::shared_ptr<Form> form_need_course = juan.CreateForm(FormType::NeedCourseCreation);
  std::shared_ptr<Form> form_subscription = juan.CreateForm(FormType::SubscriptionToCourse);
  juan.SetCurrentRoom(classroom);
  juan.ArchiveForm(form_course_finished); // Error, juan is not in the secretarial Room
  juan.SetCurrentRoom(so);
  juan.SetCurrentRoom(so); // Set same room twice ?
  juan.ArchiveForm(form_course_finished);
  std::cout << *form_course_finished << std::endl;
  std::cout << *form_need_more_classroom << std::endl;
  std::cout << *form_need_course << std::endl;
  std::cout << *form_subscription << std::endl;
  std::cout << *so << std::endl;
  std::cout << juan << std::endl;

  std::cout << " * * * * * * DIRECTION * * * * * *" << std::endl;
  std::shared_ptr<HeadmasterOffice> headmaster_office = std::shared_ptr<HeadmasterOffice>(new HeadmasterOffice());
  std::shared_ptr<Headmaster> conchita = std::shared_ptr<Headmaster>(new Headmaster("Conchita"));
  conchita->SetCurrentRoom(headmaster_office);
  // Put headmaster at headmaster office
  std::cout << *conchita << std::endl;

  // Finish course
  conchita->ReceiveForm(form_course_finished);
  // Classroom request
  CheckSharedNullptr(std::dynamic_pointer_cast<NeedMoreClassRoomForm>(form_need_more_classroom)->GetClassroom());
  conchita->ReceiveForm(form_need_more_classroom);
  CheckSharedNullptr(std::dynamic_pointer_cast<NeedMoreClassRoomForm>(form_need_more_classroom)->GetClassroom());
  // Course request
  CheckSharedNullptr(std::dynamic_pointer_cast<NeedCourseCreationForm>(form_need_course)->GetCourse());
  conchita->ReceiveForm(form_need_course);
  std::dynamic_pointer_cast<NeedCourseCreationForm>(form_need_course)->FillCourse("Linux 101", 100, 10);
  conchita->ReceiveForm(form_need_course);
  CheckSharedNullptr(std::dynamic_pointer_cast<NeedCourseCreationForm>(form_need_course)->GetCourse());
  // Subscribe Ines to math
  conchita->ReceiveForm(form_subscription);
  std::dynamic_pointer_cast<SubscriptionToCourseForm>(form_subscription)->FillCourseAndStudent(math, ines);
  conchita->ReceiveForm(form_subscription);
  std::cout << " * * * * * * * * * * * * * * * *" << std::endl;
  return (0);
}
