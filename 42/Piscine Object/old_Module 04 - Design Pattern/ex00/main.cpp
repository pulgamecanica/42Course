//***************************//
//*Template by pulgamecanica*//
//***************************//

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
    std::cout << YELLOW << "- - - - - - Person - - - - - - -" << ENDC << std::endl;
    Person stranger("Some Stranger");
    std::string name = stranger.getName();
    static_cast<std::string>(stranger.getName())[2] = 'B';
    std::cout << stranger << std::endl;
    std::cout << YELLOW << "- - - - - - Staff - - - - - - -" << ENDC << std::endl;
    Staff jnt("Janitor");
    std::cout << jnt << std::endl;
    std::cout << YELLOW << "- - - - - - Student - - - - - - -" << ENDC << std::endl;
    Student pp("Peter Parker");
    Student mj("Marry Jane");
    std::cout << pp << std::endl;
    std::cout << mj << std::endl;
    std::cout << YELLOW << "- - - - - - Professor - - - - - - -" << ENDC << std::endl;
    Professor px("Professor X");
    Professor ptk("Prof. J.R.R. Tolkien");
    std::cout << px << std::endl;
    std::cout << ptk << std::endl;
    std::cout << YELLOW << "- - - - - - Secretary - - - - - - -" << ENDC << std::endl;
    Secretary sct("Mike the big folk");
    std::cout << sct << std::endl;
    std::cout << YELLOW << "- - - - - - HeadMaster - - - - - - -" << ENDC << std::endl;
    HeadMaster dd("Dombuldore");
    std::cout << dd << std::endl;
    std::cout << YELLOW << "- - - - - - Room - - - - - - -" << ENDC << std::endl;
    Room r1;
    Room r2;
    Room r3;
    std::cout << r1 << std::endl;
    std::cout << r2 << std::endl;
    std::cout << r3 << std::endl;
    std::cout << YELLOW << "- - - - - - ClassRoom - - - - - - -" << ENDC << std::endl;
    ClassRoom cr1;
    ClassRoom cr2;
    std::cout << cr1 << std::endl;
    std::cout << cr2 << std::endl;
    std::cout << YELLOW << "- - - - - - SecretarialOffice - - - - - - -" << ENDC << std::endl;
    SecretarialOffice so;
    std::cout << so << std::endl;
    std::cout << YELLOW << "- - - - - - Courtyard - - - - - - -" << ENDC << std::endl;
    Courtyard c;
    std::cout << c << std::endl;
    std::cout << YELLOW << "- - - - - - StaffRestRoom - - - - - - -" << ENDC << std::endl;
    StaffRestRoom srr;
    std::cout << srr << std::endl;
    std::cout << YELLOW << "- - - - - - HeadmasterOffice - - - - - - -" << ENDC << std::endl;
    HeadmasterOffice hmo;
    std::cout << hmo << std::endl;
    std::cout << YELLOW << "- - - - - - Course - - - - - - -" << ENDC << std::endl;
    Course cs("Computer Science 101");
    Course la("Linear Algebra 102", 22);
    Course alg("Algorithms", 9, 9);
    std::cout << cs << std::endl;
    std::cout << la << std::endl;
    std::cout << alg << std::endl;
    std::cout << YELLOW << "- - - - - - " << BLUE << "Form Is Abstract" << YELLOW << " - - - - - - -" << ENDC << std::endl;
    // Form f1(FormType::CourseFinished);
    // Form f2(FormType::NeedMoreClassRoom);
    // Form f3(FormType::NeedCourseCreation);
    // Form f4(FormType::SubscriptionToCourse);
    std::cout << YELLOW << "- - - - - - CourseFinishedForm - - - - - - -" << ENDC << std::endl;
    CourseFinishedForm cff;
    std::cout << cff << std::endl;
    std::cout << YELLOW << "- - - - - - NeedCourseCreationForm - - - - - - -" << ENDC << std::endl;
    NeedCourseCreationForm nccf;
    std::cout << nccf << std::endl;
    std::cout << YELLOW << "- - - - - - NeedMoreClassRoomForm - - - - - - -" << ENDC << std::endl;
    NeedMoreClassRoomForm nmcrf;
    std::cout << nmcrf << std::endl;
    std::cout << YELLOW << "- - - - - - SubscriptionToCourseForm - - - - - - -" << ENDC << std::endl;
    SubscriptionToCourseForm stcf;
    std::cout << stcf << std::endl;
    std::cout << YELLOW << "- - - - - - Course tests - - - - - - -" << ENDC << std::endl;
    cs.assign(&px);
    la.assign(&ptk);
    alg.assign(&ptk);
    std::cout << cs << " has " << pp << " ? " << std::boolalpha <<  cs.hasStudent(&pp) << std::endl;
    std::cout << la << " has " << mj << " ? " << std::boolalpha <<  cs.hasStudent(&mj) << std::endl;
    std::cout << YELLOW << "\t- - - Subscribe Students - - -" << ENDC << std::endl;
    cs.subscribe(&pp);
    cs.subscribe(&pp);
    cs.subscribe(&mj);
    la.subscribe(&pp);
    alg.subscribe(&mj);
    std::cout << cs << " has " << pp << " ? " << std::boolalpha <<  cs.hasStudent(&pp) << std::endl;
    std::cout << cs << " has " << mj << " ? " << std::boolalpha <<  cs.hasStudent(&mj) << std::endl;
    std::cout << la << " has " << pp << " ? " << std::boolalpha <<  la.hasStudent(&pp) << std::endl;
    std::cout << la << " has " << mj << " ? " << std::boolalpha <<  la.hasStudent(&mj) << std::endl;
    std::cout << alg << " has " << pp << " ? " << std::boolalpha <<  alg.hasStudent(&pp) << std::endl;
    std::cout << alg << " has " << mj << " ? " << std::boolalpha <<  alg.hasStudent(&mj) << std::endl;
    std::cout << YELLOW << "- - - - - - Room Tests - - - - - - -" << ENDC << std::endl;
    r1.enter(&pp);
    r1.enter(&stranger);
    r2.enter(&ptk);
    r2.enter(&px);
    r3.enter(&mj);
    r3.enter(&jnt);
    r3.enter(&jnt); // Janitor tries to enter twice, make sure he's not repeated
    // Try to change the janitor from room illegally!
    // Room * rmptr1 = dynamic_cast<Room *>(jnt.getRoom());
    std::cout << r1 << std::endl;
    r1.printOccupants();
    std::cout << r2 << std::endl;
    r2.printOccupants();
    std::cout << r3 << std::endl;
    r3.printOccupants();
    std::cout << YELLOW << "\t- - - Exit Room - - -" << ENDC << std::endl;
    r1.exit(&pp);
    r1.exit(&pp); // Twice!? Deal with it!
    r1.exit(&ptk); // Professor Tolkien tries to exit room 1 where he is not. Deal with it!
    r1.exit(&stranger);
    r2.exit(&ptk);
    r2.exit(&px);
    r3.exit(&mj);
    r3.exit(&jnt);
    std::cout << r1 << std::endl;
    r1.printOccupants();
    std::cout << r2 << std::endl;
    r2.printOccupants();
    std::cout << r3 << std::endl;
    r3.printOccupants();
    std::cout << YELLOW << "- - - - - - ClassRoom Tests - - - - - - -" << ENDC << std::endl;
    std::cout << YELLOW << "\tOnly Staff & Students can enter a ClassRoom" << ENDC << std::endl;
    cr1.enter(&stranger);
    cr1.enter(&jnt);
    cr2.enter(&pp);
    cr2.enter(&mj);
    cr2.enter(&px);
    cr2.enter(&jnt);
    cr1.enter(&pp);
    std::cout << cr1 << std::endl;
    cr1.printOccupants();
    std::cout << cr2 << std::endl;
    cr2.printOccupants();
    std::cout << YELLOW << "- - - - - - SecretarialOffice Tests - - - - - - -" << ENDC << std::endl;
    std::cout << YELLOW << "\tOnly Secretary And HeadMaster can enter a SecretarialOffice" << ENDC << std::endl;
    so.enter(&stranger);
    so.enter(&pp);
    so.enter(&px);
    so.enter(&sct);
    so.enter(&dd);
    std::cout << so << std::endl;
    so.printOccupants();
    std::cout << YELLOW << "- - - - - - Courtyard Tests - - - - - - -" << ENDC << std::endl;
    std::cout << YELLOW << "\tEveryone can enter the Courtyard" << ENDC << std::endl;
    // Courtyard c;
    c.enter(&stranger);
    c.enter(&dd);
    std::cout << c << std::endl;
    c.printOccupants();
    std::cout << YELLOW << "- - - - - - StaffRestRoom Tests - - - - - - -" << ENDC << std::endl;
    std::cout << YELLOW << "\tOnly Staff can enter the StaffRestRoom" << ENDC << std::endl;
    // StaffRestRoom srr;
    srr.enter(&stranger);
    srr.enter(&pp);
    srr.enter(&mj);
    srr.enter(&px);
    srr.enter(&sct);
    srr.enter(&dd);
    std::cout << srr << std::endl;
    srr.printOccupants();
    std::cout << YELLOW << "- - - - - - HeadmasterOffice Tests - - - - - - -" << ENDC << std::endl;
    std::cout << YELLOW << "\tOnly Headmaster can enter the HeadmasterOffice" << ENDC << std::endl;
    hmo.enter(&stranger);
    hmo.enter(&pp);
    hmo.enter(&mj);
    hmo.enter(&px);
    hmo.enter(&sct);
    hmo.enter(&dd);
    std::cout << hmo << std::endl;
    hmo.printOccupants();
    std::cout << YELLOW << "- - - - - - - - - - - - - - - - - - - - -" << ENDC << std::endl;
    return (0);
}
