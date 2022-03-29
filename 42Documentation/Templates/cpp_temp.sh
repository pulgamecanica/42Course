#!/bin/sh

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

echo "Creating Makefile..."

echo "# Makefile created automatically from script\n"				>> $PROJECT/Makefile

DATE=`date`

echo	"# C++ $PROJECT [$DATE]\n"						>> $PROJECT/Makefile

echo	"#MAKEFLAGS += --silent	# Silence makefile [Commented by default]\n"	>> $PROJECT/Makefile

echo	"NAME	=	$PROJECT\n"						>> $PROJECT/Makefile

cat << EOF									>> $PROJECT/Makefile

SRC	=	srcs/main.cpp

CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -fsanitize=address

INC	=	-I .

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
// Automatic script
#include <iostream>
#include <string>
using namespace std;

int	main(void)
{
	cout << "Hello Friend\nPulgamecanica greets you :D\n";
	return (0);
}
EOF
echo "$PROJECT succesfully created! :D!"
