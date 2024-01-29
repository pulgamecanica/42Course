# Makefile created automatically from script\n
# C++ zelda [Fri Jan 19 09:15:51 AM CET 2024]\n
# MAKEFLAGS += --silent	# Silence makefile [Commented by default]\n
MLX	= mlx

MLX_SOURCE = https://github.com/codam-coding-college/MLX42

MLX_INC	= $(MLX)/include/MLX42

MLX_LIB	= mlx42

MLX_INC_LIB = mlx/build/

LIBFT	= libft

LIBFT_INC = $(LIBFT)/includes

LIBFT_LIB = ft

NAME	=	menu_tutorial

SRC	=	$(wildcard srcs/*.c)

CFLAGS	=	-g -Wall -Wextra -Werror -pedantic -fsanitize=address

LFLAGS	=	-L$(MLX_INC_LIB) -l$(MLX_LIB) -L$(LIBFT) -l$(LIBFT_LIB)

INC	=	-I includes -I $(MLX_INC) -I$(LIBFT_INC)

OBJS_D	=	objs

OBJ	=	$(SRC:srcs/%.c=$(OBJS_D)/%.o)

UNAME	:=	$(shell uname)

RM		= rm -rf

ifeq ($(UNAME), Darwin)
	CC = gcc
	LFLAGS += -framework OpenGL -framework AppKit
else ifeq ($(UNAME), FreeBSD)
	CC = clang++
else
	CC	= gcc
	CFLAGS += -D LINUX
	LFLAGS += -ldl -lglfw -pthread -lm

endif

all:            $(MLX) $(NAME)

$(OBJS_D)/%.o:srcs/%.c
			@mkdir -p $(OBJS_D)
			$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(NAME):	$(OBJ) | ${LIBFT}
			@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
			$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LFLAGS)
			@printf "$(C_GREEN)DONE$(C_END)\n"

test:		re
			@printf "$(C_BLUE)Testing $(C_YELLOW)$(NAME)$(C_END)\n"
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@./$(NAME)
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@printf "\n$(C_BLUE)Finished Test...$(C_END)\n"
			@$(MAKE) show
			@$(MAKE) fclean

$(MLX):
	git clone $(MLX_SOURCE) $@
	cd $@; cmake -B build;
	make -C $@/build

$(LIBFT):
	@make all -C $(LIBFT) --no-print-directory

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
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "INCLUDES	: $(INC)\n"
	@printf "SRC		: $(C_YELLOW)$(SRC)$(C_GREEN)\n"
	@printf "OBJ		: $(C_YELLOW)[$(OBJS_D)] --> $(OBJ)$(C_END)\n"

.PHONY: all test re $(LIBFT)

C_BLACK		= \033[0;30m
C_RED		= \033[0;31m
C_GREEN		= \033[0;32m
C_YELLOW	= \033[0;33m
C_BLUE		= \033[0;34m
C_WHITE		= \033[0;37m
C_END		= \033[0m
