# Makefile created automatically from script
# C++ cpp_on_rails [Tue Sep  3 04:18:39 AM CEST 2024]
#MAKEFLAGS += --silent  # Silence makefile [Commented by default]
RAYLIB		= libraylib.a

RAYLIB_PATH = raylib/src

RAYGUI = libraygui.a

RAYGUI_PATH = raygui/src

NAME            =   cpp_on_rails

NAME_DEBUG      =   ${NAME}_debug

INC_PATH        =   .

SRC_PATH        =   .

SRC             =   ./main.cpp \
		./Visualizer/SimulationGrid.cpp \
		./Visualizer/SimulationPanelItem.cpp \
		./Visualizer/VerticalScrollPanel.cpp \
		./Visualizer/MinimapGrid.cpp \
		./Visualizer/EditableGrid.cpp \
		./Visualizer/Grid.cpp \
		./utils/Animation.cpp \
		./utils/ButtonManager.cpp \
		./Visualizer/SimulationsState.cpp \
		./Visualizer/SettingsState.cpp \
		./Visualizer/SchedulesState.cpp \
		./Visualizer/NetworkState.cpp \
		./Visualizer/MenuState.cpp \
		./RailwaySystem/Subject.cpp \
		./SimulationsEngine.cpp \
		./utils/FileLogger.cpp \
		./utils/Settings.cpp \
		./utils/ErrorHandler.cpp \
		./RailwaySystem/RailwaySystem.cpp \
		./RailwaySystem/Simulation.cpp \
		./RailwaySystem/SimulationsManager.cpp \
		./RailwaySystem/EventMediator.cpp \
		./RailwaySystem/TrainsTimeMediator.cpp \
		./RailwaySystem/CollisionMediator.cpp \
		./RailwayElements/Schedule.cpp \
		./RailwayElements/Event.cpp \
		./RailwayElements/Train.cpp \
		./RailwayElements/Rail.cpp \
		./RailwayElements/Node.cpp \
		./utils/Parser.cpp \

CFLAGS          =   -Wall -Werror -Wextra -std=c++20 -O3 #-fsanitize=address -O3 -g -fsanitize=leak

INC             =   -I${INC_PATH} -IRailwaySystem -IRailwayElements -Iutils -IVisualizer -IDijkstra # -Iraylib/src -Iraygui/src

LDFLAGS			=	-L. -lraylib -lraygui

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

all: $(RAYGUI) $(RAYLIB) $(NAME)

$(RAYLIB):
	make -C $(RAYLIB_PATH) PLATFORM=PLATFORM_DESKTOP 
	mv raylib/src/$(RAYLIB) .

$(RAYGUI):
	cp $(RAYGUI_PATH)/raygui.h $(RAYGUI_PATH)/raygui.c
	gcc -c $(RAYGUI_PATH)/raygui.c -o $(RAYGUI_PATH)/raygui.o -Iraylib/src
	ar rcs $(RAYGUI) $(RAYGUI_PATH)/raygui.o

${OBJ_D}/%.o:${SRC_PATH}/%.cpp
			@mkdir -p ${OBJ_D} ${OBJ_D}/RailwaySystem ${OBJ_D}/RailwayElements ${OBJ_D}/utils $(OBJ_D)/Visualizer
			$(CC) -o $@ $(CFLAGS) -c $< ${INC}

${NAME}:   ${OBJ}
			@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
			$(CC) $(CFLAGS) ${INC} $(OBJ) -o $(NAME) $(LDFLAGS)
			@printf "$(C_GREEN)DONE$(C_END)\n"

${OBJ_D_DEBUG}/%.o:${SRC_PATH}/%.cpp
			@mkdir -p ${OBJ_D_DEBUG}
			${CC} ${CFLAGS} -D DEBUG=1 ${INC} -o $@ -c $<

${NAME_DEBUG}: $(OBJ_DEBUG)
			@printf "Compiling DEBUG $(C_YELLOW)$(NAME_DEBUG)$(C_END) ...\n"
			$(CC) $(CFLAGS) ${INC} $(OBJ_DEBUG) -o $(NAME_DEBUG)
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
			$(RM) $(NAME) SimulationsLog_*
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

