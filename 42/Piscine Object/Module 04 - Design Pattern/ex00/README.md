# ex00 - In Praise of Design

This first exercise is extreamely important so that the project's base is strong and flexible.
Reliable and reusable. I tried to follow the SOLID principles in order to have a good design.
I redid this exercise and all the classes at least 10 times. I tried using pointers, but I didn't like
the dangling pointers issue. I tries implementing reference wrappers but the code was very sturdy and 
not so easy to understand, mixing references with pointers and instances.
I decided to imlpement smart pointers and converted everything to smart pointers. But then I realized that
smart pointers DO represent ownership and I had to adapt and analyse when to use weak pointers & shared pointers.

---

## Design Pattern

---

### Class Room

The class Room is representing a room in the school

It has an id which is generated autimatically, by using a static int reference which increments each time a room is created.
It has an aggregation of Persons which indicates the occupants of the room.
The room can exist without having any persons.
A room's destruction does not implicate the persons should be deleted. They are both independant.
A person might enter a room or exit if they want to.


---

### Class Classroom

Is a class representing a Room with a concrete implementation of Classroom

The classroom is a room where classes can happen, in order for a class to occurr, the teacher must 

---

### Class Course

The class Course represents a course in the school
It has a name, a number of total lessons and the capacity.
The Course had an agregation of Students and an association with a Professor.
The class Course is responsible monitoring the course's students.
Students should subscribe or unsubscribe to a course through the Course class.

Course class follows the Single responsability principle.
It has only one responsability, which is to subscribe and unsubscribe students from a course.

A course might exist without students or professor.
A student and professor might exist without a course.
The destruction of the members mentioned before does not imply the termination of the others.

> Error handing:
- \[Warning\] Cannot subscribe same student multiple times, will output an error
- \[Warning\] Unsubscribe student which is not subscribed will output an error
- \[Warning\] Assigning same professor multiple times will output an error
- \[Prevention\] Using smart pointers to avoid dangling pointers (weak ptr)
- The Course class should still work even if an object is deleted without warning.

---

### Class Person

The class Person represents a generic Person in the system
It has a name.
It also had an association with a room, a person might have a current_room which can change with the methods `room.enter(person)` and `room.exit(person)`
The current_room is totally indipendent of the person and vice-versa.

> Error handing:
- \[Warning\] Cannot enter the same room where you are, will output an error
- \[Warning\] Cannot exit a room where you are not, will output an error
- \[Prevention\] Pointer dangling is avaouded by using smart pointer for the current_room (weak_ptr)

### Class Student

The Student class represents a person with a concrete type student
It has an aggregation of Courses

The student's courses are just merely for metadata and keeping a reference to the courses directly through the student.
The student course's are not described by this aggregation but rather are just a list of references of the subscription the student has.
A student is only subscribed to a course through the Course class, not by adding a course to this aggregation.
When the course in fact allows a student to subscribe, it will automatically add the course reference to the student's list. The same will happen on unsubscription of the course, the course will automatically remove the course's reference from the student's aggregation list.

> Error handling
- \[Warning\] Cannot add a Course twice, will output an error
- \[Warning\] Removing an unexisting Course will output an error

---

### Class Staff

The class Staff represents an abstract Staff in the system.
You cannot instanciate a generic Staff, staff must be concrete derivate object.

Staff is responsible of signing forms, with the method `sign`

---

### Class Professor

The class Professor represents a staff member with the conrete implementation of a Professor

The professors have an association with a course. They do not  depend on each other, both are compleately independent.
A Professor is able to exist without a course and the other way arround.
By assigning a professor to a course `course.assign(professor)` the Course will automatically add it's reference to the professor's aggregation.
You cannot assign a course if there is an already assigned one, you can remove the old one with `professor.RemoveCourse()`

> Error handing
- \[Warning\] Cannot assign a Course twice, will output an error
- \[Prevention\] Deal with dangling pointers by using a smart pointer (weak ptr)

---

### Class Secretarial Office

Is a class representing a Room with a concrete implementation of Secretarial Office

The secretarial office is in charge of compiling all the forms that the secretaries create
The secretaries will use this office to archive forms
A form can be archived if it was already executed.
The secretarial office has an aggregation of Forms. 

---

### Class Secretary

The class Secretary is in charge of creating and archiving forms

It has an association with the secretarial office 1..1 through current room
Secreraries are in charge of managing forms
A secretary cannot archiveForms without a valid secretarial office
But the secretarial office can exist without knowing about secretaries

---

### Class Headmaster

The class represents a staff member with the conrete implementation of a Headmaster

Headmasters have an aggregation of forms to validate, which is a collections of forms that they have requested to the secretary. Headmasters only allow forms that are validated by them, you can't just give them any form!!!

The headmaster is in charge of comunicating with the Secretary, later on we will find out he is the mediator of the school.
He is the one that allows all the elements in the school to comunicate between each other.

---

UML Diagrams

