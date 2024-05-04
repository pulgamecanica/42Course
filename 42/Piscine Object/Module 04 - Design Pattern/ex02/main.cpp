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
#include "CourseList.hpp"
#include "RoomList.hpp"
#include "StudentList.hpp"
#include "StaffList.hpp"
#include "Singleton.hpp"


void testFormByType(Secretary & mike, enum FormType type) {
    HeadMaster * hm = dynamic_cast<HeadMaster *>(StaffList::GetInstance()->get<HeadMaster *>());
    if (hm == nullptr) {
        if (DEBUG)
            std::cout << RED << "[ERROR] " << ENDC << " HeadMaster is missing" << std::endl;
        return ;
    }
    Form * f = mike.createForm(type);
    hm->receiveForm(f);
    hm->sign(f);
    if (f != nullptr)  {
        f->execute();
    } else {
        std::cerr << RED << "[ERROR]" << ENDC << " form failed to be created" << std::endl;
        return ;
    }
    // delete f; // No need to delete, forms are created by Secretary but belong to the Headmaster, headmaster will delete them on destruction
}

int main(void)
{
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;
    CourseList * cl = CourseList::GetInstance();
    StaffList * stfl = StaffList::GetInstance();
    RoomList * rl = RoomList::GetInstance();
    StudentList * stul = StudentList::GetInstance();
    (void)cl;
    (void)stfl;
    (void)rl;
    (void)stul;
    
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

    std::cout << YELLOW << "- - - - - - Manually add courses - - - - - - -" << ENDC << std::endl;
    hp.addCourse(&c);
    hp.addCourse(&python);
    draco.addCourse(&python);
    hermione.addCourse(&ruby);

    hp.printStudentProgram();
    draco.printStudentProgram();
    hermione.printStudentProgram();
    std::cout << YELLOW << "- - - - - - Subscribed to courses - - - - - - -" << ENDC << std::endl;
    c.subscribe(&draco);
    c.subscribe(&hermione);
    ruby.subscribe(&draco);
    python.subscribe(&hp);
    python.subscribe(&draco);

    hp.printStudentProgram();
    draco.printStudentProgram();
    hermione.printStudentProgram();
    std::cout << YELLOW << "- - - - - - Forms - - - - - - -" << ENDC << std::endl;
    CourseFinishedForm cff(&python);
    cff.execute();
    NeedCourseCreationForm nccf("Rust for begginers", 9, 42);
    nccf.execute();

    // Thest without head master will fail of course
    testFormByType(mike, FormType::CourseFinished);
    // Try again with the headmaster present
    HeadMaster hm("Conchita");
    std::cout << YELLOW << "- - - - - - Secretary NeedMoreClassRoomForm Form Tests - - - - - - -" << ENDC << std::endl;
    testFormByType(mike, FormType::NeedMoreClassRoom);
    // hermione is responsible, she will attend 11 times her classes
    ClassRoom * class_room1 = dynamic_cast<ClassRoom *>(rl->get<ClassRoom *>());
    if (class_room1 != nullptr) {
        // Assign hermione's course to the class room so it will work!
        class_room1->assignCourse(&c);
        std::cout << "Founded a class room" << std::endl;
        for (int i = 0; i < 9; ++i)
            hermione.attendClass(class_room1);
    }
    std::cout << YELLOW << "- - - - - - Secretary CourseFinished Form Tests - - - - - - -" << ENDC << std::endl;
    testFormByType(mike, FormType::CourseFinished);
    std::cout << YELLOW << "- - - - - - Secretary NeedCourseCreation Form Tests - - - - - - -" << ENDC << std::endl;
    testFormByType(mike, FormType::NeedCourseCreation);
    std::cout << YELLOW << "- - - - - - Secretary SubscriptionToCourse Form Tests - - - - - - -" << ENDC << std::endl;
    testFormByType(mike, FormType::SubscriptionToCourse);
    std::cout << YELLOW << "- - - - - - - - - - - - - - - - - - - - -" << ENDC << std::endl;
    return (0);
}



