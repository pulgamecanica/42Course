# Makefile for CppOnRails

# Project name
NAME := CppOnRails
NAME_DEBUG := ${NAME}_debug

# Directories
INC_DIR := includes
SRC_DIR := srcs
OBJ_DIR := objs
OBJ_DIR_DEBUG := objs_debug
LIB_DIR := Dijkstra
LIB_NAME := libDijkstraGraph.a

# Compiler and flags
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    CXX := c++
else ifeq ($(UNAME), FreeBSD)
    CXX := clang++
else
    CXX := g++
    CXXFLAGS += -D LINUX
endif

CXXFLAGS := -Wall -Wextra -Werror -std=c++23 -pedantic -g -fsanitize=address
INC := -I${INC_DIR} -I${LIB_DIR}
LDFLAGS := -L${LIB_DIR} -lDijkstraGraph
RM := rm -rf
# Colors
C_BLACK := \033[0;30m
C_RED := \033[0;31m
C_GREEN := \033[0;32m
C_YELLOW := \033[0;33m
C_BLUE := \033[0;34m
C_WHITE := \033[0;37m
C_END := \033[0m

# Source files
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ_DEBUG := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR_DEBUG)/%.o)

# Default target
all: $(LIB_DIR)/$(LIB_NAME) $(NAME)

# Rule to build the release object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -o $@ -c $<

# Rule to build the release executable
$(NAME): $(OBJ)
	@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
	$(CXX) $(CXXFLAGS) $(INC) $(OBJ) $(LDFLAGS) -o $(NAME)
	@printf "$(C_GREEN)DONE$(C_END)\n"

# Rule to build the debug object files
$(OBJ_DIR_DEBUG)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR_DEBUG)
	$(CXX) $(CXXFLAGS) -D DEBUG=1 $(INC) -o $@ -c $<

# Rule to build the debug executable
$(NAME_DEBUG): $(OBJ_DEBUG)
	@printf "Compiling DEBUG $(C_YELLOW)$(NAME_DEBUG)$(C_END) ...\n"
	$(CXX) $(CXXFLAGS) $(INC) $(OBJ_DEBUG) $(LDFLAGS) -o $(NAME_DEBUG)
	@printf "$(C_GREEN)DONE$(C_END)\n"

# Debug target
debug: $(NAME_DEBUG)

# Build the library in the Dijkstra directory
$(LIB_DIR)/$(LIB_NAME):
	@printf "Building library $(C_YELLOW)$(LIB_NAME)$(C_END) ... \n"
	$(MAKE) -C $(LIB_DIR)
	@printf "$(C_GREEN)DONE$(C_END)\n"

# Test target
test: all
	@printf "$(C_BLUE)Testing $(C_YELLOW)$(NAME)$(C_END)\n"
	@printf "\n$(C_BLUE)********************************************$(C_END)\n"
	@./$(NAME)
	@printf "\n$(C_BLUE)********************************************$(C_END)\n"
	@printf "\n$(C_BLUE)Finished Test...$(C_END)\n"
	@$(MAKE) show --no-print-directory

# Valgrind target
valgrind: re
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(NAME) $(NAME)_ARGS
	@echo "[MAKEFILE] You can set up test arguments by setting up the env $(NAME)_ARGS"
	@echo "[MAKEFILE] Ex: export $(NAME)_ARGS=\"-a --recursive ..\""

# Clean object files
clean:
	$(RM) $(OBJ_DIR) $(OBJ_DIR_DEBUG)
	@printf "$(C_RED)Cleaning objs$(C_END)\n"

# Clean all generated files and library
fclean: clean
	$(RM) $(NAME) $(NAME_DEBUG)
	$(MAKE) -C $(LIB_DIR) clean
	@printf "$(C_RED)Deleted Everything$(C_END)\n"

# Rebuild target
re: fclean all

# Show build information
show:
	@printf "$(C_GREEN)"
	@printf "UNAME     : $(UNAME)\n"
	@printf "NAME      : $(C_YELLOW)$(NAME)$(C_GREEN)\n"
	@printf "CXX       : $(CXX)\n"
	@printf "CXXFLAGS  : $(CXXFLAGS)\n"
	@printf "INCLUDES  : $(INC)\n"
	@printf "SRC       : $(C_YELLOW)$(SRC)$(C_GREEN)\n"
	@printf "OBJ       : $(C_YELLOW)[$(OBJ_DIR)] --> $(OBJ)$(C_END)\n"

.PHONY: all test re show clean fclean debug valgrind

# Include dependency files if they exist
-include $(OBJ:.o=.d)
-include $(OBJ_DEBUG:.o=.d)
