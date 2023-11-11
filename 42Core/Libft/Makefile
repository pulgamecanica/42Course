SRCS	= ${wildcard src/*.c}
OBJS	= ${SRCS:.c=.o}
INS	= includes
NAME	= libft.a
LIB	= ar rc
RL	= ranlib
CC		= cc
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o} -I ${INS}

${NAME}: ${OBJS}
	${LIB} ${NAME} ${OBJS}
	${RL} ${NAME}

all: ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

norm :
	norminette -R CheckForbiddenSourceHeader *.c
	norminette -R CheckDefine *.h

.PHONY: all re clean fclean
