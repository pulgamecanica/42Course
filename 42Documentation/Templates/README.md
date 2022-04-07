# Templates:  C++ Project |  C++ Class
> Linux & Darwin(macOS) compatible
---

### About C++ Template
+	Povides a minimalist structure for your C++ projects.
+	Generates a c++ compatible Makefile, header and a main for your project.
+	Creates Classes with constructors, destructors, operators and it's header.
---
###   Set Up
##### 	Follow this instructions to install the commands in your environment...
```bash
    git clone https://github.com/pulgamecanica/42Course.git && cd 42course/42Documentation/Templates/
    sh setup.sh
```
---
### Usage
*create new c++ project*
>	**cpp_new** Project_name

*create a new class*	*execute at project root
>	**cpp_class** Class_name

#####	Examples: 
<img width="337" alt="Screen Shot 2022-04-01 at 12 46 11 AM" src="https://user-images.githubusercontent.com/28810331/161167296-1a151537-8843-4683-8f3b-8870a9082bf2.png">

<img width="367" alt="Screen Shot 2022-04-01 at 12 44 19 AM" src="https://user-images.githubusercontent.com/28810331/161167112-376e73bd-5fba-4409-a3fb-17d984f462cc.png">


---
### Makefile:
    Rules --> [all, clean, fclean, re, show, NAME: PROJECT_NAME]
+   all: compile the project $(NAME)
+   clean: clean all .o (binary/objects files)
+   fclean: run clean and clean the executable $(NAME)
+   re: run fclean & all
+   show: show compilation information (flags, includes, SO, compiler, sources, objects, and executable)
+   test: run re & execute ./$(NAME) without arguments & run show & run fclean
+   MAKEFLAGS += --silent --> to silence the makefile messages

### Video :D


https://user-images.githubusercontent.com/28810331/162333739-3ea444f6-1331-43e2-ac8e-817d60d41a34.mp4



##### TODO
-   std::ostream&   operator<<(std::ostream&, const Fixed&);
-   Fixed&              operator= (Fixed);
-   object params
-   remove print
-   Add Lib Rule to Makefile

<img width="1058" alt="Screen Shot 2022-03-29 at 11 06 02 PM" src="https://user-images.githubusercontent.com/28810331/160715139-cc37caa1-fb4b-4f9d-a762-526cb3d138ce.png">
