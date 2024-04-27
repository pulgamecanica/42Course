//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COVARIANTOBJ_HPP__
# define __COVARIANTOBJ_HPP__

#include <iostream>

template<class T>
class CovariantObj {
    public:
        CovariantObj(const std::string & name): name_(name) {
            std::cout << "Covariant " << name_ << " created" << std::endl;
        }
        // same logic as the CovariantObj<T> copy constructor
        template<class U>
        CovariantObj(const CovariantObj<U> &blah) {
            name_ = blah.getName();
            std::cout << "Copy Covariant " << name_ << " created" << std::endl;
        }

        template<class U>
        CovariantObj<T>(const CovariantObj<U > *blah) {
            // name_ = blah.getName();
            std::cout << "Copy Covariant<T> " << name_ << " created" << std::endl;
        }

        virtual ~CovariantObj() {
            std::cout << "Covariant " << name_ << " destroyed" << std::endl;
        }

        std::string getName() const {
            return name_;
        }
    private:
        std::string name_;
};
#endif
