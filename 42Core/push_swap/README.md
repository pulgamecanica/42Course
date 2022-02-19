# Push Swap [42Lisbon]

---

If you find this usefull im happy for you, but remember that copying won't do any good for you.
Find your own solutions, and if you are inspired with mine im gladd ;)


## Devopment Dependencies

| Name | Subject | Link |
| ------ | ------ | ------ |
| Libft | C | [Libft](https://github.com/pulgamecanica/C_Libft) |
| Tests | Shell | [tests](tests/) |
| Checker | C | [Checker](checker/) |

## How to use my program?

## Tests Instalation

```bash
	# Install project
	$ git clone https://github.com/pulgamecanica/push_swap.git push_swap
	# Copy the tests
	$ cp -r tests .
	# Delete this dir after copying the tests to your project...
	$ rm -r push_swap

``` 

## Test Programs:

### I created 2 Tests for Push swap

+ ##### Test5
	- Name: [test5.sh](tests/test5.sh)
	- Purpose: This test will display run ALL combinations of five numbers and will display how many moves it took your push swap to solve.
	- Usage: At the root of your repo run  ```sh ./tests/test5.sh ```

+ ##### Test
	- Name: [test.sh](tests/test.sh)
	- Requirements: You must have the rule "run" and "run_checker" on your [Makefile](Makefile)
	- Purpose: This test is interactive, therefore you will be able to change the inputs for push swap. You can change the range (default (-500..500))and how many numbers you want to input (default first(100)), you can also change the amount of random tests you want to run (default 100).
	- Usage: In the Makefile you can change the range and ammount on the MACRO "GEN" the code is quite intuitive... The amount of tests can be changed on the [test.sh](tests/test.sh) on the variable "count". When you finish at the root of your repo run  ``` sh ./tests/test.sh ```
	- NOTE: (you have to substract - 1 to the number displayed, if you take a look at the test file you'll see why... when reading with wc -l the file it counts also the command at the top...)


## Checker Programmmmmm
```bash 
	
	# Push Swap
	# Compile and run the checker
	$ make -C checker
	$ ./checker/checker "HEllo World"
	pa
	KO
	$ ./checker/checker 1 4 3
	ra
	sa
	rra

	OK
	$ ./checker/checker "1 4 3da"
	Error

	OK
	$ ARG=`ruby -e "puts (-500..500).to_a.shuffle.first(100).join(' ')"`; ./push_swap $ARG | ./checker/checker $ARG
	..
	..
	..
	.. etc all moves
	OK
	- - - - - - - - - - -
	- -  SHOW  OPTION - -
	$ make -C checker show
	$ ./checker/checker "4 0 -3 1"
	ra
	ra
	*/---------- ra---------\*
	 |	    0: 0     	|
	 |	    1: -3    	|
	 |	    2: 1     	|
	 |	    3: 4     	|
	*\---------- ra---------/*
	ra
	ra
	*/---------- ra---------\*
	 |	    0: -3    	|
	 |	    1: 1     	|
	 |	    2: 4     	|
	 |	    3: 0     	|
	*\---------- ra---------/*
	pb
	pb
	*/----------------------\*
	 |	A	 B 	|
	 | 		 #	|
	 |       1 	-3      |
	 |       4  		|
	 |       0  		|
	 | –		      – |
	*\----------------------/*
	rra
	rra
	*/----------rra---------\*
	 |	    0: 0     	|
	 |	    1: 1     	|
	 |	    2: 4     	|
	*\----------rra---------/*
	pa
	pa
	*/----------------------\*
	 |	A	 B 	|
	 | 		 #	|
	 |     -3  		|
	 |       0  		|
	 |       1  		|
	 |       4  		|
	 | –		      – |
	*\----------------------/*
	OK

``` 

For any further questions contact me at: **pulgamecanica11@gmail.com**
