NAME	= animation_test
BIN 	= bin
SRC	= main.c
OBJS	= $(SRC:src/%c=$(BIN)/%o)

LIBFT	= ../libft
LIBFT_INCS = $(LIBFT)/includes

LIB_MLX_ANIMATIONS	= ..
LIBFT_MLX_ANIMATIONS_INCS = $(LIB_MLX_ANIMATIONS)/includes

LIBMLX  = /usr/local/lib
LIB_MLX_INCS	= /usr/local/include

CFLAGS	= -Wall -Werror -Wextra -g -O3
LFLAGS	= -L$(LIB_MLX_ANIMATIONS) -lmlx_animations -L$(LIBFT) -lft -L$(LIBMLX) -lmlx
IFLAGS	= -I$(LIBFT_MLX_ANIMATIONS_INCS) -I$(LIBFT_INCS) -I$(LIB_MLX_INCS)

UNAME	:= $(shell uname)

RM	= rm -rf

ifeq ($(UNAME), Darwin)
	CC = gcc
	LFLAGS += -framework OpenGL -framework AppKit
else ifeq ($(UNAME), FreeBSD)
	CC = clang
	LFLAGS += -lbsd -lXext -lX11 -lm
else
	CC	= cc
	CFLAGS += -D LINUX
	LFLAGS += -lbsd -lXext -lX11 -lm
endif

all: $(NAME)

$(NAME): ${BIN} ${OBJS} | ${LIB_MLX_ANIMATIONS}
	${CC} ${OBJS} ${IFLAGS} -g ${LFLAGS} -o ${NAME}

$(BIN)/%o: src/%c
	${CC} -c $< ${CFLAGS} ${IFLAGS} -o $@

$(BIN):
	mkdir -p $(BIN)

$(LIB_MLX_ANIMATIONS):
	@make all -C $(LIB_MLX_ANIMATIONS) --no-print-directory

clean:
	$(RM) $(BIN)

fclean: clean
	$(RM) $(NAME)
	@make fclean -C $(LIB_MLX_ANIMATIONS) --no-print-directory
	@make fclean -C $(LIBFT) --no-print-directory

re: fclean all

test:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(NAME) $(ARGS)

show:
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(NAME)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "LFLAGS		: $(LFLAGS)\n"
	@printf "IFLAGS		: $(IFLAGS)\n"
	@printf "SRC		: $(SRC)\n"
	@printf "OBJS		: $(OBJS)\n"


.PHONY: $(LIB_MLX_ANIMATIONS) re all clean fclean
