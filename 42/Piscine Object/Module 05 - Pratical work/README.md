## Questions:

| Question | Answer |
| -------- | ------ |
| Are the Rails two way? | The rails are bidirectional, the distance from A-B = B-A |
| Can the rail be One Way only while others are not? | Any enge defines two ways, from A-B and B-A |
| Can there be two rails between two points (two trains traveling using the same path each on his own rail) | There is no such logic, there is only one two rail per edge, from A-B and B-A |
| When a train is on a node, stoped, can other train pass through? | Yes, nodes are like "translation" or "rest" areas, where trains can convey and let other trains pass |
| Are cities and rails both nodes? | Rails are edges, cities are vertices, but the file name, might call "rail" to an area which refers to be a vertice, a bit confussion I know |
| Is the distance in KM and the speed in KM/H? | It is |
| Should I set a max speed (LIMIT)? | Yes, even the subject mentions this |
| When starting a journey should I calculate the total time based on the acceleration and breaking? (The train attempts to accelerate until LIMIT and continues until it arrives to the station, braking and calculating the time to brake) | Yes, based on the train force and weight, you can calculate how much it will take to reach the MAX SEED and also how much it takes to brake. Trains take all of this into consideration and calculate the travel time with this info |
| Should I use multithreading? | For doing the GUI it's a good idea |
| How do I convert the maximum acceleration to KM/H (max speed) | *insert Formula here | 
| How do I convert the maximum brake force to time (seconds) given the current speed? | *insert Formula here |
| How should I handle date time?, is the simulation for only one day?, What if a travel takes longer than one day? | Travels can take several days, the simulation starts on day 0, it may end on whatever day |
| Random event simulator should be based on what? (Entropy) Need some links! | seed time is okay |
| What is an overtaking? | Overtaking is done in the vertices, when a train is waiting, other trains can overtake |
| How is overtaking done in real life? | Good question... :O | I've never seen a train over take another |
| How do overtaking systems work? Can you overtake while traveling on a rail? Can they only be done on the stations? | Only at the stations (vertices) |
| Should I implement a Logging system for each train? | yes, it's a good idea |
| In case of an event, should the train stop right there, or wait the given amoun at the station next available (City)? Maybe this depends. | Depending on the event, some events might be inmediat and vlock the rail for some time, others will need the train to stop at the nearest station | 
| What happens if there is no way to get to the destination? | Display error on the logfile and stay put |
| How often should the system update(fps)? | Probably each second or every 2 seconds |
| Can trains wait in a node and other trains pass by? (is this overtaking) | Exactly |
| Can Trains block the rails? | Yes, in case of event, derail |
| Can trains calculate their paths on runtime? (after they started their journey, in case another train is blocking the way?) | No, the path is decided before hand |
| Should I calculate the REAL time when finding the path | No |
| What is the find path algorithm? | Find the shortest path |
| Should the network be 100% connected? | Not necesarilly |
| Can there be "Broken" rails "Out of service" (lol)? | Yes |
| Should Trains stop at each station? | Nope |

### Events & Effects