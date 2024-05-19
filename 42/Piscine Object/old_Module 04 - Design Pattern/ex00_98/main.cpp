//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"

#include "Person.hpp"
#include "Room.hpp"
#include "Professor.hpp"
#include "Student.hpp"
#include "Course.hpp"

using namespace SpringfieldElementary;


void personTest() {
    Person p1("Mr. Burns");
    std::cout << p1 << std::endl;
    Person p2("Sideshow Bob");
    std::cout << p2 << std::endl;
}

void roomTest() {
    Room r1;
    std::cout << r1 << std::endl;
    Room r2;
    std::cout << r2 << std::endl;
}

void roomPersonTest() {
    std::shared_ptr<Room> r1 = std::shared_ptr<Room>(new Room());
    {
    std::shared_ptr<Person> p1 = std::shared_ptr<Person>(new Person("Mr. Burns"));
    r1->enter(p1);
    // r1->exit(p1); // Enable or dissable to see how destruction works, after all, it's a ~SMART~ pointer 
    }
    r1->printOccupants();
    {
    std::shared_ptr<Person> p1 = std::shared_ptr<Person>(new Person("Ned Flanders"));
    r1->enter(p1);
    r1->printOccupants();
    r1->exit(p1); // Enable or dissable to see how destruction works, after all, it's a ~SMART~ pointer 
    }
    r1->printOccupants();
    std::cout << *r1 << std::endl;
}

void personRoomTest() {
    std::shared_ptr<Person> p1 = std::shared_ptr<Person>(new Person("Homer"));
    {
    std::shared_ptr<Room> r1 = std::shared_ptr<Room>(new Room());
    r1->enter(p1);
    r1->printOccupants();
    // r1->exit(p1); // Enable or dissable to see how destruction works, after all, it's a ~SMART~ pointer 
    }
    std::cout << *p1 << std::endl;
}

void professorTest() {
    Professor p1("Nelson Muntz");
    std::cout << p1 << std::endl;
    Professor p2("Seymour Skinner");
    std::cout << p2 << std::endl;
}

void studentTest() {
    Student s1("Bart Simpson");
    std::cout << s1 << std::endl;
    Student s2("Lisa Simpson");
    std::cout << s2 << std::endl;
}

void courseTests() {
    Course cm("Mathematics", 9, 9);
    std::cout << cm << std::endl;
    Course cb("Biology", 29, 3);
    std::cout << cb << std::endl;
}

void courseStudentTest() {
    auto skinner = std::shared_ptr<Professor>(new Professor("Seymour Skinner"));
    auto eng = std::shared_ptr<Course>(new Course("English", 4, 6));
    eng->assign(skinner);
    eng->assign(skinner);
    std::cout << *eng << std::endl;
    auto  bart = std::shared_ptr<Student>(new Student("Bart Simpson"));
    auto  lisa = std::shared_ptr<Student>(new Student("Lisa Simpson"));
    std::cout << *bart << std::endl;
    std::cout << *lisa << std::endl;

    lisa->addCourse(eng);
    // eng->subscribe(bart);
    std::cout << *eng << std::endl;
}

// void studentCourseTest() {
// }

int main(void)
{
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;
    std::cout << " - - - " << YELLOW << "personTest()" << ENDC << " - - -" << std::endl;
    personTest();
    std::cout << " - - - " << YELLOW << "roomTest()" << ENDC << " - - -" << std::endl;
    roomTest();
    std::cout << " - - - " << YELLOW << "roomPersonTest()" << ENDC << " - - -" << std::endl;
    roomPersonTest();
    std::cout << " - - - " << YELLOW << "personRoomTest()" << ENDC << " - - -" << std::endl;
    personRoomTest();
    std::cout << " - - - " << YELLOW << "professorTest()" << ENDC << " - - -" << std::endl;
    professorTest();
    std::cout << " - - - " << YELLOW << "studentTest()" << ENDC << " - - -" << std::endl;
    studentTest();
    std::cout << " - - - " << YELLOW << "courseTests()" << ENDC << " - - -" << std::endl;
    courseTests();
    std::cout << " - - - " << YELLOW << "courseStudentTest()" << ENDC << " - - -" << std::endl;
    courseStudentTest();
    std::cout << " - - - - - - - - - - - - - -" << std::endl;
    return (0);
}
