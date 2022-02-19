NUM	= 4
SLEEP	= 200
EAT	= 200
DIE	= 410
TIMES	= -1;

INC	= includes/

UNAME	:= $(shell uname)

NAME	= philo

SRC	= $(wildcard src/*.c)

OBJ	= $(SRC:%.c=%.o)

CFLAGS	= -Wall -Werror -Wextra -O3 -I$(INC) -g

RM	= rm -f

ifeq ($(UNAME), Darwin)
	CC	= clang
else ifeq ($(UNAME), FreeBSD)
	CC	= clang
else
	CC	= gcc
endif

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) -o $(NAME) $(OBJ) -lpthread

clean:
	$(RM) $(OBJ)

re: clean all

run:
	./$(NAME) $(NUM) $(DIE) $(EAT) $(SLEEP) $(TIMES)

show:
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(NAME)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "SRC		: $(SRC)\n"
	@printf "OBJ		: $(OBJ)\n"


.PHONY:	all clean re show


