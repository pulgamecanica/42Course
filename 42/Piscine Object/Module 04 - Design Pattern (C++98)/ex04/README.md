# Mediator

***

It is obvious that the mediator is going to be the headmaster.

***

The mediator design pattern is a bit controversial in my opinion, it applies the Single Responsibility Principle. 
BEcause all the various objects will only serve as nodes where the responsabilitie of comunicating is going to be singular to the Mediator, so all the logic will be there.

But overtime the Mediator can become a "God Object", which is an object which is aware of ALL things happening in the program. And can get extreamly unreadable.