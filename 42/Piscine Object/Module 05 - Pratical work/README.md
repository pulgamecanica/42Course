# CppOnRails

CppOnRails is a C++ application designed to simulate train schedules and railway systems. The program parses elements such as nodes, rails, and events from a single elements file and train schedules from multiple schedule files. Detailed error handling is provided to ensure that parsing errors are reported with precise information about the location and nature of the error.

## Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Building](#building)
- [Usage](#usage)
- [File Formats](#file-formats)
  - [Elements File](#elements-file)
  - [Schedule Files](#schedule-files)
- [Error Handling](#error-handling)
- [Examples](#examples)
- [Options](#options)

## Features

- Parse railway system elements (nodes, rails, events) from a file.
- Parse multiple train schedule files.
- Detailed error reporting with file, line, and column information.
- Colored terminal output for errors.

## Requirements

- C++17 or later
- CMake 3.10 or later

## Building

To build the project, follow these steps:

1. Clone the repository:

```sh
git clone https://github.com/yourusername/CppOnRails.git
cd CppOnRails
```

2. Create a build directory and run CMake:

```sh
mkdir build
cd build
cmake ..
make
```

3. Run the program:

```sh
./CppOnRails [options]
```

## Usage

The program requires an elements file and a directory containing schedule files to run. The schedule files must have a `.schedule` extension. Use the following options to specify the files and directory:

```sh
./CppOnRails --elements <elements_file> --directory <schedule_directory>
```

### Options

- `-h, --help`: Show help message.
- `-e, --elements <elements_file>`: Specify the elements file.
- `-d, --directory <schedule_directory>`: Specify the directory containing schedule files.

### Example

```sh
./CppOnRails --elements elements.txt --directory schedules/
```

## File Formats

### Elements File

The elements file defines the nodes, rails, and events in the railway system. It must follow the format below:

#### Nodes

```plaintext
Node <NodeName>
```

#### Rails

```plaintext
Rail <NodeName1> <NodeName2> <Distance>
```

#### Events

```plaintext
Event <EventType> <Probability> <Duration> <Location>
```

### Schedule Files

Each schedule file represents a different simulation. Schedule files must have a `.schedule` extension and follow the format below:

```plaintext
<Name> <MaxAcceleration> <MaxBrakeForce> <Departure> <Arrival> <Hour>
```

## Error Handling

The program provides detailed error reporting, including:

- File name
- Line number
- Column number
- Exact location of the error in the line, highlighted with a `^` symbol

### Error Example

```sh
Error in file elements.txt at line 3, column 12: Distance cannot be negative
Rail CityA CityB -50
            ^
```

## Examples

### Elements File (`elements.txt`)

```plaintext
Node CityA
Node CityB
Node CityC

Rail CityA CityB 100.5
Rail CityB CityC 200.3
Rail CityA CityC 150.0

Event Accident 0.05 2.0 CityA
Event Delay 0.10 1.5 CityB
Event Maintenance 0.03 3.0 CityC
```

### Schedule File (`schedule1.schedule`)

```plaintext
TrainAB1 1.87 1.4 CityA CityB 14h10
TrainBC1 2.10 1.6 CityB CityC 15h30
TrainCA1 1.75 1.2 CityC CityA 17h00
```

### Schedule File (`schedule2.schedule`)

```plaintext
TrainAB2 1.95 1.5 CityA CityB 12h20
TrainBC2 2.20 1.7 CityB CityC 16h40
TrainCA2 1.85 1.3 CityC CityA 18h15
```

### Detailed Explanation

1. **Nodes**: Define the nodes (stations) in the railway system.
2. **Rails**: Define the rails (connections) between nodes, including the distance.
3. **Events**: Define events that can occur in the railway system, with a probability and duration at a specific location.
4. **Trains**: Define the train schedules, including the train's name, maximum acceleration, maximum brake force, departure station, arrival station, and departure time.

### How to Build Files

- **Elements File**: Create a text file (e.g., `elements.txt`) with the format specified above. Define all nodes, rails, and events.
- **Schedule Files**: Create one or more text files with a `.schedule` extension (e.g., `schedule1.schedule`, `schedule2.schedule`) containing the train schedules.

### Example Directory Structure

```
/path/to/schedule_files/
|-- elements.txt
|-- schedule1.schedule
|-- schedule2.schedule
```

### Parsing

The program reads and parses the elements and schedule files, storing the parsed data in a structured format. Detailed error handling ensures that any issues in the input files are reported with precise information about the location and nature of the error, making it easier to identify and fix issues.

### Building and Running the Program

1. **Prepare the elements file (`elements.txt`) and schedule files (`*.schedule`) as per the specified formats.**
2. **Build the project as described in the [Building](#building) section.**
3. **Run the program with the appropriate options to specify the elements file and schedule directory:**

```sh
./CppOnRails --elements elements.txt --directory schedules/
```

---

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