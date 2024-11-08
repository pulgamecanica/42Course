# Makefile created automatically from script
# C++ ft_game [Sun Oct 13 10:36:16 AM CEST 2024]
#MAKEFLAGS += --silent  # Silence makefile [Commented by default]
NAME            =   ft_game

NAME_DEBUG      =   ${NAME}_debug

INC_PATH        =   includes

SRC_PATH        =   srcs

SRC             =   srcs/main.c \
					srcs/image_utils.c \
					srcs/window_utils.c \

CFLAGS          =   -Wall -Wextra -Werror #-O3 -g -fsanitize=leak


LIBMLX  = /usr/local/lib

LFLAGS	= -L$(LIBMLX) -lmlx

LIBMLX_INC	= /usr/local/include

INC             =   -I${INC_PATH} -I$(LIBMLX_INC)

OBJ_D           =   objs
OBJ_D_DEBUG     =   objs_debug

OBJ             =   $(SRC:${SRC_PATH}/%.cpp=${OBJ_D}/%.o)
OBJ_DEBUG       =   $(SRC:${SRC_PATH}/%.cpp=${OBJ_D_DEBUG}/%.o)

UNAME           :=  $(shell uname)

RM              =   rm -rf

ifeq ($(UNAME), Darwin)
    CC = cc
	LFLAGS += -framework OpenGL -framework AppKit
else ifeq ($(UNAME), FreeBSD)
    CC = clang
	LFLAGS += -lbsd -lXext -lX11 -lm
else
    CC  = cc
    CFLAGS += -D LINUX
	LFLAGS += -lbsd -lXext -lX11 -lm
endif

all:        ${NAME}

${OBJ_D}/%.o:${SRC_PATH}/%.cpp
			@mkdir -p ${OBJ_D}
			$(CC) $(CFLAGS) ${INC} -o $@ -c $<

${NAME}:   ${OBJ}
			@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
			$(CC) ${INC} $(OBJ) -o $(NAME) ${LFLAGS}
			@printf "$(C_GREEN)DONE$(C_END)\n"

${OBJ_D_DEBUG}/%.o:${SRC_PATH}/%.cpp
			@mkdir -p ${OBJ_D_DEBUG}
			${CC} ${CFLAGS} -D DEBUG=1 ${INC} -o $@ -c $<

${NAME_DEBUG}: $(OBJ_DEBUG)
			@printf "Compiling DEBUG $(C_YELLOW)$(NAME_DEBUG)$(C_END) ...\n"
			$(CC) $(CFLAGS) ${INC} ${LFLAGS} $(OBJ_DEBUG) -o $(NAME_DEBUG)
			@printf "$(C_GREEN)DONE$(C_END)\n"

debug:      ${NAME_DEBUG}

test:       re
			@printf "$(C_BLUE)Testing $(C_YELLOW)$(NAME)$(C_END)\n"
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@./$(NAME)
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@printf "\n$(C_BLUE)Finished Test...$(C_END)\n"
			@$(MAKE) show --no-print-directory
			@$(MAKE) fclean --no-print-directory

valgrind:   re
			valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(NAME) $(NAME)_ARGS
			@echo "[MAKEFILE] You can setup test arguments by setting up the env $(NAME)_ARGS"
			@echo "[MAKEFILE] Ex: export $(NAME)_ARGS=\"-a --recursive ..\""

clean:
			$(RM) $(OBJ_D) $(OBJ_D_DEBUG)
			@printf "$(C_RED)Cleaning objs$(C_END)\n"

fclean:     clean
			$(RM) $(NAME) $(NAME_DEBUG)
			@printf "$(C_RED)Deleted Everything$(C_END)\n"

re: fclean all

show:
	@printf "$(C_GREEN)"
	@printf "UNAME     : $(UNAME)\n"
	@printf "NAME      : $(C_YELLOW)$(NAME)$(C_GREEN)\n"
	@printf "CC        : $(CC)\n"
	@printf "CFLAGS    : $(CFLAGS)\n"
	@printf "INCLUDES  : $(INC)\n"
	@printf "SRC       : $(C_YELLOW)$(SRC)$(C_GREEN)\n"
	@printf "OBJ       : $(C_YELLOW)[$(OBJ_D)] --> $(OBJ)$(C_END)\n"

.PHONY: all test re

C_BLACK     = \033[0;30m
C_RED       = \033[0;31m
C_GREEN     = \033[0;32m
C_YELLOW    = \033[0;33m
C_BLUE      = \033[0;34m
C_WHITE     = \033[0;37m
C_END       = \033[0m

