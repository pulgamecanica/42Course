# Makefile created automatically from script
# C++ Visualizer [Fri May 31 03:04:59 PM CEST 2024]
#MAKEFLAGS += --silent  # Silence makefile [Commented by default]
NAME            =   Visualizer

NAME_DEBUG      =   ${NAME}_debug

RAYLIB			= raylib

RAYLIB_PATH		= raylib/src	

RAYLIB_NAME		= raylib/src/lib$(RAYLIB).a

RAYLIB_INC_PATH = ${RAYLIB_PATH}

RAYGUI			= raygui

RAYGUI_PATH		= raygui/src

RAYGUI_NAME		= raygui/src/lib$(RAYGUI).a

RAYGUI_INC_PATH = $(RAYGUI_PATH)

INC_PATH        =   .

SRC_PATH        =   .

SRC             =   ./main.cpp

CFLAGS          =   -std=c++20 -w #-pedantic -fsanitize=address #-O3 -g -fsanitize=leak

INC             =   -I${INC_PATH} -I$(RAYLIB_INC_PATH) -I$(RAYGUI_INC_PATH)

LIBS			=  -L${RAYGUI_PATH} -l${RAYGUI} -L${RAYLIB_PATH} -l${RAYLIB} -lGL -lm -lpthread -ldl -lrt -lX11

OBJ_D           =   objs
OBJ_D_DEBUG     =   objs_debug

OBJ             =   $(SRC:${SRC_PATH}/%.cpp=${OBJ_D}/%.o)
OBJ_DEBUG       =   $(SRC:${SRC_PATH}/%.cpp=${OBJ_D_DEBUG}/%.o)

UNAME           :=  $(shell uname)

RM              =   rm -rf

ifeq ($(UNAME), Darwin)
    CC = c++
else ifeq ($(UNAME), FreeBSD)
    CC = clang++
else
    CC  = g++
    CFLAGS += -D LINUX
endif

all:      $(RAYLIB_NAME) $(RAYGUI_NAME) ${NAME}


${RAYLIB_NAME}:
			@make -C ${RAYLIB_PATH} PLATFORM=PLATFORM_DESKTOP

${RAYGUI_NAME}:
			cp $(RAYGUI_PATH)/raygui.h $(RAYGUI_PATH)/raygui.c
			gcc -c $(RAYGUI_PATH)/raygui.c -o $(RAYGUI_PATH)/raygui.o
			ar rcs $(RAYGUI_NAME) $(RAYGUI_PATH)/raygui.o

${OBJ_D}/%.o:${SRC_PATH}/%.cpp
			@mkdir -p ${OBJ_D}
			$(CC) $(CFLAGS) ${INC} -o $@ -c $<

${NAME}:   ${OBJ}
			@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
			$(CC) $(CFLAGS) $(OBJ) $(RAYGUI_PATH)/raygui.o $(LIBS) -o $(NAME)
			@printf "$(C_GREEN)DONE$(C_END)\n"

${OBJ_D_DEBUG}/%.o:${SRC_PATH}/%.cpp
			@mkdir -p ${OBJ_D_DEBUG}
			${CC} ${CFLAGS} -D DEBUG=1 ${INC} -o $@ -c $<

${NAME_DEBUG}: $(OBJ_DEBUG)
			@printf "Compiling DEBUG $(C_YELLOW)$(NAME_DEBUG)$(C_END) ...\n"
			$(CC) $(CFLAGS) $(OBJ_DEBUG) $(LIBS) -o $(NAME_DEBUG)
			@printf "$(C_GREEN)DONE$(C_END)\n"

debug:      ${NAME_DEBUG}

test:       all
			@printf "$(C_BLUE)Testing $(C_YELLOW)$(NAME)$(C_END)\n"
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@./$(NAME)
			@printf "\n$(C_BLUE)********************************************$(C_END)\n"
			@printf "\n$(C_BLUE)Finished Test...$(C_END)\n"
			@$(MAKE) show --no-print-directory

valgrind:   re
			valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(NAME) $(NAME)_ARGS
			@echo "[MAKEFILE] You can setup test arguments by setting up the env $(NAME)_ARGS"
			@echo "[MAKEFILE] Ex: export $(NAME)_ARGS=\"-a --recursive ..\""

clean:
			$(RM) $(OBJ_D)
			@printf "$(C_RED)Cleaning objs$(C_END)\n"

fclean:     clean
			$(RM) $(NAME)
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

.PHONY: all test re $(RAYGUI)

C_BLACK     = \033[0;30m
C_RED       = \033[0;31m
C_GREEN     = \033[0;32m
C_YELLOW    = \033[0;33m
C_BLUE      = \033[0;34m
C_WHITE     = \033[0;37m
C_END       = \033[0m

