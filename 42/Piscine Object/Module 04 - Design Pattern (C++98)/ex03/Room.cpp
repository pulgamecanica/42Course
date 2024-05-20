//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm>

#include "School.inc"

#include "Room.hpp"
#include "Person.hpp"

long Room::_gID = 0;

Room::Room(): ID(_gID++) {
    if (DEBUG)
        std::cout << YELLOW << "[Room] " << GREEN << "CREATE\t" << ENDC << "Room " << BLUE << ID << ENDC << std::endl;
}

Room::~Room() {
    if (DEBUG)
        std::cout << "Room " << *this << " destroyed" << std::endl;
}

int Room::getID() const {
    return ID;
}

size_t Room::getTotalOccupants() const {
    return occupants_.size();
}

bool Room::hasPerson(Person * p_person) const {
    return std::find(occupants_.begin(), occupants_.end(), p_person) != occupants_.end();
}

bool Room::canEnter(Person* p_person) {
    return !hasPerson(p_person);
};

bool Room::enter(Person* p_person) {
    if (hasPerson(p_person)) {
        if (DEBUG)
            std::cout << YELLOW << "[Room] " << RED << "ENTER FAIL\t" << ENDC << *p_person << " is in the room " << BLUE << ID << ENDC << std::endl;
        return false;
    }
    if (canEnter(p_person)) {
        occupants_.push_back(p_person);
        p_person->setRoom(this);
        if (DEBUG)
            std::cout << YELLOW << "[Room] " << GREEN << "ENTER\t" << ENDC << *p_person << " entered room " << BLUE << ID << ENDC << std::endl;
        return true;
    }
    if (DEBUG)
            std::cout << YELLOW << "[Room] " << RED << "ENTER FAIL\t" << ENDC << *p_person << " is not allowed to enter the room " << BLUE << ID << ENDC << std::endl;
    return false;
};

void Room::exit(Person* p_person) {
    std::vector<Person*>::iterator it = std::find(occupants_.begin(), occupants_.end(), p_person);
    if (it != occupants_.end()) {
        occupants_.erase(it);
        if (DEBUG)
            std::cout << YELLOW << "[Room] " << GREEN << "EXIT\t" << ENDC << *p_person << " exited room " << BLUE << ID << ENDC << std::endl;
        p_person->setRoom(nullptr);
    } else if (DEBUG) {
        std::cout << YELLOW << "[Room] " << RED << "EXIT FAIL\t" << ENDC << *p_person << " is not in the room " << BLUE << ID << ENDC << std::endl;
    }
}
    
void Room::printOccupants() {
    for (std::vector<Person *>::iterator i = occupants_.begin(); i != occupants_.end(); ++i)
        std::cout << " - Person: " << **i << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Room& room) {
    s << BLUE << room.getID() << ENDC << " [" << room.getTotalOccupants() << " occupants]";
    return (s);
}

