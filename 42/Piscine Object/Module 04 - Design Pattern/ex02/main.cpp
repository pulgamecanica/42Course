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
    CourseFinishedForm cff;
    cff.execute();
    NeedCourseCreationForm nccf;
    nccf.fill("Rust for begginers", 9, 42);
    nccf.execute();

    // Thest without head master will fail of course
    testFormByType(mike, FormType::CourseFinished);
    // Try again with the headmaster present
    HeadMaster hm("Conchita");
    std::cout << YELLOW << "- - - - - - Secretary NeedMoreClassRoomForm Form Tests No Head Master - - - - - - -" << ENDC << std::endl;
    testFormByType(mike, FormType::NeedMoreClassRoom);
    std::cout << YELLOW << "- - - - - - Secretary CourseFinished Form Tests No Fill - - - - - - -" << ENDC << std::endl;
    testFormByType(mike, FormType::CourseFinished);
    std::cout << YELLOW << "- - - - - - Secretary NeedCourseCreation Form Tests No Fill - - - - - - -" << ENDC << std::endl;
    testFormByType(mike, FormType::NeedCourseCreation);
    std::cout << YELLOW << "- - - - - - Secretary SubscriptionToCourse Form Tests No Fill - - - - - - -" << ENDC << std::endl;
    testFormByType(mike, FormType::SubscriptionToCourse);
    std::cout << YELLOW << "- - - - - - Secretary Archive Tests  - - - - - - -" << ENDC << std::endl;
    mike.archiveForms();
    // hermione is responsible, she will attend 11 times her classes
    ClassRoom * class_room1 = dynamic_cast<ClassRoom *>(rl->get<ClassRoom *>());
    if (class_room1 != nullptr) {
        // Assign hermione's course to the class room so it will work!
        class_room1->assignCourse(&c);
        std::cout << "Founded a class room" << std::endl;
        for (int i = 0; i < 9; ++i)
            hermione.attendClass(class_room1);
    }
    std::cout << YELLOW << "- - - - - - Secretary CourseFinished Test  - - - - - - -" << ENDC << std::endl;
    CourseFinishedForm * cff2 = dynamic_cast<CourseFinishedForm *>(mike.createForm(FormType::CourseFinished));
    cff2->fill(&c);
    hm.receiveForm(cff2);
    cff2->execute();
    hm.sign(cff2);
    cff2->execute(); // This form is reusable
    std::cout << YELLOW << "- - - - - - Secretary NeedCourseCreation Test  - - - - - - -" << ENDC << std::endl;
    NeedCourseCreationForm * nccf2 = dynamic_cast<NeedCourseCreationForm *>(mike.createForm(FormType::NeedCourseCreation));
    nccf2->fill("Rust Advanced Topics", 42, 42);
    hm.receiveForm(nccf2);
    nccf2->execute();
    hm.sign(nccf2);
    nccf2->execute();
    nccf2->execute(); // Execute twice, attempt to create two courses with the same form
    std::cout << YELLOW << "- - - - - - Secretary SubscriptionToCourse Test  - - - - - - -" << ENDC << std::endl;
    SubscriptionToCourseForm * stcf2 = dynamic_cast<SubscriptionToCourseForm *>(mike.createForm(FormType::SubscriptionToCourse));
    stcf2->fill(&python, &hermione);
    hm.receiveForm(stcf2);
    stcf2->execute();
    hm.sign(stcf2);
    stcf2->execute();
    stcf2->execute(); // Execute twice, attempt to create subscribe twice?
    std::cout << YELLOW << "- - - - - - - - - - - - - - - - - - - - -" << ENDC << std::endl;
    return (0);
}



