SRCS	= srcs/ft_putchar.c srcs/ft_strlen.c srcs/ft_putstr.c srcs/ft_strcmp.c srcs/ft_swap.c
OBJS	= ${SRCS:.c=.o}
INS	= includes
LNAME	= libft.a
LIB	= ar rc
RL	= ranlib
CC		= gcc
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o} -I${INS}

${LNAME}: ${OBJS}
	${LIB} ${LNAME} ${OBJS}
	${RL} ${LNAME}

all: ${LNAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${LNAME}

re: fclean all

check_norminette :
	norminette -R CheckForbiddenSourceHeader */*.c
	norminette -R CheckDefine */*.h

.PHONY: all re clean fclean .c.o check_norminette
