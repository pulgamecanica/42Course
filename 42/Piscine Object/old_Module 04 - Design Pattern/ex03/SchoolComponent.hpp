//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SCHOOLCOMPONENT_HPP__
# define __SCHOOLCOMPONENT_HPP__

#include <iostream>

class Mediator;
class SchoolComponent {
    public:
        SchoolComponent(Mediator * mediator_ = nullptr);
        virtual ~SchoolComponent() = 0;
        void set_mediator(Mediator *mediator);
    protected:
        Mediator * mediator_;
};
#endif
