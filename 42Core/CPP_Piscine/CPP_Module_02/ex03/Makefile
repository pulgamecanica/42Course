# Makefile created automatically from script

# C++ bsp [Wed Apr  6 23:22:41 WEST 2022]

#MAKEFLAGS += --silent	# Silence makefile [Commented by default]

NAME	=	bsp

SRC	=	$(wildcard srcs/*.cpp)

CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -fsanitize=address

INC	=	-I includes

OBJS_D	=	objs

OBJ	=	$(SRC:srcs/%.cpp=$(OBJS_D)/%.o)

UNAME	:=	$(shell uname)

RM		= rm -rf

LIB	= ar rc

RL	= ranlib

LIBNAME = libbsp.a

ifeq ($(UNAME), Darwin)
	CC = g++
else ifeq ($(UNAME), FreeBSD)
	CC = clang++
else
	CC	= g++
	CFLAGS += -D LINUX
endif

$(OBJS_D)/%.o:srcs/%.cpp
			@mkdir -p $(OBJS_D)
			$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(NAME): ${OBJ}
	${LIB} ${LIBNAME} ${OBJ}
	${RL} ${LIBNAME}
	@printf "$(C_GREEN)CREATED: ${LIBNAME} $(C_END)\n"

all:		$(NAME)

clean:
			$(RM) $(OBJS_D)
			@printf "$(C_RED)Cleaning objs$(C_END)\n"

fclean: 	clean
			$(RM) $(NAME) ${LIBNAME}
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

.PHONY: all re

C_BLACK		= \033[0;30m
C_RED		= \033[0;31m
C_GREEN		= \033[0;32m
C_YELLOW	= \033[0;33m
C_BLUE		= \033[0;34m
C_WHITE		= \033[0;37m
C_END		= \033[0m
