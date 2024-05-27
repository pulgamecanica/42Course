# Makefile created automatically from script
# C++ Algorithms_on_Graphs [Thu May 23 08:59:27 PM CEST 2024]
#MAKEFLAGS += --silent  # Silence makefile [Commented by default]
NAME_1            =   reachability
NAME_2            =   connected_components

INC_PATH        =   includes

SRC_PATH        =   srcs

SRC             = srcs/Graph.cpp \

CFLAGS          =   -Wall -Wextra -Werror -std=c++23 -pedantic -fsanitize=address #-O3 -g -fsanitize=leak

INC             =   -I${INC_PATH}

OBJ_D           =   objs

OBJ             =   $(SRC:${SRC_PATH}/%.cpp=${OBJ_D}/%.o)

UNAME           :=  $(shell uname)

RM              =   rm -rf

ifeq ($(UNAME), Darwin)
    CC = c++
else ifeq ($(UNAME), FreeBSD)
    CC = clang++
else
    CC  = g++
    CFLAGS += -D LINUX
endif

all:        ${NAME_1} ${NAME_2}

${OBJ_D}/%.o:${SRC_PATH}/%.cpp
			@mkdir -p ${OBJ_D}
			$(CC) $(CFLAGS) ${INC} -o $@ -c $<

${NAME_1}:   ${OBJ} $(NAME_1).cpp
			@printf "Compiling $(C_YELLOW)$(NAME_1)$(C_END) ... \n"
			$(CC) $(CFLAGS) ${INC} $(OBJ) $(NAME_1).cpp -o $(NAME_1)
			@printf "$(C_GREEN)DONE$(C_END)\n"

${NAME_2}:   ${OBJ} $(NAME_2).cpp
			@printf "Compiling $(C_YELLOW)$(NAME_2)$(C_END) ... \n"
			$(CC) $(CFLAGS) ${INC} $(OBJ) $(NAME_2).cpp -o $(NAME_2)
			@printf "$(C_GREEN)DONE$(C_END)\n"

test:       all
			@printf "$(C_BLUE)Testing $(C_YELLOW)$(NAME_1)$(C_END)\n"
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@./$(NAME_1) inputs/5_input.txt
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@printf "$(C_BLUE)Testing $(C_YELLOW)$(NAME_2)$(C_END)\n"
			@./$(NAME_2) inputs/5_input.txt
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@printf "\n$(C_BLUE)Finished Test...$(C_END)\n"
			@$(MAKE) show --no-print-directory

clean:
			$(RM) $(OBJ_D)
			@printf "$(C_RED)Cleaning objs$(C_END)\n"

fclean:     clean
			$(RM) $(NAME_1) $(NAME_2)
			@printf "$(C_RED)Deleted Everything$(C_END)\n"

re: fclean all

show:
	@printf "$(C_GREEN)"
	@printf "UNAME     : $(UNAME)\n"
	@printf "NAME      : $(C_YELLOW)$(NAME_1)$(NAME_2)$(C_GREEN)\n"
	@printf "CC        : $(CC)\n"
	@printf "CFLAGS    : $(CFLAGS)\n"
	@printf "INCLUDES  : $(INC)\n"
	@printf "SRC       : $(C_YELLOW)$(SRC)$(C_GREEN)\n"
	@printf "OBJ       : $(C_YELLOW)[$(OBJ_D)] --> $(OBJ)$(C_END)\n"

.PHONY: all test re

C_BLACK     = \033[0;30m
C_RED       = \033[0;31m
C_GREEN     = \033[0;32m
C_YELLOW    = \033[0;33m
C_BLUE      = \033[0;34m
C_WHITE     = \033[0;37m
C_END       = \033[0m

