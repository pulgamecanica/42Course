# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmachado <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/17 15:15:37 by bmachado          #+#    #+#              #
#    Updated: 2022/02/23 15:51:41 by bmachado         ###   ########.fr        #
#    Updated: 2021/10/11 17:25:10 by bmachado         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LEAK = -fsanitize=address -g

CFLAGS = -Wall -Wextra -Werror `pkg-config readline --cflags`

YELLOW='\033[33m'
NONE='\033[0m'
GREEN='\033[32m'
GRAY='\033[2;37m'
STYLE='\033[3m'

NAME = minishell

SRC = 	./minishell.c			\
		./create_locals.c		\
		./local_utils.c			\
		./utils.c				\
		./utils2.c				\
		./utils3.c				\
		./env_table.c			\
		./cmds.c				\
		./export_utils.c		\
		./exec.c				\
		./env_utils.c			\
		./sorted_env.c			\
		./expand_utils.c		\
		./token.c				\
		./tokens_utils.c		\
		./token_utils2.c		\
		./token_utils3.c		\
		./expand_variables.c	\
		./cmd_lst.c				\
		./cmd_process.c			\
		./create_pipe.c			\
		./redirections.c		\
		./quotes.c				\
		./quotes_utils.c		\
		./signal.c				\
		./export.c				\
		./unset.c				\
		./error.c				\
		./here_doc.c			\
		./token_splits.c		\
		./piped_cmds.c			\
		./redirections_utils.c	\
		./redirection_utils2.c	\
		./here_doc_utils.c		\
		./term.c				\
		./free.c				\
		./terminal_path.c		\


OBJS = $(SRC:.c=.o)

LINKS = -L./libft -lft `pkg-config readline --libs`
INCLUDE = -I./includes -I./libft `pkg-config readline --cflags`

all: $(NAME)

$(NAME):
	@echo $(STYLE)$(YELLOW) ">> Passing Norminette  <<"$(GRAY)
	@norminette .
	@echo $(NONE)$(STYLE)$(GREEN) "<< Norminette passed >>" $(NONE)
	@make -C ./libft
	@echo $(STYLE)$(YELLOW) ">> Compiling $(NAME) <<"$(GRAY)
	gcc $(CFLAGS) $(SRC) -o $(NAME) $(LINKS)
	gcc $(CFLAGS) -c $(SRC)
	@echo $(NONE)$(STYLE)$(GREEN) "<< $(NAME) compiled >>" $(NONE)

clean:
	@echo $(STYLE)$(YELLOW) ">> Deleting files <<"$(NONE)
	@/bin/rm -f $(OBJS) ./libft/libft.a
	@make clean -C ./libft
	@echo $(STYLE)$(GREEN) "<< Files deleted >>"$(NONE)

fclean: clean
	@/bin/rm -f $(NAME)
	@make clean -C ./libft

re:
	@echo $(STYLE)$(YELLOW) ">> Recompiling $(NAME) <<" $(NONE)
	@make fclean all
