## Questions:

Are the Rails two way?
Can the rail be One Way only while others are not?
Can there be two rails between two points (two trains traveling using the same path each on his own rail)
When a train is on a node, stoped, can other train pass through?
Are cities and rails both nodes?
Is the distance in KM and the speed in KM/H?
Should I set a max speed (LIMIT)?
When starting a journey should I calculate the total time based on the acceleration and breaking? (The train attempts to accelerate until LIMIT and continues until it arrives to the station, braking and calculating the time to brake)
Should I use multithreading?
How do I convert the maximum acceleration to KM/H (max speed)
How do I convert the maximum brake force to time (seconds) given the current speed?
How should I handle date time?, is the simulation for only one day?, What if a travel takes longer than one day?
Random event simulator should be based on what? (Entropy) Need some links!
What is an overtaking?
How is overtaking done in real life?
How do overtaking systems work? Can you overtake while traveling on a rail? Can they only be done on the stations?
Should I implement a Logging system for each train?
In case of an event, should the train stop right there, or wait the given amoun at the station next available (City)? Maybe this depends.
What happens if there is no way to get to the destination?
How often should the system update(fps)?
Can trains wait in a node and other trains pass by? (is this overtaking)
Can Trains block the rails?
Can trains calculate their paths on runtime? (after they started their journey, in case another train is blocking the way?)
Should I calculate the REAL time when finding the path?
What is the find path algorithm?
Should the network be 100% connected?
Can there be "Broken" rails "Out of service" (lol)?
Should Trains stop at each station?\

### Events & Effects