NAME = some_spheres

CC = c++

CPPFLAGS = -g -Wall -Werror -Wextra -pedantic -O3 --std=c++17

INCLUDES = ../include

RM = rm -rf

IMAGE = ../some_spheres.ppm

all: ${NAME}

${NAME}: ${NAME}.cpp
	${CC} -o $@ ${CPPFLAGS} $< -I $(INCLUDES)
	@echo 'Created `${@}` executable'

test: re
	@./${NAME} > $(IMAGE)

fclean:
	@-${RM} ${NAME}

re: fclean all

.PHONY: re all clean fclean test
