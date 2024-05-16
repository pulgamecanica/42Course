//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Student.hpp"
#include "Course.hpp"
#include "Professor.hpp"

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


int main(void)
{
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;
    testIndependence();
    return (0);
}
