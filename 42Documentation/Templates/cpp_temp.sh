#!/bin/bash

cpp_new()
{
if [[ "$1" != "" ]];then
	PROJECT="$1"
else
	echo "Enter Project Name..."
	read PROJECT
fi

echo "Creating C++ Project [$PROJECT]"

mkdir $PROJECT

mkdir $PROJECT/includes

mkdir $PROJECT/srcs

touch $PROJECT/Makefile

touch $PROJECT/includes/$PROJECT.hpp

touch $PROJECT/srcs/main.cpp

echo "Creating Makefile..."

echo "# Makefile created automatically from script\n"				>> $PROJECT/Makefile

DATE=`date`

echo	"# C++ $PROJECT [$DATE]\n"						>> $PROJECT/Makefile

echo	"MAKEFLAGS += --silent	# Silence makefile [Commented by default]\n"	>> $PROJECT/Makefile

echo	"NAME	=	$PROJECT"						>> $PROJECT/Makefile

cat << EOF									>> $PROJECT/Makefile

SRC	=	\$(wildcard srcs/*.cpp)

CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -fsanitize=address

INC	=	-I includes

OBJS_D	=	objs

OBJ	=	\$(SRC:srcs/%.cpp=\$(OBJS_D)/%.o)

UNAME	:=	\$(shell uname)

RM		= rm -rf

ifeq (\$(UNAME), Darwin)
	CC = clang++
else ifeq (\$(UNAME), FreeBSD)
	CC = clang++
else
	CC	= g++
	CFLAGS += -D LINUX
endif

\$(OBJS_D)/%.o:srcs/%.cpp
			@mkdir -p \$(OBJS_D)
			\$(CC) \$(CFLAGS) \$(INC) -o \$@ -c \$<

\$(NAME):	\$(OBJ)
			@printf "Compiling \$(C_YELLOW)\$(NAME)\$(C_END) ... \n"
			\$(CC) \$(CFLAGS) \$(FSANIT) \$(OBJ) -o \$(NAME)
			@printf "\$(C_GREEN)DONE\$(C_END)\n"

test:		re
			@printf "\$(C_BLUE)Testing \$(C_YELLOW)\$(NAME)\$(C_END)\n"
			@printf "\n\$(C_BLUE)********************************************\$(C_END)\n"
			@./\$(NAME)
			@printf "\n\$(C_BLUE)********************************************\$(C_END)\n"
			@printf "\n\$(C_BLUE)Finished Test...\$(C_END)\n"
			@\$(MAKE) show
			@\$(MAKE) fclean

all:		\$(NAME)

clean:
			\$(RM) \$(OBJS_D)
			@printf "\$(C_RED)Cleaning objs\$(C_END)\n"

fclean: 	clean
			\$(RM) \$(NAME)
			@printf "\$(C_RED)Deleted Everything\$(C_END)\n"

re: fclean all

show:
	@printf "\$(C_GREEN)"
	@printf "UNAME		: \$(UNAME)\n"
	@printf "NAME  		: \$(C_YELLOW)\$(NAME)\$(C_GREEN)\n"
	@printf "CC		: \$(CC)\n"
	@printf "CFLAGS		: \$(CFLAGS)\n"
	@printf "INCLUDES	: \$(INC)\n"
	@printf "SRC		: \$(C_YELLOW)\$(SRC)\$(C_GREEN)\n"
	@printf "OBJ		: \$(C_YELLOW)[\$(OBJS_D)] --> \$(OBJ)\$(C_END)\n"

.PHONY: all test re

C_BLACK		= \033[0;30m
C_RED		= \033[0;31m
C_GREEN		= \033[0;32m
C_YELLOW	= \033[0;33m
C_BLUE		= \033[0;34m
C_WHITE		= \033[0;37m
C_END		= \033[0m
EOF

echo "Creating Sample main.cpp"

cat << EOF >> $PROJECT/srcs/main.cpp
//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "${PROJECT}.hpp"

int	main(void)
{
	cout << "Hello Friend\nPulgamecanica greets you :D\n";
	return (0);
}
EOF

echo "Creating Sample ${PROJECT}.hpp"

HEADER=$(echo $PROJECT | tr '[:lower:]' '[:upper:]')

cat << EOF >> $PROJECT/includes/$PROJECT.hpp

//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __${HEADER}_HPP__
# define __${HEADER}_HPP__

#include <iostream>
using namespace std;

#endif
EOF

echo "$PROJECT succesfully created! :D!"
}

cpp_new $1
