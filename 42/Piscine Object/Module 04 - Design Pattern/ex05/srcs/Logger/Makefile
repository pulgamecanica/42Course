# Makefile created automatically from script
# C++ ex00 [Thu May 16 10:51:04 AM CEST 2024]
#MAKEFLAGS += --silent  # Silence makefile [Commented by default]
NAME	   =   liblogger.a

INC_PATH        =   includes


SRC             =   ConstHeader.cpp \
    DateHeader.cpp \
    StreamLogger.cpp \
    FileLogger.cpp \

CFLAGS          =   -Wall -Wextra -Werror -std=c++11 -pedantic -fsanitize=address #-O3 -g -fsanitize=leak

OBJ_D           =   objs

OBJ             =   $(SRC:%.cpp=${OBJ_D}/%.o)

UNAME           :=  $(shell uname)

LIB   = ar rc

RL    = ranlib

RM              =   rm -rf

ifeq ($(UNAME), Darwin)
    CC = c++
else ifeq ($(UNAME), FreeBSD)
    CC = clang++
else
    CC  = g++
    CFLAGS += -D LINUX
endif

all:        ${NAME}

${OBJ_D}/%.o:%.cpp
	@mkdir -p $(OBJ_D)
	$(CC) $(CFLAGS) -o $@ -c $<

${NAME}:   ${OBJ}
	@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
	$(LIB) $(NAME) $(OBJ)
	$(RL) $(NAME)
	@printf "$(C_GREEN)DONE$(C_END)\n"

clean:
	$(RM) $(OBJ_D) $(OBJ_D_DEBUG)
	@printf "$(C_RED)Cleaning objs$(C_END)\n"

fclean:     clean
	$(RM) $(NAME) $(NAME_DEBUG)
	@printf "$(C_RED)Deleted Everything$(C_END)\n"

re: fclean all

.PHONY: all test re

C_BLACK     = \033[0;30m
C_RED       = \033[0;31m
C_GREEN     = \033[0;32m
C_YELLOW    = \033[0;33m
C_BLUE      = \033[0;34m
C_WHITE     = \033[0;37m
C_END       = \033[0m

