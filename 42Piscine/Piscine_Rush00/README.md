# Rush 00 <42Lisbon>

---

## Rush Description :

You must create a function that takes an *x* and *y* int argument and display on the screen a rectangle of *x* characters

## Devopment Files

- main.c
- ft_putchar.c
- rush0X.c (X -> 1..4)

# Getting started

## Instalation
```bash
	# Install project
	$ git clone https://github.com/pulgamecanica/Piscine_Rush00.git <Name>
	$ cd <Name>

	# Run the Norme
	$ norminette -R CheckForbiddenSourceHeader
```

## Run Tests:
In order to run Tests, you must open *main.c* and assign values to the rush function.

## Some Examples
```bash 
	
	# main -> rush(4, 4);
	# Compile and run the program
	$ gcc -Wall -Werror -Wextra main.c ft_putchar.c rush01.c && ./a.out
	o--o
	|  |
	|  |
	o--o
	# main -> rush(3, 5);
	$ gcc -Wall -Werror -Wextra main.c ft_putchar.c rush01.c && ./a.out
	/*\
	* *
	* *
	* *
	\*/
	# main -> rush(2, 5);
	$ gcc -Wall -Werror -Wextra main.c ft_putchar.c rush02.c && ./a.out
	ABBA
	B  B
	B  B
	CBBC
	# main -> rush(5, 10);
	$ gcc -Wall -Werror -Wextra main.c ft_putchar.c rush03.c && ./a.out
	ABBBC
	B   B
	B   B
	B   B
	B   B
	B   B
	B   B
	B   B
	B   B
	CBBBA
``` 

## Functions comments
- -main(void) : Function created to test the program.
- -ft_putchar(char c) : Function to display a character on the screen.
- -rush(int x, int y) : This Function loops through the number of lines *y* and for each line it prints the characters it should. The Function should only work if *x* is greater (>) than zero (0) and there is at least one line *y*.
- -ft_put_n_char(int n, char c, char start_c, char end_c) : This function takes a character and a starting character and an ending character , prints the starting character followed by printing *n* times the character and at last prints the ending character.


For any further questions contact me at: **pulgamecanica11@gmail.com**
