#include "observer.hpp"
#include <iostream>

enum class EventType {
    EVENT_ONE,
    EVENT_TWO,
    EVENT_THREE
};

int main() {
    Observer<EventType> observer;

    // Subscribe to EVENT_ONE
    observer.subscribe(EventType::EVENT_ONE, []() {
        std::cout << "Event One triggered" << std::endl;
    });

    // Subscribe first lambda to EVENT_TWO
    observer.subscribe(EventType::EVENT_TWO, []() {
        std::cout << "Event Two triggered (First subscriber)" << std::endl;
    });

    // Subscribe second lambda to EVENT_TWO
    observer.subscribe(EventType::EVENT_TWO, []() {
        std::cout << "Event Two triggered (Second subscriber)" << std::endl;
    });

    // Triggering EVENT_ONE
    std::cout << "Notify EVENT_ONE" << std::endl;
    observer.notify(EventType::EVENT_ONE);  // Output: "Event One triggered"

    // Triggering EVENT_TWO
    std::cout << "Notify EVENT_TWO" << std::endl;
    observer.notify(EventType::EVENT_TWO);  
    // Output: 
    // "Event Two triggered (First subscriber)"
    // "Event Two triggered (Second subscriber)"
    // The order may differ

    // Triggering EVENT_THREE (No subscriber)
    std::cout << "Notify EVENT_THREE" << std::endl;
    observer.notify(EventType::EVENT_THREE);  // Output: None, as there are no subscribers

    return 0;
}
