# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmachado <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/17 15:15:37 by bmachado          #+#    #+#              #
#    Updated: 2022/02/23 14:20:42 by bmachado         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC = 	./ft_atoi.c 		\
 		./ft_bzero.c 		\
 		./ft_isalnum.c 		\
 		./ft_isalpha.c 		\
 		./ft_isascii.c 		\
 		./ft_isdigit.c 		\
 		./ft_isprint.c 		\
 		./ft_itoa.c 		\
 		./ft_memccpy.c 		\
 		./ft_memcpy.c 		\
 		./ft_memmove.c 		\
 		./ft_memset.c 		\
 		./ft_putchar_fd.c 	\
 		./ft_putendl_fd.c 	\
 		./ft_putnbr_fd.c 	\
 		./ft_putstr_fd.c 	\
 		./ft_strchr.c 		\
 		./ft_strdup.c 		\
 		./ft_strjoin.c 		\
 		./ft_strlcat.c 		\
 		./ft_strlen.c 		\
 		./ft_strmapi.c 		\
 		./ft_strncmp.c 		\
 		./ft_strnstr.c 		\
 		./ft_strrchr.c 		\
 		./ft_split.c 		\
 		./ft_substr.c 		\
 		./ft_strtrim.c 		\
 		./ft_tolower.c 		\
 		./ft_toupper.c 		\
		./ft_calloc.c		\
		./ft_strlcpy.c		\
		./ft_memchr.c		\
		./ft_memcmp.c		\
		./get_next_line.c	\
		./ft_lstadd_back.c	\
		./ft_lstadd_front.c	\
		./ft_lstclear.c		\
		./ft_lstdelone.c	\
		./ft_lstiter.c		\
		./ft_lstlast.c		\
		./ft_lstmap.c		\
		./ft_lstnew.c		\
		./ft_lstsize.c		\
		./ft_strcpy.c		\
		./ft_strcmp.c		\
		./ft_isfilled.c		\
		./ft_subarray.c		\
		./ft_arrlen.c		\

STYLE='\033[3m'
GREEN='\033[32m'
YELLOW='\033[33m'
NONE='\033[0m'
GRAY='\033[2;37m'

OBJS = $(SRC:.c=.o)

HDRS = ./

all: $(NAME)

$(NAME):
	@echo $(STYLE)$(YELLOW) ">> Compiling libft <<" $(GRAY)
	gcc -c -Wall -Wextra -Werror -I $(HDRS) $(SRC) 
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)
	@echo $(NONE)$(STYLE)$(GREEN) "<< Libft compiled >>" $(NONE)

clean:
	@/bin/rm -f $(OBJS)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all
