NAME = libasm.a

SRCS = 	ft_strlen.s \
				ft_strcpy.s \
				ft_strcmp.s \
				ft_write.s \
				ft_read.s \
				ft_strdup.s \

OBJS = ${SRCS:%.s=%.o}

AS = nasm

CC = gcc

CXX = c++

ASFLAGS = -felf64

CFLAGS = -Wall -Werror -Wextra -g

CXXFLAGS = -Wall -Werror -Wextra -g

AR = ar

RANLIB = ranlib

ARFLAGS = rcs

LDFLAGS = -L. -lasm

RM = rm -rf

RESET 	= "\033[0m"

YELLOW	= "\033[0;33m"

all: $(NAME) test_c test_cpp

%.o: %.s
	$(AS) $(ASFLAGS) -o $(@) $(<) 

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $(@) $(OBJS)
	$(RANLIB) $(NAME)

test_c: test_lib.c
	$(CC) $(CFLAGS) -o test_c test_lib.c $(LDFLAGS)

test_cpp: test_lib.cpp
	$(CXX) $(CXXFLAGS) -o test_cpp test_lib.cpp $(LDFLAGS)

test: all test_c test_cpp
	@echo -e $(YELLOW)"Testing library with C"$(RESET)
	-@ ./test_c
	@echo -e $(YELLOW)"\nTesting library with C++"$(RESET)
	-@ ./test_cpp

clean:
	$(RM) $(OBJS) test_c test_cpp

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all