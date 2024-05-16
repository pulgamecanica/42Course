//***************************//
//*Template by pulgamecanica*//
//***************************//
#include <algorithm>

#include "ex00.inc"
#include "Person.hpp"
#include "Room.hpp"

namespace SpringfieldElementary {

long long Room::nextID = 0;

Room::Room(): ID(nextID++) {
    if (DEBUG)
        std::cout << GREEN << "[NEW]" << YELLOW << "[Room]\t" << ENDC << ID << std::endl;
}

Room::~Room() {
    for (const auto& weak_ptr : occupants_)
        if (!weak_ptr.expired())
            weak_ptr.lock()->setRoom(nullptr);
    occupants_.clear();
    if (DEBUG)
        std::cout << RED << "[DESTROY]" << YELLOW << "[Room]\t" << ENDC << ID << std::endl;
}

long long Room::getID() const {
    return ID;
}

size_t Room::getTotalOccupants() const {
    size_t count = 0;
    for (const auto& weak_ptr : occupants_) {
        if (!weak_ptr.expired()) {
            ++count;
        }
    }
    return count;
}

void Room::printOccupants() const {
    std::cout << *this << " (" << getTotalOccupants() << ")Occupants" << std::endl;
    for (const auto& weak_ptr : occupants_) {
        if (!weak_ptr.expired()) {
            std::cout << " - " << *(weak_ptr.lock()) << std::endl;
        }
    }
}

bool Room::canEnter(std::shared_ptr<Person> p_person)  {
    (void)p_person;
    return true; // Anyone can enter a generic room
}

void Room::enter(std::shared_ptr<Person> p_person) {
    if (hasPerson(p_person)) {
        if (DEBUG)
            std::cout << RED << "[ENTER FAIL]" << YELLOW << "[Room]\t" << ENDC << *p_person << " is inside the room" << std::endl;
    } else {
        occupants_.emplace_back(p_person);
        std::cout << GREEN << "[ENTER]" << YELLOW << "[Room]\t" << ENDC << *p_person << std::endl;
    }
}

void Room::exit(std::shared_ptr<Person> p_person) {
    auto it = std::find_if(occupants_.begin(), occupants_.end(),
        [p_person](const std::weak_ptr<Person>& weak_ptr) {
            return !weak_ptr.expired() && weak_ptr.lock() == p_person;
        });
    if (it != occupants_.end()) {
        occupants_.erase(it);
        std::cout << GREEN << "[EXIT]" << YELLOW << "[Room]\t" << ENDC << *p_person << std::endl;
    } else {
        if (DEBUG)
            std::cout << RED << "[EXIT FAIL]" << YELLOW << "[Room]\t" << ENDC << *p_person << " is not in the room" << std::endl;
    }
}

void Room::cleanExpiredOccupants() {
    occupants_.erase(
        std::remove_if(occupants_.begin(), occupants_.end(), [](const std::weak_ptr<Person>& weak_ptr) {
            return weak_ptr.expired();
        }), occupants_.end());
}

bool Room::hasPerson(std::shared_ptr<Person> p_person) const {
    return std::any_of(occupants_.begin(), occupants_.end(),
        [p_person](const std::weak_ptr<Person>& weak_ptr) {
            return !weak_ptr.expired() && weak_ptr.lock() == p_person;
        });
}

std::ostream& operator<<(std::ostream& s, const Room& r) {
    s << "Room [" << r.getID() << "]";
    return s;
}

}
