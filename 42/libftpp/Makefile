NAME            =   libftpp

INC_PATH        =   include

SRC_PATH        =   src

SRC             =   src/main.cpp \
		src/Obj.cpp \

CXXFLAGS          =   -Wall -Wextra -Werror -std=c++17 -pedantic -fsanitize=address #-O3 -g -fsanitize=leak

INC             =   -I${INC_PATH}

OBJ_D           =   objs

OBJ             =   $(SRC:${SRC_PATH}/%.cpp=${OBJ_D}/%.o)

UNAME           :=  $(shell uname)

RM              =   rm -rf

ifeq ($(UNAME), Darwin)
    CXX = c++
else ifeq ($(UNAME), FreeBSD)
    CXX = clang++
else
    CXX  = g++
    CXXFLAGS += -D LINUX
endif

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = .
BUILDDIR      = _build

all:        ${NAME}

${OBJ_D}/%.o:${SRC_PATH}/%.cpp
			@mkdir -p ${OBJ_D}
			$(CXX) $(CFLAGS) ${INC} -o $@ -c $<

${NAME}:   ${OBJ}
			@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
			$(CXX) $(CFLAGS) ${INC} $(OBJ) -o $(NAME)
			@printf "$(C_GREEN)DONE$(C_END)\n"

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
			$(RM) $(OBJ_D)
			@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
			@printf "$(C_RED)Cleaning objs$(C_END)\n"

fclean:     clean
			$(RM) $(NAME) xml
			@printf "$(C_RED)Deleted Everything$(C_END)\n"

re: fclean all

show:
	@printf "$(C_GREEN)"
	@printf "UNAME     : $(UNAME)\n"
	@printf "NAME      : $(C_YELLOW)$(NAME)$(C_GREEN)\n"
	@printf "CXX       : $(CXX)\n"
	@printf "CFLAGS    : $(CFLAGS)\n"
	@printf "INCLUDES  : $(INC)\n"
	@printf "SRC       : $(C_YELLOW)$(SRC)$(C_GREEN)\n"
	@printf "OBJ       : $(C_YELLOW) $(OBJ)$(C_END)\n"

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile all test re

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@-doxygen Doxyfile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

C_BLACK     = \033[0;30m
C_RED       = \033[0;31m
C_GREEN     = \033[0;32m
C_YELLOW    = \033[0;33m
C_BLUE      = \033[0;34m
C_WHITE     = \033[0;37m
C_END       = \033[0m
