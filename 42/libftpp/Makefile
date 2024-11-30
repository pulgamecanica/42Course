NAME            =   libftpp.a

INC_PATH        =   include

SRC_PATH        =   src

SRC             =   src/data_buffer.cpp \
		src/client.cpp \
		src/server.cpp \
		src/message.cpp \
		src/persistent_worker.cpp \
		src/worker_pool.cpp \
		src/thread.cpp \
		src/thread_safe_iostream.cpp \
		src/memento.cpp \

CXXFLAGS          =  -g -Wall -Wextra -Werror -std=c++17 -pedantic -fsanitize=address #-O3 -g -fsanitize=leak

INC             =   -I${INC_PATH}

OBJ_D           =   objs

OBJ             =   $(SRC:${SRC_PATH}/%.cpp=${OBJ_D}/%.o)

UNAME           :=  $(shell uname)

RM              =   rm -rf

LIB	= ar rc

RL	= ranlib

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
SOURCEDIR     = ./doc
BUILDDIR      = ./doc/_build

all:        ${NAME}

${OBJ_D}/%.o:${SRC_PATH}/%.cpp
			@mkdir -p ${OBJ_D}
			$(CXX) $(CFLAGS) ${INC} -o $@ -c $<

${NAME}:   ${OBJ}
			@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
			${LIB} ${NAME} ${OBJ}
			${RL} ${NAME}
			@printf "$(C_GREEN)DONE$(C_END)\n"

test:	all test_pool test_data_buffer test_memento test_observer test_singleton test_state_machine test_thread_safe_iostream test_thread_safe_queue test_thread test_worker_pool test_persistant_worker test_message test_client test_server
			@printf "$(C_BLUE)Testing $(C_YELLOW)$(NAME)$(C_END)\n"

			@printf "\n$(C_BLUE)********************** Pool Test**********************$(C_END)\n"
			-@./test_pool
			@printf "$(C_BLUE)*****************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** Data Buffer Test**********************$(C_END)\n"
			-@./test_data_buffer
			@printf "$(C_BLUE)***************************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** Memento **********************$(C_END)\n"
			-@./test_memento
			@printf "$(C_BLUE)*******************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** Observer **********************$(C_END)\n"
			-@./test_observer
			@printf "$(C_BLUE)********************************************************$(C_END)\n"
			

			@printf "\n$(C_BLUE)********************** Singleton **********************$(C_END)\n"
			-@./test_singleton
			@printf "$(C_BLUE)*********************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** State Machine **********************$(C_END)\n"
			-@./test_state_machine
			@printf "$(C_BLUE)*************************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** Thread Safe IOStream **********************$(C_END)\n"
			-@echo "42" | ./test_thread_safe_iostream
			@printf "$(C_BLUE)********************************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** Thread Safe Queue **********************$(C_END)\n"
			-@./test_thread_safe_queue
			@printf "$(C_BLUE)*****************************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** Thread **********************$(C_END)\n"
			-@./test_thread
			@printf "$(C_BLUE)******************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** Worker Pool **********************$(C_END)\n"
			-@echo -n "Total : "; ./test_worker_pool | wc -l
			@printf "$(C_BLUE)***********************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** Persistant Worker **********************$(C_END)\n"
			-@echo -n "Total : "; ./test_persistant_worker | wc -l
			@printf "$(C_BLUE)*****************************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** Message **********************$(C_END)\n"
			-@./test_message
			@printf "$(C_BLUE)******************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** Client **********************$(C_END)\n"
			-@./test_client
			@printf "$(C_BLUE)******************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)********************** Server **********************$(C_END)\n"
			-@./test_server
			@printf "$(C_BLUE)******************************************************$(C_END)\n"
			
			@printf "\n$(C_BLUE)Finished Test...$(C_END)\n"
			@$(MAKE) show --no-print-directory

test_pool: tests/main_pool.cpp
	$(CXX) -o test_pool -I include $< -L . -lftpp

test_data_buffer: tests/main_data_buffer.cpp
	$(CXX) -o test_data_buffer -I include $< -L . -lftpp

test_memento: tests/main_memento.cpp
	$(CXX) -o test_memento -I include $< -L . -lftpp

test_observer: tests/main_observer.cpp
	$(CXX) -o test_observer -I include $< -L . -lftpp

test_singleton: tests/main_singleton.cpp
	$(CXX) -o test_singleton -I include $< -L . -lftpp

test_state_machine: tests/main_state_machine.cpp
	$(CXX) -o test_state_machine -I include $< -L . -lftpp

test_thread_safe_iostream: tests/main_thread_safe_iostream.cpp
	$(CXX) -o test_thread_safe_iostream -I include $< -L . -lftpp

test_thread_safe_queue: tests/main_thread_safe_queue.cpp
	$(CXX) -o test_thread_safe_queue -I include $< -L . -lftpp

test_thread: tests/main_thread.cpp
	$(CXX) -o test_thread -I include $< -L . -lftpp

test_worker_pool: tests/main_worker_pool.cpp
	$(CXX) -o test_worker_pool -I include $< -L . -lftpp

test_persistant_worker: tests/main_persistant_worker.cpp
	$(CXX) -o test_persistant_worker -I include $< -L . -lftpp

test_message: tests/main_message.cpp
	$(CXX) -o test_message -g -fsanitize=address -I include $< -L . -lftpp

test_client: tests/main_client.cpp
	$(CXX) -o test_client -g -fsanitize=address -I include $< -L . -lftpp

test_server: tests/main_server.cpp
	$(CXX) -o test_server -g -fsanitize=address -I include $< -L . -lftpp


valgrind-%:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=$(subst valgrind-,,$@)-valgrind-out.txt ./$(subst valgrind-,,$@) $(subst valgrind-,,$@)_ARGS
	@echo "[MAKEFILE] You can setup test arguments by setting up the env $(NAME)_ARGS"
	@echo "[MAKEFILE] Ex: export $(NAME)_ARGS=\"-a --recursive ..\""
clean:
			$(RM) $(OBJ_D)
			@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
			@printf "$(C_RED)Cleaning objs$(C_END)\n"

fclean:     clean
			$(RM) $(NAME) *valgrind-out.txt doc/xml test_*
			@printf "$(C_RED)Deleted Everything$(C_END)\n"

re: fclean all

show:
	@printf "$(C_GREEN)"
	@printf "UNAME     : $(UNAME)\n"
	@printf "NAME      : $(C_YELLOW)$(NAME)$(C_GREEN)\n"
	@printf "AR        : $(AR)\n"
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
	@-doxygen ./doc/Doxyfile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

C_BLACK     = \033[0;30m
C_RED       = \033[0;31m
C_GREEN     = \033[0;32m
C_YELLOW    = \033[0;33m
C_BLUE      = \033[0;34m
C_WHITE     = \033[0;37m
C_END       = \033[0m
