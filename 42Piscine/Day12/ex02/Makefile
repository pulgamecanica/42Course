SRCS	= ./srcs/utils.c ./srcs/display_file.c ./srcs/main.c ./srcs/utils2.c
NAME	= ft_tail
INCS	= includes
OBJS	= ${SRCS:.c=.o}
GCC	= gcc
RM	= rm -f
CFLAGS	= -g -Wall -Wextra -Werror

.c.o :
	${GCC} ${CFLAGS} -c $< -o ${<:.c=.o} -I${INCS}

${NAME} : ${OBJS}
	${GCC} ${CFLAGS} ${OBJS} -o ${NAME}

all: ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

norm:
	norminette srcs/*.c
	norminette -R CheckDefine includes/*

.PHONY: all clean fclean re .c.o test
