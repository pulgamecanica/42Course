# SOLID - SMART

***

## Single Responsability [S]

This Principle says that each class should have only responsability

In the context of the exercise we are told that a car can do the following actions:

* start(): starts the engine
* stop(): stops the engine and applies the brakes
* accelerate(speed): increases the speed of the car by a specified value
* shift_gears_up(): shifts up to the next gear
* shift_gears_down(): shifts down to the previous gear
* reverse(): puts the transmission in reverse gear
* turn_wheel(angle): turns the wheels by a specified angle
* straighten_wheels(): returns the wheels to a straight-ahead position
* apply_force_on_brakes(force): applies a specified force to the brakes
* apply_emergency_brakes(): applies the brakes with maximum force for an emergency stop

Given this example, I imediately identify that the only responsability of the car was to enforce the car actions.<br>
Nothing more, tha car is only the concept, the car is the enforcer, but he will do nothing more.<br>
The respponsability of the car is only to serve as a concept to execute the actions (link the car components)<br>

***

So I also identify which actions are compatible:

start & stop (both affect the Motor)
accelerate, apply_force_on_brakes & apply_emergency_brakes (both have to do with the speed of the car, although also with the brakes)
shift_gears_up, shift_gears_down, reverse (affect the transmission gear)
turn_wheel & straighten_wheels (affect the wheels)

The only "tricky" case in this example is the accelerate, apply_force_on_brakes & apply_emergency_brakes, because they have two responsabilities<br>
Change the car speed and actionate the brakes (if you concider that applying force on the brakes is an action)<br>
For this you can divide in a third object to handle the speed, and other objects which will use this object to modify the speed<br>
Accelerator will simply increase the speed<br>
While the brakes will decrease the speed according to the force<br>

***

## Open/Closed [O]

This principle basically says that the code should be open for extension and closed for modification.

In other words, if you want to add functionalities you should be able to do it without changing the current code.

In the case of the example, the Command class should be designed so that you can add Command derivates without having to change the original Command.

This is possible by using for example the protected properly and virtual, so that new Classes will automatically have access to the Base class and can implement method calls over the Base.

***

## Liskov substitution [L]

The Liskov Substitution Principle (LSP) is a fundamental concept in object-oriented programming (OOP) that states that objects of a superclass should be replaceable with objects of its subclasses without affecting the correctness of the program. In simpler terms, if S is a subtype of T, then objects of type T may be replaced with objects of type S without altering any of the desirable properties of the program.

Let's consider the example from the subject: a superclass called Shape and subclasses Rectangle, Circle, and Triangle.


Now, let's examine how these classes adhere to the Liskov Substitution Principle:

    Substitutability: Any method that expects a Shape should be able to work with any subclass of Shape without knowing the difference.

    Preservation of behavior: If we replace an instance of Shape with an instance of its subclass (Rectangle, Circle, or Triangle), the behavior should remain consistent.

***

## Interface segregation [I]

The Interface Segregation Principle (ISP) in object-oriented design states that a class should not be forced to implement interfaces it doesn't use.<br>
Instead of one large interface, clients should be able to depend only on the methods they use.<br>
Essentially, it promotes the idea of smaller, more focused interfaces.

In this case, the interface of Employee is making sure only the essential methods are implemented by the implementing classes.

For example, calculateSalary must be implemented by EVERYONE because it is essencial and each employee has a different salary.
Now executeWorkday might be similar for some users, which I identified.
There is a generic executeWorkday which just counts the day as worked. But if an employee should register the total amount of hours and not the whole day he can implement this method (because it's virtual) and change it's functionality, but only if needed, respecting the Interface segregation.
Others might just count a day as worked not counting hours, like contractEmployees.

***

## Dependency Inversion [D]

Dependency Inversion Principle (DIP) is one of the five SOLID principles of object-oriented programming. It states that high-level modules should not depend on low-level modules; both should depend on abstractions. Additionally, abstractions should not depend on details; rather, details should depend on abstractions.

In simpler terms, DIP encourages decoupling between modules by introducing an abstraction layer. This allows for more flexibility and easier maintenance of the codebase because changes in one module do not necessarily affect other modules directly.

Instead of implementing the date method inside the classes, I decided to implement another interface an IHeader so that the Loggers can choose to have a header, but they don't need to know how to use the header, or what the header should do, they just use the interface, and the .header() to get the header!

***