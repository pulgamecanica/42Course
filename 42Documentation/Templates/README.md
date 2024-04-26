***

# C++ Project and Class Creator

> Linux & Darwin(macOS) compatible

This script automates the process of creating C++ projects and classes, making development faster and more efficient.

### About C++ Template
+   Povides a minimalist structure for your C++ projects.
+   Generates a c++ compatible Makefile, header and a main for your project.
+   Creates Classes with constructors, destructors, operators and it's header.


<img style="margin: auto" width=80% src=https://github.com/pulgamecanica/42Course/assets/28810331/e33452b4-f97c-4bc3-b1ec-ba2f647624ed>

***

### New Project

Quickly create a new C++ project with customizable folder structures and Makefile settings.

<img width="876" alt="Screenshot 2024-04-24 at 21 14 49" src="https://github.com/pulgamecanica/42Course/assets/28810331/bbf58ced-26ab-4513-897b-a7c067db4e50">

***

### New Class

Generate C++ class files with options for copy constructors, assignment operators, and overloading operators.

<img width="858" alt="Screenshot 2024-04-24 at 21 23 49" src="https://github.com/pulgamecanica/42Course/assets/28810331/52a558e5-95e2-4bed-b4d3-0e5efba2a788">

## Prerequisites

Before using the script, ensure that the following prerequisites are met:

- **Bash Shell**: The script is written in Bash and requires a compatible shell environment.
- **GNU Make**: Make is used to automate the build process and requires the GNU implementation.
- **C++ Compiler**: A C++ compiler such as GCC or Clang is needed to compile the generated C++ code.

## Installation & SetUp

```bash
    git clone https://github.com/pulgamecanica/42Course.git && cd 42course/42Documentation/Templates/
    sh set_up.sh
```
***

## Usage

### Creating a New Project

To create a new C++ project, use the following command:

```bash
cpp_new [options] [name]
```
If you don't provide a name, you will be promped to do it after

#### Options

| Option | Description |
| ------ | ----------- |
| `-s, --sources-path <path>` | Specify the sources path (default: ./srcs) |
| `-i, --includes-path <path>` | Specify the includes path (default: ./includes) |
| `-m, --main-path [<path>]` | Specify the main path (default: --sources-path). Omitting a path will fallback to the root (./) |
| `-q, --quiet` | Enable quiet mode on Makefile |
| `-v, --version [version]` | Specify the C++ version (default: 98). Example: -v 98 -v 11 -v 23 |

#### Example

To create a project named "my_project" with custom paths:

```bash
cpp_new -s src -i include my_project
```

***

### Creating a New Class

To create a new C++ class, use the following command:

```bash
cpp_class [options] [name]
```

#### Options

| Option | Description |
| ------ | ----------- |
| `-c, --copy-constructor` | Generate copy constructor |
| `-a, --assign-operator` | Generate assign operator |
| `-o, --overloading` | Generate overloading operator (<<) |
| `-d, --defaulting` | Use default for the copy and assign if not true. (this feature is only for C++ version >= 11) |
| `-p, --params <param_list>` | Specify parameters with types (e.g., param1:string param2:int). Omitting a type will fallback to std::string |

#### Example

To create a class named "MyClass" with copy constructor, assign operator, and custom parameters:

```bash
cpp_class -c -a -p "param1:int param2:string" MyClass
```

#####   Examples: 
<img width="337" alt="Screen Shot 2022-04-01 at 12 46 11 AM" src="https://user-images.githubusercontent.com/28810331/161167296-1a151537-8843-4683-8f3b-8870a9082bf2.png">

<img width="367" alt="Screen Shot 2022-04-01 at 12 44 19 AM" src="https://user-images.githubusercontent.com/28810331/161167112-376e73bd-5fba-4409-a3fb-17d984f462cc.png">

***

### Makefile:
    Rules --> [all, clean, fclean, debug, re, show, test, NAME: PROJECT_NAME]
+   all: compile the project $(NAME)
+   clean: clean all .o (binary/objects files)
+   fclean: run clean and clean the executable $(NAME)
+   re: run fclean & all
+   debug: compiles with the DEBUG Macro which is defined on the .inc
+   show: show compilation information (flags, includes, SO, compiler, sources, objects, and executable)
+   test: run re & execute ./$(NAME) without arguments & run show & run fclean
+   MAKEFLAGS += --silent --> to silence the makefile messages

***

## Tips:

Configure an alias in your .bash_profile or .zshrc with your favourite configuration<br>
So your favourite configuration will be accesible all the time<br>
For instance I can give you some examples.

***

#### Example 1
##### (Simple structure for a C++98):
```
MyProject/
├── Makefile
├── main.cpp
├── *.cpp
└── *.hpp
```

`alias cpp_new="cpp_new -s . -i . -m . -q -v 98"`

Explanation of options used:
`-s .`: Specifies the sources path as the current directory (.).
`-i .`: Specifies the includes path as the current directory (.).
`-m .`: Specifies the main path as the current directory (.).
`-q`: Enables quiet mode in the generated Makefile.
`-v 98`: Specifies the C++ version as 98.
***

#### Example 2
##### (Modularized example for a C++11):
```
MyProject/
├── Makefile
├── srcs/
│   └── main.cpp
└── includes/
    └── myProject.inc
```
`alias cpp_new="cpp_new -s srcs -i includes -m srcs -v 11"`

`-s srcs`: Specifies the sources path as srcs.
`-i includes`: Specifies the includes path as includes.
`-m srcs`: Specifies the main path as srcs.
`-v 11`: Specifies the C++ version as 11."

***

#### Example 3
##### (Isolated main with modularized structure C++98):
```
MyProject/
├── Makefile
├── main.cpp
├── src/
│   └── object.cpp
└── include/
    └── object.hpp
    └── myProject.inc
```
`alias cpp_new="cpp_new -s src -i include -m . -v 98"`

`-s src`: Specifies the sources path as src.
`-i include`: Specifies the includes path as include.
`-m .`: Specifies the main path as root `.`
`-v 98`: Specifies the C++ version as 98."

***

And you can do the same for the classes if you have a prefered way to do it<br>

For example:

#### Cannonical Form

`alias cpp_class="cpp_class -c -o -a"`

### Video Tutorial  :D

![Template](https://user-images.githubusercontent.com/28810331/163208471-42f32401-08de-4cfc-807e-a33979bc7e30.gif)

<img width="1058" alt="Screen Shot 2022-03-29 at 11 06 02 PM" src="https://user-images.githubusercontent.com/28810331/160715139-cc37caa1-fb4b-4f9d-a762-526cb3d138ce.png">
