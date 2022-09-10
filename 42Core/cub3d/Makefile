NAME		= cub3D

MSG		= make.msg

INC		= /usr/include

INCFT		= libft/includes

INCFTLIB	= libft

# Commented for home dev (uncomment before commit)
# INCLIB		=$(INC)/../lib
INCLIB		= .

UNAME		:= $(shell uname)

CFLAGS		= -Wall -Werror -Wextra -I$(INC) -O3 -I${INCFT} -Iincludes -g

LFLAGS		= -lmlx -L$(INCLIB) -lXext -lX11 -lm -L${INCFTLIB} -lft

SRC		= $(wildcard src/*.c src/*/*.c)

OBJ		= $(SRC:%.c=%.o)

RM		= rm -f

ifeq ($(UNAME), Darwin)
	CC = gcc
	LFLAGS = -lmlx -L$(INCLIB) -framework OpenGL -framework AppKit -L${INCFTLIB} -lft
else ifeq ($(UNAME), FreeBSD)
	CC = clang
else
	CC	= gcc
	CFLAGS += -D LINUX
	LFLAGS += -lbsd
endif

all: $(NAME)

$(NAME): runlibft $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LFLAGS)

show:
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(NAME)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "LFLAGS		: $(LFLAGS)\n"
	@printf "SRC		: $(SRC)\n"
	@printf "OBJ		: $(OBJ)\n"

runlibft:
	@make -C libft --no-print-directory

norm:
	norminette -R CheckForbiddenSourceHeader ${SRC}
	norminette -R CheckDefine includes/fractol.h includes/config.h

info:
	@make -f $(MSG) msglibft --no-print-directory
	@make -f $(MSG) msgft_mlx --no-print-directory

clean:
	${RM} $(OBJ)

fclean:
	@make -C libft fclean --no-print-directory
	@make clean
	${RM} ${NAME}

run: all
	./cub3D tools/maps/map1.cub | cat -e  > trace.txt

re: fclean all info
