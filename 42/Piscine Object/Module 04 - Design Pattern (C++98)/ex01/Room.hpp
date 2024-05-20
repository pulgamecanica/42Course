//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ROOM_HPP__
# define __ROOM_HPP__

#include <iostream>
#include <vector>

class Person;

class Room {
    public:
        static long _gID;
        Room();
        virtual ~Room();
        virtual bool canEnter(Person* p_person);
        int getID() const;
        size_t getTotalOccupants() const;
        bool enter(Person* p_person);
        void exit(Person*);
        void printOccupants();
        bool hasPerson(Person * p_person) const;
    private:
        const long ID;
        std::vector<Person *> occupants_;
};
std::ostream& operator<<(std::ostream&, const Room&);
#endif
