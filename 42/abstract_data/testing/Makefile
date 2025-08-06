NAME = ft_test

CC = c++

CPPFLAGS = -g -O3 --std=c++98

SRC = main.cpp

INCLUDES = ../include

RM = rm -rf

all: ${NAME}

${NAME}: ${SRC}
	${CC} -o $@ ${CPPFLAGS} ${SRC} -I ${INCLUDES}
	@echo 'Created `${@}` executable'

test: re
	@./${NAME}

fclean:
	@-${RM} ${NAME}

re: fclean all

.PHONY: re all clean fclean test
