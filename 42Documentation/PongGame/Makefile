NAME    = pong
INC     = /usr/local/include
INCFT   = ./libft/include
LIBFT   = ./libft
LIBMLX  = /usr/local/lib
UNAME   := $(shell uname)
CFLAGS  = -Wall -Werror -Wextra -O3 -g -I$(INC) -I${INCFT} -Iinclude
LFLAGS  = -L$(LIBMLX) -lmlx
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:%.c=%.o)

ifeq ($(UNAME), Darwin)
	CC = gcc
	LFLAGS += -framework OpenGL -framework AppKit
else ifeq ($(UNAME), FreeBSD)
	CC = clang
else
	CC = gcc
	CFLAGS += -D LINUX
	LFLAGS += -lbsd -lXext -lX11 -lm
endif

all: $(NAME) show

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LFLAGS)

test: re
	@./$(NAME)

clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)

re: fclean all

show:
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(NAME)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "LFLAGS		: $(LFLAGS)\n"
	@printf "SRC		: $(SRC)\n"
	@printf "OBJ		: $(OBJ)\n"

PHONY: show fclean clean all
