# Piscine [42Lisbon]

---

## This Repo is my Code Vault for the exercises developed through the Piscine :

If you find this usefull im happy for you, but remember that copying won't do any good for you.
Find your own solutions, and if you are inspired with mine im gladd ;)

## Devopment Dependencies

The Piscine is Divided in 3 main groups. Rushes, Exams and Exercises(Days).

| Exercices | Subject | Solutions | Tests [Main] |
| ------ | ------ | ------ | ------ |
| Day00 | Shell | [Day00/](Day00) | 
| Day01 | Shell | [Day01/](Day01) | NA |
| Day02 | C | [Day02/](Day02) | [tests_day02](Day02/tests) |
| Day03 | C | [Day03/](Day03) | [tests_day03](Day03/tests) |
| Day04 | C | [Day04/](Day04) | [tests_day04](Day04/tests) |
| Day05 | C | [Day05/](Day05) | [tests_day05](Day05/tests) |
| Day06 | C | [Day06/](Day06) | [tests_day06](Day06/tests) |
| Day07 | C | [Day07/](Day07) | [tests_day07](Day07/tests) |
| Day08 | C | [Day08/](Day08) | [tests_day08](Day08/tests) |
| Day09 | C | [Day09/](Day09) | [tests_day09](Day09/tests) |
| Day10 | C | [Day10/](Day10) | [tests_day10](Day10/tests) |
| Day11 | C | [Day11/](Day11) | [tests_day11](Day11/tests) |
| Day12 | C | [Day12/](Day04) | [tests_day12](Day12/tests) |

<img width="936" alt="Screen Shot 2022-06-30 at 15 52 37" src="https://user-images.githubusercontent.com/28810331/176725351-baef877a-708c-41b7-bad9-b55033d5d216.png">

## To Use my Tests you must follow this steps:

### Instalation
```bash
	# Install project and copy tests to your own project
	$ git clone https://github.com/pulgamecanica/Piscine_42.git TEST
	# Choose which tests you want to copy (Day01, 02 etc....)
	$ cp -r TEST/DayXX/tests ../your_directory
	$ cp -r TEST/DayXX/run_tests.sh ../your_directory/
	# Delete this dir after copying the tests to your project...
	$ rm -r TEST
	# The /tests/ folder must be on the root of your project that is in the same folder as ex00, ex01 etc, and also the script must be on the root.

	# Run the Tests:
	$ cd ../your_directory
	$ bash run_tests.sh
```

### Run Tests:
It's highly recomended that you open the Tests, there you can assign values on the main function.

### Some Examples
```bash 
	
	# Day03
	# Compile and run the program
	$ bash run_tests.sh

	ex00/ft_ft.c: OK!
	ex01/ft_ultimate_ft.c: OK!
	ex02/ft_swap.c: OK!
	ex03/ft_div_mod.c: OK!
	ex04/ft_ultimate_div_mod.c: OK!
	ex05/ft_putstr.c: OK!
	ex06/ft_strlen.c: OK!
	ex07/ft_rev_int_tab.c: OK!
	ex08/ft_sort_int_tab.c: OK!

	- - - - - - - - - - -
	Ex00 ft_ft ...
	Memory Slot: 0x7ffeea5a0b04, Value: 42
	- - - - - - - - - - -
	Ex01 ft_ultimate_ft ...
	Memory Slot: 0x7ffee6e51ac4, Value: 42
	- - - - - - - - - - -
	Ex02 ft_swap ...
	Value of n1 is: 9 and the value of n2 is: 6.
	Now the value of n1 is: 6 and the value of n2 is: 9.
	- - - - - - - - - - -
	Ex03 ft_div_mod[14, 6] ...
	Division: 2, Module: 2
	- - - - - - - - - - -
	Ex04 ft_ultimate_div_mod[25, 7] ...
	Division: 3, Reminder: 4
	- - - - - - - - - - -
	Ex05 ft_putstr ...
	Don't Panic!
	- - - - - - - - - - -
	Ex06 ft_strlen ...
	The String is 42 characters long.
	- - - - - - - - - - -
	Ex07 ft_rev_int_tab.c[55, 32, 23, 53, 10, 6] ...
	55
	32
	23
	53
	10
	6
	- - - - - - - - - - -
	Ex08 ft_sort_int_tab.c ...
	0
	6
	10
	23
	32
	53
	- - - - - - - - - - -
	Nice Job :D !!!
``` 

### Good Luck and don't drown! Keep Swimming! ;D

#### Passing ALL my tests does **NOT** guarantee that you'll pass Moulinette, althoug if you FAIL any of my tests, then you won't pass Moulinette tests for sure!
For any further questions contact me at: **pulgamecanica11@gmail.com**
