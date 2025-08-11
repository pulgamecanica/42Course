# ========================
# VARIABLES
# ========================

# Compiler and flags
CXX             := c++
CXXFLAGS        := -Wall -Wextra -Werror -std=c++98 -fsanitize=address
CXXFLAGS_BENCH  := -Wall -Wextra -Werror -std=c++11

INCLUDES        := -Iinclude

RM := rm -rf

# Executables
BIN_FT          := test_suite/bin/ft.out
BIN_STD         := test_suite/bin/std.out
BIN_BENCH       := benchmark.out

# Directories and sources
TEST_SUITE_SRC_DIR         := test_suite/src
SRC_DIR         := src
SRC_COMMON      := $(TEST_SUITE_SRC_DIR)/main.cpp
SRC_BENCH       := $(SRC_DIR)/benchmark.cpp

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
	make all -C test_suite

$(BIN_STD): $(SRC_COMMON)
	@echo "Building: $@"
	make all -C test_suite

$(BIN_BENCH): $(SRC_BENCH)
	@echo "Building: $@ (C++11)"
	$(CXX) $(CXXFLAGS_BENCH) $(INCLUDES) $^ -o $@

test: all
	make test -C test_suite
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
	$(PYTHON) plot.py &> /dev/null

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
	@echo "Use \`make plot\` to show plot"

clean:
	make clean -C test_suite

fclean: clean
	@echo "Removing binaries..."
	make fclean -C test_suite
	$(RM) $(BIN_FT) $(BIN_STD) $(BIN_BENCH) benchmark_results.csv plots

re: fclean all

# ========================
# PHONY
# ========================
.PHONY: all clean fclean re test show debug benchmark venv install_deps graphs
