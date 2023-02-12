SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)
NAME = exam06
CC = gcc
CFLAGS = -g -Werror -Wextra -Wall #-fsanitize=address
LEAKS_LOG = valgrind-out.txt
VALGRINDFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=$(LEAKS_LOG)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

leaks:
	-valgrind $(VALGRINDFLAGS) ./$(NAME) 8080

show:
	@echo "OBJ: " $(OBJ)
	@echo "SRC: " $(SRC)
	@echo "CFLAGS: " $(CFLAGS)
