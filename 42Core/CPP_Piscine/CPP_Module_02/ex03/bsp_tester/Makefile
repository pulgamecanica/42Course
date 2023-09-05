# Makefile created automatically from script\n
# C++ BSP_TESTER [Thu Apr  7 16:41:18 WEST 2022]\n
# ***************************************************
# iOS INSTALL
# Download the latest version os SDL
# https://www.libsdl.org/
# If you have enough permissions then:
#  copy the installed folder --> /Library/Frameworks
# else
#  copy to /USER/Library/Frameworks
# Link: -F /Library/Frameworks -framework SDL2
# **************************************************
# Linux/Unix SDL INSTALL
# $ git clone https://github.com/libsdl-org/SDL
# $ cd SDL/
# $ mkdir build
# $ cd build
# $ ../configure
# $ make
# $ sudo make install
# Then compile with: `sdl2-config --cflags --libs`
# ***************************************************

MAKEFLAGS += --silent	# Silence makefile [Commented by default]\n

NAME	=	BSP_TESTER

SRC	=	$(wildcard srcs/*.cpp srcs/*/*.cpp)

CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -fsanitize=address

INC	=	-I includes -I ../includes/ -I ../

OBJS_D	=	objs

LFLAGS	=	-L ../ -lbsp

OBJ	=	$(SRC:srcs/%.cpp=$(OBJS_D)/%.o)

UNAME	:=	$(shell uname)

RM	= rm -rf

ifeq ($(UNAME), Darwin)
	CC = g++
 	LFLAGS	+= -F /Users/arosado-/Library/Frameworks -framework SDL2
	INC += -I/Users/arosado-/Library/Frameworks/SDL2.framework/Headers
else ifeq ($(UNAME), FreeBSD)
	CC = clang++
else
	CC	= g++
	CFLAGS += -D LINUX `sdl2-config --cflags --libs`
endif

$(OBJS_D)/%.o:srcs/%.cpp
			@mkdir -p $(OBJS_D)
# 			@mkdir -p $(OBJS_D)/mlx_utils
			$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(NAME):	$(OBJ)
			@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
			$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LFLAGS) $(INC)
			@printf "$(C_GREEN)DONE$(C_END)\n"

test:		re
			@printf "$(C_BLUE)Testing $(C_YELLOW)$(NAME)$(C_END)\n"
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@./$(NAME)
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@printf "\n$(C_BLUE)Finished Test...$(C_END)\n"
			@$(MAKE) show
			@$(MAKE) fclean

all:		$(NAME)

clean:
			$(RM) $(OBJS_D)
			@printf "$(C_RED)Cleaning objs$(C_END)\n"

fclean: 	clean
			$(RM) $(NAME)
			@printf "$(C_RED)Deleted Everything$(C_END)\n"

re: fclean all

show:
	@printf "$(C_GREEN)"
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(C_YELLOW)$(NAME)$(C_GREEN)\n"
	@printf "CC			: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "INCLUDES	: $(INC)\n"
	@printf "SRC		: $(C_YELLOW)$(SRC)$(C_GREEN)\n"
	@printf "OBJ		: $(C_YELLOW)[$(OBJS_D)] --> $(OBJ)$(C_END)\n"

.PHONY: all test re

C_BLACK		= \033[0;30m
C_RED		= \033[0;31m
C_GREEN		= \033[0;32m
C_YELLOW	= \033[0;33m
C_BLUE		= \033[0;34m
C_WHITE		= \033[0;37m
C_END		= \033[0m
