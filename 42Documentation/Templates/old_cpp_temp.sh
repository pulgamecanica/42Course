#!/bin/bash

C_BLACK='\033[0;30m'
C_RED='\033[0;31m'
C_GREEN='\033[0;32m'
C_YELLOW='\033[0;33m'
C_BLUE='\033[0;34m'
C_WHITE='\033[0;37m'
C_END='\033[0m'

cpp_new()
{
if [[ "$1" != "" ]];then
	PROJECT="$1"
else
	echo "Enter Project Name..."
	read PROJECT
fi

printf "${C_RED}Creating C++ Project ${C_YELLOW}[$PROJECT]${C_END}\n"

mkdir $PROJECT

mkdir $PROJECT/includes

mkdir $PROJECT/srcs

touch $PROJECT/Makefile

touch $PROJECT/includes/$PROJECT.hpp

touch $PROJECT/srcs/main.cpp

echo "# Makefile created automatically from script\n" >> $PROJECT/Makefile

DATE=`date`

echo	"# C++ $PROJECT [$DATE]\n" >> $PROJECT/Makefile

echo	"MAKEFLAGS += --silent	# Silence makefile [Commented by default]\n" >> $PROJECT/Makefile

echo	"NAME	=	$PROJECT" >> $PROJECT/Makefile

cat << EOF >> $PROJECT/Makefile

SRC	=	\$(wildcard srcs/*.cpp)

CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -pedantic -fsanitize=address

INC	=	-I includes

OBJS_D	=	objs

OBJ	=	\$(SRC:srcs/%.cpp=\$(OBJS_D)/%.o)

UNAME	:=	\$(shell uname)

RM		= rm -rf

ifeq (\$(UNAME), Darwin)
	CC = c++
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
			\$(CC) \$(CFLAGS) \$(OBJ) -o \$(NAME)
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

printf "${C_GREEN}    create    ${C_END}${PROJECT}/Makefile\n"

cat << EOF >> $PROJECT/srcs/main.cpp
//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "${PROJECT}.hpp"

int	main(void)
{
	std::cout << "Hello Friend\nPulgamecanica greets you :D\n";
	return (0);
}
EOF

printf "${C_GREEN}    create    ${C_END}${PROJECT}/srcs/main.cpp\n"

HEADER=$(echo $PROJECT | tr '[:lower:]' '[:upper:]')

cat << EOF >> $PROJECT/includes/$PROJECT.hpp
//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __${HEADER}_HPP__
# define __${HEADER}_HPP__

#include <iostream>

/*
# define BLACK	"\033[0;30m"
# define RED	"\033[0;31m"
# define GREEN	"\033[0;32m"
# define YELLOW	"\033[0;33m"
# define BLUE	"\033[0;34m"
# define WHITE	"\033[0;37m"
# define ENDC	"\033[0m"
*/

#endif
EOF

printf "${C_GREEN}    create    ${C_END}${PROJECT}/includes/${PROJECT}.hpp\n"

printf "${C_YELLOW}${PROJECT}${C_BLUE} succesfully created :D${C_END}\n"
}

cpp_class()
{
if [[ "$1" != "" ]];then
	CLASS="$1"
else
	echo "Enter Class Name..."
	read CLASS
fi

ERROR=0
{
touch srcs/${CLASS}.cpp && touch includes/${CLASS}.hpp
} || {
echo "Sorry something went wrong... please make sure you are inside a cpp project..."
echo "--------------------"
echo "|- Project"
echo "|  - includes/"
echo "|  | - project.hpp"
echo "|  | - Object.hpp"
echo "|  | - .......hpp"
echo "|  - srcs/"
echo "|  | - main.cpp"
echo "|  | - Object.cpp"
echo "|  | - .......cpp"
echo "--------------------"
ERROR=1
}
if [[ "$ERROR" -eq 0 ]];then 

cat << EOF >> srcs/${CLASS}.cpp
//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "${CLASS}.hpp"

${CLASS}::${CLASS}() {
	// TODO (default constructor)
}

${CLASS}::${CLASS}(const ${CLASS}& param) {
	// TODO (copy constructor)
	(void)param;
}

${CLASS}::~${CLASS}() {
	std::cout << "${CLASS}" << " destroyed" << std::endl;
	// TODO (destructor)
}

${CLASS}& ${CLASS}::operator= (const ${CLASS}& param) {
	// TODO (Assignment operatior)
	// std::swap()
	(void)param;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const ${CLASS}& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

EOF

printf "${C_GREEN}    create    ${C_END}srcs/${CLASS}.cpp\n"

HEADERC=$(echo $CLASS | tr '[:lower:]' '[:upper:]')
cat << EOF >> includes/${CLASS}.hpp
//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __${HEADERC}_HPP__
# define __${HEADERC}_HPP__

#include <iostream>

class $CLASS {
	public:
		${CLASS}();
		${CLASS}(const ${CLASS}&);
		~${CLASS}();
		${CLASS}&	operator= (const ${CLASS}&); // const for safety... not super nesessary
};

std::ostream&	operator<<(std::ostream&, const ${CLASS}&);

#endif

EOF

printf "${C_GREEN}    create    ${C_END}includes/${CLASS}.hpp\n"

printf "${C_YELLOW}${CLASS}${C_BLUE} succesfully created! :D!${C_END}\n"
fi
}

# cpp_new $1
# cpp_class $1
