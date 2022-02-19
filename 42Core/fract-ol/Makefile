INC=/usr/include

INCFT=libft/includes

INCFTLIB=libft

INCLIB=$(INC)/../lib

UNAME := $(shell uname)

CFLAGS= -Wall -Werror -Wextra -I$(INC) -O3 -I.. -I${INCFT} -Iincludes -g

NAME= fract-ol
SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)

LFLAGS = -L.. -lmlx -L$(INCLIB) -lXext -lX11 -lm -L${INCFTLIB} -lft

ifeq ($(UNAME), Darwin)
	# mac
	CC = clang
	LFLAGS = -L.. -lmlx -L$(INCLIB) -framework OpenGL -framework AppKit -L${INCFTLIB} -lft
else ifeq ($(UNAME), FreeBSD)
	# FreeBSD
	CC = clang
else
	#Linux and others...
	CC	= gcc
	LFLAGS += -lbsd
endif

all: $(NAME)

$(NAME): runlibft $(OBJ)
	@$(CC) -o $(NAME) $(OBJ) $(LFLAGS)
	@echo "Finished compiling... --> ${NAME}"
	
show:
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(NAME)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "LFLAGS		: $(LFLAGS)\n"
	@printf "SRC		: $(SRC)\n"
	@printf "OBJ		: $(OBJ)\n"

runlibft:
	@make -C libft all --no-print-directory
	@make -f Makefile.msg msglibft --no-print-directory

norm:
	norminette -R CheckForbiddenSourceHeader ${SRC}
	norminette -R CheckDefine includes/fractol.h includes/config.h

clean:
	rm -f $(NAME) $(OBJ)

re: clean all
