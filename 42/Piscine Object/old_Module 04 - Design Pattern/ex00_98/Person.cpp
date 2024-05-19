//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"
#include "Person.hpp"
#include "Room.hpp"

namespace SpringfieldElementary {

Person::Person(const std::string& p_name) : name_(p_name) {
    if (DEBUG)
        std::cout << GREEN << "[NEW]" << YELLOW << "[Person]\t" << ENDC << name_ << std::endl;
}

Person::~Person() {
    if (current_room_)
        current_room_->exit(shared_from_this());
    if (DEBUG)
        std::cout << RED << "[DESTROY]" << YELLOW << "[Person]\t" << ENDC << name_ << std::endl;
}

const std::string& Person::getName() const {
    return name_;
}

std::shared_ptr<Room> Person::getRoom() const {
    return current_room_;
}

void Person::setRoom(std::shared_ptr<Room> room) {
    // If the person is already in a room, exit that room first
    if (current_room_) {
        current_room_->exit(shared_from_this());
    }
    current_room_ = room;
    if (current_room_) {
        current_room_->enter(shared_from_this());
    }
}

std::ostream& operator<<(std::ostream &s, const Person &p) {
    s << p.getName();
    if (p.getRoom() != nullptr)
        s << " is at " << "room.getName()";
    return s;
}
}
