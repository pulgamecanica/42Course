# ========================
# VARIABLES
# ========================

# Compiler and flags
CXX             := c++
CXXFLAGS        := -Wall -Wextra -Werror -std=c++98 -fsanitize=address
CXXFLAGS_DEBUG  := $(CXXFLAGS) -g -DDEBUG
CXXFLAGS_BENCH  := -Wall -Wextra -Werror -std=c++11

INCLUDES        := -Iinclude

# Executables
BIN_FT          := ft_containers.out
BIN_STD         := std_containers.out
BIN_DEBUG       := debug.out
BIN_BENCH       := vector_benchmark.out

# Directories and sources
SRC_DIR         := src
SRC_COMMON      := $(SRC_DIR)/main.cpp $(SRC_DIR)/test_vector.cpp
SRC_BENCH       := $(SRC_DIR)/benchmark_vector.cpp

# Python setup
VENV_DIR := .venv
PYTHON := $(VENV_DIR)/bin/python
PIP := $(VENV_DIR)/bin/pip


# ========================
# TARGETS
# ========================

all: $(BIN_FT) $(BIN_STD) $(BIN_BENCH)

$(BIN_FT): $(SRC_COMMON)
	@echo "Building: $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -DMODE_FT $^ -o $@

$(BIN_STD): $(SRC_COMMON)
	@echo "Building: $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -DMODE_STD $^ -o $@

$(BIN_BENCH): $(SRC_BENCH)
	@echo "Building: $@ (C++11)"
	$(CXX) $(CXXFLAGS_BENCH) $(INCLUDES) $^ -o $@

debug: $(SRC_COMMON)
	@echo "Building: $(BIN_DEBUG) (Debug)"
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) -DMODE_FT $^ -o $(BIN_DEBUG)

test: all
	@echo "\n--- FT Version ---"
	./$(BIN_FT)
	@echo "\n--- STD Version ---"
	./$(BIN_STD)
	@echo "\n--- Benchmark ---"
	./$(BIN_BENCH)

benchmark: $(BIN_BENCH)
	@echo "\n--- Running Benchmark ---"
	./$(BIN_BENCH)

venv:
	@echo "Creating Python virtual environment..."
	@test -d $(VENV_DIR) || python3 -m venv $(VENV_DIR)

install: venv
	@echo "Installing Python requirements..."
	$(PIP) install --upgrade pip
	$(PIP) install -r requirements.txt

plot: $(BIN_BENCH)
	@echo "Generating benchmark graphs..."
	$(PYTHON) plot.py

show:
	@echo "Project: Abstract Data - C++ Containers"
	@echo "Containers implemented:"
	@echo " - Sequence: vector, list, deque"
	@echo " - Associative: map, set, multimap, multiset"
	@echo " - Adaptors: stack, queue, priority_queue"
	@echo " - $(BIN_FT): Uses your ft::containers"
	@echo " - $(BIN_STD): Uses standard std::containers"
	@echo "Use \`make test\` to compare output and correctness"
	@echo "Use \`make benchmark\` to compare performance"

clean:
	@echo "Nothing to clean (header-only project)"

fclean:
	@echo "Removing binaries..."
	rm -f $(BIN_FT) $(BIN_STD) $(BIN_DEBUG) $(BIN_BENCH) benchmark_results.csv

re: fclean all

# ========================
# PHONY
# ========================
.PHONY: all clean fclean re test show debug benchmark venv install_deps graphs
