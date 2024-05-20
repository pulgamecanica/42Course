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

#include "ex02.inc"


int main(void)
{
  if (DEBUG)
      std::cout << "Debug ON!" << std::endl;

  std::cout << " * * * * * * INIT * * * * * * * *" << std::endl;
  std::shared_ptr<Secretary> juan = std::shared_ptr<Secretary>(new Secretary("Juan"));
  std::shared_ptr<Headmaster> elsa = std::shared_ptr<Headmaster>(new Headmaster("Elsa"));
  std::shared_ptr<Student> diana = std::shared_ptr<Student>(new Student("Diana"));
  std::shared_ptr<Student> hugo = std::shared_ptr<Student>(new Student("Hugo"));
  std::shared_ptr<Course> bio = std::shared_ptr<Course>(new Course("Biology", 0, 3));
  bio->Subscribe(diana);
  auto finished_course_form = std::dynamic_pointer_cast<CourseFinishedForm>(juan->CreateForm(FormType::CourseFinished));
  auto need_course_form = std::dynamic_pointer_cast<NeedCourseCreationForm>(juan->CreateForm(FormType::NeedCourseCreation));
  auto need_classroom_form = std::dynamic_pointer_cast<NeedMoreClassRoomForm>(juan->CreateForm(FormType::NeedMoreClassRoom));
  auto subscribe_form = std::dynamic_pointer_cast<SubscriptionToCourseForm>(juan->CreateForm(FormType::SubscriptionToCourse));
  std::cout << " * * * * * * * * Testing Subscription To Course * * * * * * * *" << std::endl;
  // Subscribe Hugo to bio
  subscribe_form->Execute();
  subscribe_form->FillCourseAndStudent(bio, hugo);
  subscribe_form->Execute();
  std::cout << *subscribe_form << " Errors: " << finished_course_form->Errors() << std::endl;
  elsa->ReceiveForm(subscribe_form);
  std::cout << " * * * * * * * * Testing Course Finished * * * * * * * *" << std::endl;
  // Try manually, need signature!
  finished_course_form->Execute();
  finished_course_form->FillCourse(bio);
  finished_course_form->Execute();
  std::cout << *finished_course_form << " Errors: " << finished_course_form->Errors() << std::endl;
  std::cout << *bio << " Course Can Finish? " << std::boolalpha << (bio->LessonsLeft() == 0) << std::endl;
  // Now give to headmaster to sign and execute!
  elsa->ReceiveForm(finished_course_form);
  std::cout << " * * * * * * * * Testing Need Course * * * * * * * *" << std::endl;
  need_course_form->Execute();
  need_course_form->FillCourse("Geography", 3, 1);
  need_course_form->Execute();
  std::cout << *need_course_form << " Errors: " << need_course_form->Errors() << std::endl;
  {
  std::shared_ptr<Course> geo = need_course_form->GetCourse();
  if (geo)
    std::cout << *geo << std::endl;
  else
    std::cout << "Cannot get Course" << std::endl;
  }
  elsa->ReceiveForm(need_course_form);
  {
  std::shared_ptr<Course> geo = need_course_form->GetCourse();
  if (geo)
    std::cout << *geo << std::endl;
  else
    std::cout << "Cannot get Course" << std::endl;
  }
  std::cout << " * * * * * * * * Testing Need Classroom * * * * * * * *" << std::endl;
  need_classroom_form->Execute();
  std::cout << *need_classroom_form << " Errors: " << need_classroom_form->Errors() << std::endl;
  {
  std::shared_ptr<Classroom> cr = need_classroom_form->GetClassroom();
  if (cr)
    std::cout << *cr << std::endl;
  else
    std::cout << "Cannot get Classroom" << std::endl;
  }
  elsa->ReceiveForm(need_classroom_form);
  {
  std::shared_ptr<Classroom> cr = need_classroom_form->GetClassroom();
  if (cr)
    std::cout << *cr << std::endl;
  else
    std::cout << "Cannot get Classroom" << std::endl;
  }
  std::cout << " * * * * * * * * * * * * * * * *" << std::endl;
  return (0);
}
