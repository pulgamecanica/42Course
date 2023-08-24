NAME			= ft_lst_performance
SRC				= $(wildcard srcs/*.c)
BIN 			= bin
INCS			= includes
LIBFT			= Libft
LIBFT_INCS	= $(LIBFT)/includes
CFLAGS		= -Wall -Werror -Wextra -g
LFLAGS		= -L$(LIBFT) -lft
IFLAGS		= -I$(INCS) -I$(LIBFT_INCS)
UNAME			:= $(shell uname)
RM				= rm -rf
OBJS			= $(SRC:srcs/%c=$(BIN)/%o)

ifeq ($(UNAME), Darwin)
	CC = gcc
else ifeq ($(UNAME), FreeBSD)
	CC = clang
else
	CC	= gcc
	CFLAGS += -D LINUX
endif

all: $(NAME)

$(NAME): ${BIN} ${OBJS} | ${LIBFT}
	${CC} ${OBJS} ${LFLAGS} -o ${NAME}

$(BIN)/%o: srcs/%c
	${CC} -c $< ${CFLAGS} ${IFLAGS} -o $@

$(BIN):
	@mkdir -p $(BIN)

clean:
	$(RM) $(BIN) $(DEBUGBIN)

fclean: clean
	$(RM) $(NAME) $(DNAME)
	@make fclean -C $(LIBFT) --no-print-directory

$(LIBFT):
	@make all -C $(LIBFT) --no-print-directory

re: fclean all

show:
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(NAME)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "LFLAGS		: $(LFLAGS)\n"
	@printf "SRC		: $(SRC)\n"
	@printf "OBJS		: $(OBJS)\n"


.PHONY: $(LIBFT) re all clean fclean debug test
