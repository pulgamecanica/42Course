SRC	= srcs/main.c
BIN 	= bin
INCS	= includes/
LIBFT	= Libft
LIBFT_INCS = $(LIBFT)/includes
CFLAGS	= -Wall -Werror -Wextra -g
LFLAGS	= -I$(INCS) -I$(LIBFT_INCS) -L$(LIBFT) -lft
UNAME	:= $(shell uname)
NAME	= ft_ls
RM	= rm -rf
OBJS	= $(SRC:srcs/%c=$(BIN)/%o)

ifeq ($(UNAME), Darwin)
	CC = gcc
else ifeq ($(UNAME), FreeBSD)
	CC = clang
else
	CC	= gcc
	CFLAGS += -D LINUX
endif

$(NAME): ${OBJS}
	${CC} ${OBJS} ${LFLAGS} -o ${NAME}

all: LIBFT ${BIN} ${NAME}

$(BIN)/%o: srcs/%c
	${CC} -c $< ${CFLAGS} ${LFLAGS} -o $@

$(BIN):
	mkdir -p $(BIN)

clean:
	$(RM) $(OBJ) $(BIN)

fclean: clean
	$(RM) $(NAME)
	make fclean -C $(LIBFT) --no-print-directory

LIBFT:
	@make -C $(LIBFT) --no-print-directory

re: fclean all

show:
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(NAME)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "LFLAGS		: $(LFLAGS)\n"
	@printf "SRC		: $(SRC)\n"
	@printf "OBJS		: $(OBJS)\n"

