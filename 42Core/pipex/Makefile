BSRCS	= pipex_bonus.c utils_bonus.c
SRCS	= pipex.c
NAME	= pipex
CC	= gcc
FLAGS	= -Wall -Werror -Wextra -fsanitize=address
RM	= rm -f
INCS	= libft/includes
LIBFT	= -Llibft -lft

all: ${NAME}

${NAME}: runlibft norm
	${CC} ${FLAGS} -o ${NAME} ${SRCS} -I ${INCS} ${LIBFT}

ignore: runlibft
	${CC} ${FLAGS} -o ${NAME} ${SRCS} -I ${INCS} ${LIBFT}

bonus: runlibft norm
	${CC} ${FLAGS} -o ${NAME} ${BSRCS} -I ${INCS} ${LIBFT}

clean:
	${RM} ${NAME}

fclean: clean
	make -C libft fclean

re: fclean all

runlibft:
	make -C libft

norm:
	norminette -R CheckForbiddenSourceHeader ${SRCS} ${BSRCS}
	norminette -R CheckDefine ${INCS} pipex.h pipex_bonus.h

.PHONY: all re clean fclean
