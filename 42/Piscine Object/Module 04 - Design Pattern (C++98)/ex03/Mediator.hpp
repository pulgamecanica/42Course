//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __MEDIATOR_HPP__
# define __MEDIATOR_HPP__

#include <iostream>

class Person;
class Mediator {
    public:
        virtual void notify(Person *sender, std::string event) const = 0;
};
#endif
