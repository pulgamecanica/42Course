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


template<typename T>
void ThreadFoo() {
    T* singleton = T::GetInstance();
    std::cout << singleton << std::endl;   
}

template<typename T>
void ThreadBar() {
 T* singleton = T::GetInstance();
    std::cout << singleton << std::endl;   
}

int main(void)
{
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;
    std::cout << YELLOW << "- - - - - - Thread Test CourseList - - - - - - -" << ENDC << std::endl;
    {
    std::thread t1(ThreadFoo<CourseList>);
    std::thread t2(ThreadBar<CourseList>);
    t1.join();
    t2.join();
    }
    std::cout << YELLOW << "- - - - - - Thread Test RoomList - - - - - - -" << ENDC << std::endl;
    {
    std::thread t1(ThreadFoo<RoomList>);
    std::thread t2(ThreadBar<RoomList>);
    t1.join();
    t2.join();
    }
    std::cout << YELLOW << "- - - - - - Thread Test StudentList - - - - - - -" << ENDC << std::endl;
    {
    std::thread t1(ThreadFoo<StudentList>);
    std::thread t2(ThreadBar<StudentList>);
    t1.join();
    t2.join();
    }
    std::cout << YELLOW << "- - - - - - Thread Test StaffList - - - - - - -" << ENDC << std::endl;
    {
    std::thread t1(ThreadFoo<StaffList>);
    std::thread t2(ThreadBar<StaffList>);
    t1.join();
    t2.join();
    }
    std::cout << YELLOW << "- - - - - - Test CourseList - - - - - - -" << ENDC << std::endl;
    CourseList * cl = CourseList::GetInstance();
    Course c("Awesome Programming Course");
    Course ruby("Ruby 101");
    Course python("Python");
    cl->add(&python); // try to add python manually... ?
    cl->remove(&python); // remove manually... ?
    cl->remove(&python); // again? ?
    cl->add(&python); // try to add python manually... ?
    try {
        Course & c = (*cl)[9];
        std::cout << "Item 9 |" << c << "|" << std::endl;
    } catch (std::exception & e) {
        std::cerr << RED << e.what() << ENDC << std::endl;
    }
    std::cout << "CourseList has " << cl->size() << " courses" << std::endl;
    for (size_t i = 0; i < cl->size(); ++i)
        std::cout << " - " << (*cl)[i] <<  std::endl;
    std::cout << YELLOW << "- - - - - - Test StaffList - - - - - - -" << ENDC << std::endl;
    StaffList * stfl = StaffList::GetInstance();
    Staff staff("Megan the handy gal");
    HeadMaster dombuldore("Dombuldore IV");
    Secretary lola("Lola Goth");
    std::cout << "StaffList has " << stfl->size() << " members" << std::endl;
    for (size_t i = 0; i < stfl->size(); ++i)
        std::cout << " - " << (*stfl)[i] <<  std::endl;
    std::cout << YELLOW << "\t - - - Polymorphic Test StaffList - - -" << ENDC << std::endl;
    for (size_t i = 0; i < stfl->size(); ++i) {
        HeadMaster * hm = dynamic_cast<HeadMaster *>(&((*stfl)[i]));
        Professor * pr = dynamic_cast<Professor *>(&((*stfl)[i]));
        Secretary * sc = dynamic_cast<Secretary *>(&((*stfl)[i]));
        if (hm != nullptr)
            std::cout << *hm << std::endl;
        else if (pr != nullptr)
            std::cout << *pr << std::endl;
        else if  (sc != nullptr)
            std::cout << *sc << std::endl;
        else
            std::cout << "Plymorphism Failed :( for he's just a staff member " << (*stfl)[i] << std::endl;
    }
    std::cout << YELLOW << "- - - - - - Test RoomList - - - - - - -" << ENDC << std::endl;
    RoomList * rl = RoomList::GetInstance();
    Room r1;
    Courtyard courtyard;
    SecretarialOffice so1;
    SecretarialOffice so2;
    HeadmasterOffice hmo;
    ClassRoom cr1;
    ClassRoom cr2;
    std::cout << "RoomList has " << rl->size() << " rooms" << std::endl;
    for (size_t i = 0; i < rl->size(); ++i)
        std::cout << " - " << (*rl)[i] <<  std::endl;
    std::cout << YELLOW << "\t - - - Polymorphic Test RoomList - - -" << ENDC << std::endl;
    for (size_t i = 0; i < rl->size(); ++i) {
        Courtyard * cy = dynamic_cast<Courtyard *>(&((*rl)[i]));
        SecretarialOffice * so = dynamic_cast<SecretarialOffice *>(&((*rl)[i]));
        HeadmasterOffice * hmo2 = dynamic_cast<HeadmasterOffice *>(&((*rl)[i]));
        ClassRoom * cr = dynamic_cast<ClassRoom *>(&((*rl)[i]));
        if (cy != nullptr)
            std::cout << *cy << std::endl;
        else if (so != nullptr)
            std::cout << *so << std::endl;
        else if  (hmo2 != nullptr)
            std::cout << *hmo2 << std::endl;
        else if  (cr != nullptr)
            std::cout << *cr << std::endl;
        else
            std::cout << "Plymorphism Failed :( for it's just a room " << (*rl)[i] << std::endl;
    }
    std::cout << YELLOW << "- - - - - - Test StudentList - - - - - - -" << ENDC << std::endl;
    StudentList * stul = StudentList::GetInstance();
    Student hp("Harry Potter");
    Student draco("Draco Malfoy");
    Student hermione("Hermione Granger");
    std::cout << "StudentList has " << stul->size() << " students" << std::endl;
    for (size_t i = 0; i < stul->size(); ++i)
        std::cout << " - " << (*stul)[i] <<  std::endl;
    std::cout << YELLOW << "- - - - - - - - - - - - - - - - - - - - -" << ENDC << std::endl;
    return (0);
}



