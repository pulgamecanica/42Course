//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __PERSON_HPP__
# define __PERSON_HPP__

#include <iostream>

class Room;
class Person {
    public:
        Person(const std::string & name);
        virtual ~Person();
        const std::string & getName() const;
        void setRoom(Room *p_room);
        const Room * getRoom() const;
    private:
        const std::string name_;
        Room* current_room_;
};
std::ostream& operator<<(std::ostream&, const Person&);
#endif
