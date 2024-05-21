//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __IOBSERVER__
# define __IOBSERVER__

// https://refactoring.guru/design-patterns/observer
// Every "Person" is an observer

// They observe the Event, which is controlled by the Headmaster
enum class Event
{
  RingBell
};


class IObserver {
  public:
    virtual ~IObserver(){
      ;
    };
    virtual void Update(enum Event e) = 0;
};
#endif
