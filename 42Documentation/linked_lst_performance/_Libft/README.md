# Libft [42Lisbon]

## To Use my tests follow this steps:

## **1**
Download my [tests/](tests) Folder and place it into your root

```bash
.
|- Makefile
|- srcs/ft_functions.c
|- includes/libft.h
|- tests/.... <- (My Folder)
```

#### If you are in the root of the project you can just run this commands:

```bash
git clone https://github.com/pulgamecanica/42Course.git
```
```bash
mv 42Course/42Core/Libft/tests .
```
```bash
rm -rf C_Libft
```

### Add the following lines to your Makefile:

```bash
TESTS	= tests/test1.c
TNAME	= test
TOBJS	= ${TESTS:.c=.o}
```

## **2**

### Now add the following rule to the Makefile:

```bash
test: re ${TOBJS}
	${CC} -g ${CFLAGS} ${TOBJS} -L. -lft -o ${TNAME}
```

## **3**
I have 4 tests. The 3th one does NOT work, it is NOT a test of mine but I might make it work some day...

| TEST | Subject | Works? | rule |
| ------ | ------ | ------ | ------ |
| [test1](tests/test1.c) | Mandatory functions | YES | ```make test```|
| [test2](tests/test2.c) | Bonus | YES | ```make bonus test```|
| [test3](tests/test3.c) | Mandatory functions + Bonus | NOPE | ```make bonus test ``` |
| [test4](tests/test4.c) | Bonus | YES | ```make bonus test ``` |

### To run the different tests you MUST go to the Makefile and change the ${TESTS}
For example to run the second test you must change on the Makefile:
- TESTS	= tests/test2.c

## **4**

### Finally to run the test you should execute the file generated:
Example: ./test "Hello World"

## REMEMBER TO EDIT YOUR MAKEFILE & REMOVE THE TEST
