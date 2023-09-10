NAME			= ft_ls
DNAME			= debug_ft_ls
SRC				= $(wildcard srcs/*.c)
BIN 			= bin
DEBUGBIN	= dbin
INCS			= includes
LIBFT			= Libft
LIBFT_INCS	= $(LIBFT)/includes
CFLAGS		= -Wall -Werror -Wextra -g
DEBUG			=	-D DEBUG
LFLAGS		= -L$(LIBFT) -lft
IFLAGS		= -I$(INCS) -I$(LIBFT_INCS)
UNAME			:= $(shell uname)
RM				= rm -rf
OBJS			= $(SRC:srcs/%c=$(BIN)/%o)
DOBJS			= $(SRC:srcs/%c=$(DEBUGBIN)/%o)
MSG_EXEC	= msg_ft_ls

ifeq ($(UNAME), Darwin)
	CC = gcc
else ifeq ($(UNAME), FreeBSD)
	CC = clang
else
	CC	= gcc
	CFLAGS += -D LINUX
endif

all: $(NAME)
	@./$(MSG_EXEC)

$(NAME): ${BIN} ${OBJS} | ${LIBFT}
	${CC} ${OBJS} ${LFLAGS} -o ${NAME}

$(BIN)/%o: srcs/%c ${INCS}/ft_ls.h
	${CC} -c $< ${CFLAGS} ${IFLAGS} -o $@

$(DEBUGBIN)/%o: srcs/%c ${INCS}/ft_ls.h
	${CC} -c $< ${CFLAGS} ${IFLAGS} $(DEBUG) -o $@

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

debug: $(DNAME)

$(DNAME): $(DEBUGBIN) $(DOBJS) | $(LIBFT)
	${CC} ${DOBJS} ${LFLAGS} $(DEBUG) -o ${DNAME}

test: debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(DNAME) $(FT_LS_ARGS)
	@echo "[MAKEFILE] You can setup test arguments by setting up the env FT_LS_ARGS"
	@echo "[MAKEFILE] Ex: export FT_LS_ARGS=\"-a --recursive ..\""

show:
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(NAME)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "LFLAGS		: $(LFLAGS)\n"
	@printf "SRC		: $(SRC)\n"
	@printf "OBJS		: $(OBJS)\n"


.PHONY: $(LIBFT) re all clean fclean debug test