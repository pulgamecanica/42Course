//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ROOM_HPP__
# define __ROOM_HPP__

#include <iostream>
#include <vector>
#include <memory>

namespace SpringfieldElementary {

class Person;

class Room {
public:
    Room();
    virtual ~Room();
    long long getID() const;
    size_t getTotalOccupants() const;
    void printOccupants() const;
    virtual bool canEnter(std::shared_ptr<Person> p_person);
    void enter(std::shared_ptr<Person> person);
    void exit(std::shared_ptr<Person> person);
    void cleanExpiredOccupants();
private:
    bool hasPerson(std::shared_ptr<Person> person) const;

    static long long nextID;
    const long long ID;
    std::vector<std::weak_ptr<Person>> occupants_;
};
std::ostream& operator<<(std::ostream&, const Room&);
}
#endif
