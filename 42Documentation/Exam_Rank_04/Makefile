SRC	= main.c

NAME	= ms

CC	= gcc

RM	= rm -f

CFLAGS	= -g -I$(IDIR) -Wall -Werror -Wextra

LFLAGS	=

OBJ	= $(SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LFLAGS)

check:
	$(CC) -o $(NAME) $(SRC) -D CHECK $(LFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean
