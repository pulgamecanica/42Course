# Makefile created automatically from script\n
# C++ ft_containers [Fri Apr 29 12:36:41 WEST 2022]\n

#MAKEFLAGS += --silent	# Silence makefile [Commented by default]\n

NAME	=	containers

SRC	=	$(wildcard srcs/*.cpp)

CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -pedantic -fsanitize=address

INC	=	-I includes

STD_OBJS_D	=	std_objs

STD_OBJ	=	$(SRC:srcs/%.cpp=$(STD_OBJS_D)/%.o)

FT_OBJS_D	=	ft_objs

FT_OBJ	=	$(SRC:srcs/%.cpp=$(FT_OBJS_D)/%.o)

UNAME	:=	$(shell uname)

RM		= rm -rf

ifeq ($(UNAME), Darwin)
	CC = c++
else ifeq ($(UNAME), FreeBSD)
	CC = clang++
else
	CC	= g++
	CFLAGS += -D LINUX
endif

$(FT_OBJS_D)/%.o:srcs/%.cpp
			@mkdir -p $(FT_OBJS_D)
			$(CC) -D FT=1 $(CFLAGS) $(INC) -o $@ -c $<

$(STD_OBJS_D)/%.o:srcs/%.cpp
			@mkdir -p $(STD_OBJS_D)
			$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(NAME):	$(STD_OBJ) $(FT_OBJ)
			@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
			$(CC) $(CFLAGS) $(FT_OBJ) -o ft_$(NAME)
			$(CC) $(CFLAGS) $(STD_OBJ) -o std_$(NAME)
			@printf "$(C_GREEN)DONE$(C_END)\n"

test:		all
			@printf "$(C_BLUE)Testing $(C_YELLOW)$(NAME)$(C_END)\n"
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@time ./std_$(NAME) && ./std_$(NAME) > std_log.txt
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@time ./ft_$(NAME) && ./ft_$(NAME) > ft_log.txt
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@printf "\n$(C_BLUE)Finished Test...$(C_END)\n"
			@$(MAKE) show
			@$(MAKE) fclean

all:		$(NAME)

clean:
			$(RM) $(STD_OBJS_D) $(FT_OBJS_D)
			@printf "$(C_RED)Cleaning objs$(C_END)\n"

fclean: 	clean
			$(RM) ft_$(NAME) std_$(NAME)
			@printf "$(C_RED)Deleted Everything$(C_END)\n"

re: fclean all

show:
	@printf "$(C_GREEN)"
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(C_YELLOW)ft_$(NAME), std_$(NAME)$(C_GREEN)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "INCLUDES	: $(INC) --> $(shell ls includes)\n"
	@printf "SRC		: $(C_YELLOW)$(SRC)$(C_GREEN)\n"
	@printf "STD_OBJ		: $(C_YELLOW)[$(STD_OBJS_D)] --> $(STD_OBJ)$(C_GREEN)\n"
	@printf "FT_OBJ		: $(C_YELLOW)[$(FT_OBJS_D)] --> $(FT_OBJ)$(C_END)\n"

.PHONY: all test re

C_BLACK		= \033[0;30m
C_RED		= \033[0;31m
C_GREEN		= \033[0;32m
C_YELLOW	= \033[0;33m
C_BLUE		= \033[0;34m
C_WHITE		= \033[0;37m
C_END		= \033[0m
