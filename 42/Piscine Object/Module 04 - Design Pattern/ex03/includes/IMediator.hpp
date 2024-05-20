//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __MEDIATOR_HPP__
# define __MEDIATOR_HPP__

#include <iostream>
#include <memory>

// https://refactoring.guru/design-patterns/mediator/cpp/example
// Headmaster is the concrete Mediator

// Enum to controll exactly what can be notified
// Allows controll over the application but there is no room for easy flexibility
// Exchange security over flexibility ...? I like it, I can afford it.
enum Notification {
  ProfessorRequestGraduation,
  StudentRequestToJoinCourse,
  ProfessorRequestNewCourse,
  ProfessorRequestNewClassroom,
};

class Person; // Person is the base component

class IMediator {
  public:
  virtual void Notify(std::shared_ptr<Person> sender, enum Notification notification) const = 0;

  private:
};
#endif
