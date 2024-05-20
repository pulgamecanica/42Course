//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <thread>

#include "School.inc"
#include "Person.hpp"
#include "Staff.hpp"
#include "Student.hpp"
#include "Professor.hpp"
#include "Secretary.hpp"
#include "HeadMaster.hpp"
#include "Room.hpp"
#include "ClassRoom.hpp"
#include "SecretarialOffice.hpp"
#include "Courtyard.hpp"
#include "StaffRestRoom.hpp"
#include "HeadmasterOffice.hpp"
#include "Course.hpp"
#include "Form.hpp"
#include "CourseFinishedForm.hpp"
#include "NeedCourseCreationForm.hpp"
#include "NeedMoreClassRoomForm.hpp"
#include "SubscriptionToCourseForm.hpp"

int main(void)
{
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;
  
    std::cout << YELLOW << "- - - - - - Init - - - - - - -" << ENDC << std::endl;
    Secretary mike("Mike the tyrant");
    Course c("Awesome Programming Course");
    Course ruby("Ruby 101", 1, 1);
    Course python("Python");
    Student hp("Harry Potter");
    Student draco("Draco Malfoy");
    Student hermione("Hermione Granger");
    Professor px("X");
    Professor pulga("pulga");
    c.assign(&px);
    ruby.assign(&pulga);
    python.assign(&px);
    hp.printStudentProgram();
    draco.printStudentProgram();
    hermione.printStudentProgram();
    
    std::cout << YELLOW << "- - - - - - - - - - - - - - - - - - - - -" << ENDC << std::endl;
    return (0);
}



