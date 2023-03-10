SHOW	= 0
SRCS	= ${wildcard *.c}
PSSRCS	= ../errors.c ../pmoves.c ../rmoves.c ../rrmoves.c ../smoves.c ../stack.c ../init_stack.c ../stack_max_min.c
NAME	= checker
CC	= gcc
FLAGS	= -w -D SHOW=${SHOW}
RM	= rm -f
INCS	= ../libft/includes
LIBFT	= -L ../libft -lft

${NAME}: runlibft
	${CC} ${FLAGS} -I ${INCS} ${SRCS} ${LIBFT} ${PSSRCS} -o ${NAME}

show: runlibft
	${CC} -Wall -Werror -Wextra -D SHOW=1 -I ${INCS} ${LIBFT} ${SRCS} ${PSSRCS} -o ${NAME}

all: ${NAME}

clean:
	${RM} ${NAME}

fclean: clean
	make -C .. fclean

re: fclean all

runlibft:
	make -C ../libft

norm:
	norminette -R CheckForbiddenSourceHeader ${SRCS}
	norminette -R CheckDefine ${INCS}

.PHONY: all re clean fclean
