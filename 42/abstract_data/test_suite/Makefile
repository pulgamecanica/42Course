# ========================
# CONFIGURATION
# ========================

# Compiler and flags
CXX             := c++
STD             := -std=c++17
CXXFLAGS        := -Wall -Wextra -Werror $(STD)
SANITIZE        := -fsanitize=address
DEBUG_FLAGS     := -g -DDEBUG
INCLUDES        := -Iinclude -I../include

# Source files
SRC_DIR         := src
SRC_FILES       := $(wildcard $(SRC_DIR)/*.cpp)

# Executables
BIN_DIR         := bin
BIN_FT          := $(BIN_DIR)/ft.out
BIN_STD         := $(BIN_DIR)/std.out
BIN_DEBUG       := $(BIN_DIR)/debug.out

# Out targets
OUT_DIR = out
OUT_FT  = $(OUT_DIR)/ft.out
OUT_STD = $(OUT_DIR)/std.out

# Testing
N_TESTS = 10

# ========================
# TARGETS
# ========================

all: $(BIN_FT) $(BIN_STD)

# FT build
$(BIN_FT): $(SRC_FILES)
	@mkdir -p $(BIN_DIR)
	@echo "Compiling FT version..."
	$(CXX) $(CXXFLAGS) $(SANITIZE) $(INCLUDES) -DMODE_FT $^ -o $@

# STD build
$(BIN_STD): $(SRC_FILES)
	@mkdir -p $(BIN_DIR)
	@echo "Compiling STD version..."
	$(CXX) $(CXXFLAGS) $(SANITIZE) $(INCLUDES) -DMODE_STD $^ -o $@

# Debug build
debug: $(SRC_FILES)
	@mkdir -p $(BIN_DIR)
	@echo "Compiling Debug version..."
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -DMODE_FT $^ -o $(BIN_DEBUG)

# Run both versions and compare output
test: all
	@mkdir -p $(OUT_DIR)
	@echo "\n=== FT OUTPUT ==="
	./$(BIN_FT) $(N_TESTS) > $(OUT_FT)
	@echo "\n=== STD OUTPUT ==="
	./$(BIN_STD) $(N_TESTS) > $(OUT_STD)
	@echo "\n=== DIFF ==="
	-@diff $(OUT_FT) $(OUT_STD)
	@echo "=== END OF TESTS ==="

# Clean object files and binaries
clean:
	@echo "Nothing to clean (no object files)"

fclean:
	@echo "Removing binaries..."
	@rm -rf $(BIN_DIR) $(OUT_DIR)

re: fclean all

# Info
show:
	@echo "Container Testing Suite"
	@echo "  FT implementation in: ../includes/"
	@echo "  Test sources in:      src/"
	@echo "  Test headers in:      include/"
	@echo "  Make targets:"
	@echo "    all      → build both FT and STD"
	@echo "    test     → run both binaries and see diff"
	@echo "    debug    → build FT with debug flags"
	@echo "    clean    → nothing (header-only)"
	@echo "    fclean   → remove all binaries"
	@echo "    re       → rebuild from scratch"

# ========================
# PHONY
# ========================
.PHONY: all clean fclean re test show debug
