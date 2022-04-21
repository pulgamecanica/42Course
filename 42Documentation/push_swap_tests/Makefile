SRCS	= ${wildcard *.c}
NAME	= push_swap
CC	= gcc
FLAGS	= -Wall -Werror -Wextra -fsanitize=address -D SHOW=${SHOW}
RM	= rm -f
INCS	= libft/includes
LIBFT	= -Llibft -lft
GEN	= `ruby -e "puts (-500..500).to_a.shuffle.first(100).join(' ')"`
CHECK	= ./tests/checker_OS

${NAME}: runlibft
	${CC} ${FLAGS} -I ${INCS} ${LIBFT} ${SRCS} -o ${NAME}

all: ${NAME}

clean:
	${RM} ${NAME}

fclean: clean
	make -C libft fclean

re: fclean all

runlibft:
	make -C libft

run:
	./push_swap ${GEN}

run_checker:
	ARG=${GEN}; ./push_swap $$ARG | ${CHECK} $$ARG 

norm:
	norminette -R CheckForbiddenSourceHeader ${SRCS}
	norminette -R CheckDefine ${INCS}

.PHONY: all re clean fclean
