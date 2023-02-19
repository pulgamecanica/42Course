SRC	= srcs/main.c srcs/utils.c srcs/file.c srcs/flags.c
BIN 	= bin
DEBUGBIN = dbin
INCS	= includes/
LIBFT	= Libft
LIBFT_INCS = $(LIBFT)/includes
CFLAGS	= -Wall -Werror -Wextra -g
DEBUG	=	-D DEBUG
LFLAGS	= -I$(INCS) -I$(LIBFT_INCS) -L$(LIBFT) -lft
UNAME	:= $(shell uname)
NAME	= ft_ls
DNAME = debug_ft_ls
RM	= rm -rf
OBJS	= $(SRC:srcs/%c=$(BIN)/%o)
DOBJS = $(SRC:srcs/%c=$(DEBUGBIN)/%o)

ifeq ($(UNAME), Darwin)
	CC = gcc
else ifeq ($(UNAME), FreeBSD)
	CC = clang
else
	CC	= gcc
	CFLAGS += -D LINUX
endif

all: $(NAME)
	@./msg_ft_ls

$(NAME): ${BIN} ${OBJS} | ${LIBFT}
	${CC} ${OBJS} ${LFLAGS} -o ${NAME}

$(BIN)/%o: srcs/%c
	${CC} -c $< ${CFLAGS} ${LFLAGS} -o $@

$(DEBUGBIN)/%o: srcs/%c
	${CC} -c $< ${CFLAGS} ${LFLAGS} $(DEBUG) -o $@

$(BIN):
	@mkdir -p $(BIN)

$(DEBUGBIN):
	@mkdir -p $(DEBUGBIN)

clean:
	$(RM) $(BIN) $(DEBUGBIN)

fclean: clean
	$(RM) $(NAME) $(DNAME)
	@make fclean -C $(LIBFT) --no-print-directory

$(LIBFT):
	@make all -C $(LIBFT) --no-print-directory

re: fclean all

debug: $(DEBUGBIN) $(DOBJS) | $(LIBFT)
	${CC} ${DOBJS} ${LFLAGS} $(DEBUG) -o ${DNAME}

$(DNAME): debug

test:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(NAME)

show:
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(NAME)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "LFLAGS		: $(LFLAGS)\n"
	@printf "SRC		: $(SRC)\n"
	@printf "OBJS		: $(OBJS)\n"


.PHONY: $(LIBFT) re all clean fclean debug
