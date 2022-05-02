NAME    = tutorial#
INC     = /usr/local/include#						# could be /usr/include depending on your OS
INCFT   = ./libft/include#						# header from libft
LIBFT   = ./libft#							# path to libft library
LIBMLX  = /usr/local/lib#						# could be /usr/lib or minilibx_opengl_20191021/, depends on where you decided to put your mlx library 
UNAME   := $(shell uname)#						# UNAME will get the OS name, this will help define behaviors for certain OS's
CFLAGS  = -Wall -Werror -Wextra -O3 -g -I$(INC) -I${INCFT} -Iinclude#	# C Flags (gcc) & linking. "-Iinclude" if you created the folder "include" to put your project headers"
LFLAGS  = -L$(LIBMLX) -lmlx# -L${LIBFT} -lft#				# if you decided to install libmlx.a locally you don't need "-L$(LIBMLX) -lmlx" the school also has it locally as well...
SRC     = $(wildcard src/*.c)#						# list your source files
OBJ     = $(SRC:%.c=%.o)#						# convert source files to binary list

ifeq ($(UNAME), Darwin) # iMac / iOS
	CC = gcc
	LFLAGS += -framework OpenGL -framework AppKit
else ifeq ($(UNAME), FreeBSD) # FreeBSD
	CC = clang
else #Linux and others...
	CC = clang
	LFLAGS += -lbsd -lXext -lX11 -lm
endif

all: $(NAME)

$(NAME): $(OBJ) show
	$(CC) -o $(NAME) $(OBJ) $(LFLAGS)

clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)

re: fclean all

test: re
	clear
	@./$(NAME)

show:
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(NAME)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "LFLAGS		: $(LFLAGS)\n"
	@printf "SRC		: $(SRC)\n"
	@printf "OBJ		: $(OBJ)\n"

PHONY: show fclean clean all
