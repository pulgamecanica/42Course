BIN 	= bin
SRC		= src/main.c src/image_utils.c src/window_utils.c src/sprite.c src/game.c src/animation.c src/utils.c src/parse_sprite.c
INCS	= include/
LIBFT_INCS = $(LIBFT)/includes
LIBMLX_INCS	= /usr/local/include
LIBFT	= libft
LIBMLX  = /usr/local/lib
CFLAGS	= -Wall -Werror -Wextra -g -O3 
LFLAGS	= -L$(LIBFT) -lft -L$(LIBMLX) -lmlx
IFLAGS	= -I$(INCS) -I$(LIBFT_INCS) -I$(LIBMLX_INCS) 
UNAME	:= $(shell uname)
NAME	= animations
RM		= rm -rf
OBJS	= $(SRC:src/%c=$(BIN)/%o)

ifeq ($(UNAME), Darwin)
	CC = gcc
	LFLAGS += -framework OpenGL -framework AppKit
else ifeq ($(UNAME), FreeBSD)
	CC = clang
	LFLAGS += -lbsd -lXext -lX11 -lm
else
	CC	= gcc
	CFLAGS += -D LINUX
	LFLAGS += -lbsd -lXext -lX11 -lm
endif

all: $(NAME)

$(NAME): ${BIN} ${OBJS} | ${LIBFT}
	${CC} ${OBJS} ${LFLAGS} -o ${NAME} 

$(BIN)/%o: src/%c
	${CC} -c $< ${CFLAGS} ${IFLAGS} ${LFLAGS} -o $@

$(BIN):
	mkdir -p $(BIN)

$(LIBFT):
	@make all -C $(LIBFT) --no-print-directory

clean:
	$(RM) $(BIN)

fclean: clean
	$(RM) $(NAME)
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


.PHONY: $(LIBFT) re all clean fclean
