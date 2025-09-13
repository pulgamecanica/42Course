# -------------------------
# Project configuration
# -------------------------
SRC_DIR        := src
OBJ_DIR        := objs
INC_DIR        := include
# List your C sources here (space-separated)
SRC            := $(SRC_DIR)/malloc.c

# Test harness (optional)
TEST_SRC       := tests/main.c
TEST_BIN       := test_runner

# -------------------------
# Toolchain & platform
# -------------------------
OS             := $(shell uname -s)
ARCH           := $(shell uname -m)
HOSTTYPE      ?= $(ARCH)_$(OS)

CC             ?= cc
RM             := rm -rf
LN             := ln -sf
MKDIR_P        := mkdir -p

# -------------------------
# Output names
# -------------------------
NAME           := libft_malloc_$(HOSTTYPE).so
LINK_NAME      := libft_malloc.so

# -------------------------
# Flags
# -------------------------
CFLAGS         := -Wall -Wextra -Werror -pedantic -ansi -O3 -fPIC -MMD -MP -I$(INC_DIR) -std=gnu11
LDFLAGS        :=
LDLIBS         :=
LDLINK         := LD_PRELOAD=${LINK_NAME}

# -------------------------
# Derived variables
# -------------------------
OBJ            := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
DEP            := $(OBJ:.o=.d)

# -------------------------
# Libft
# -------------------------
LIBFT_DIR      := ./Libft
LIBFT_INCS     := $(LIBFT_DIR)/includes
LIBFT_NAME     := ft
LIBFT_LIB      := $(LIBFT_DIR)/lib$(LIBFT_NAME).a
LDLIBS        += -L $(LIBFT_DIR) -l $(LIBFT_NAME)
CFLAGS         += -I $(LIBFT_INCS)

# -------------------------
# Tests
# -------------------------
TEST_LDFLAGS = -Wl,--no-as-needed -Wl,--as-needed

# OS-specific settings for shared libraries
ifeq ($(OS),Darwin)
  # macOS uses -dynamiclib; keep .so filename as requested
  SHARED_FLAG  := -dynamiclib
  # (Optional) install_name for @rpath loading:
  # LDFLAGS     += -Wl,-install_name,@rpath/$(LINK_NAME)
else
  SHARED_FLAG  := -shared
  CFLAGS     += -D LINUX
endif

# Optional toggles:
#   make DEBUG=1     -> adds -g and disables -O3 unless SAN=1
ifeq ($(DEBUG),1)
  CFLAGS += -g
endif

ifeq ($(LIB_G),1)
	LDLINK         =
else
	CFLAGS += -D FT_MODE
	TEST_LDFLAGS += -L. -lft_malloc 
endif

# -------------------------
# Default target
# -------------------------
.PHONY: all
all: $(NAME)

# -------------------------
# Build rules
# -------------------------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_LIB):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT_LIB) $(OBJ)
	@printf "\033[0;33mLinking $@\033[0m ...\n"
	$(CC) $(SHARED_FLAG) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)
	@printf "\033[0;32mDONE: [$@]\033[0m\n"
	@printf "\033[0;33mCreating symbolic link -> $(LINK_NAME)\033[0m\n"
	@$(LN) $(NAME) $(LINK_NAME)
	@printf "\033[0;32mDONE: [$(LINK_NAME)]\033[0m\n"

# -------------------------
# Test harness
# -------------------------
.PHONY: test
test: $(TEST_BIN)
	@printf "\n\033[0;34mExecutabe loaded shared libraries\033[0m\n"
	ldd ./$(TEST_BIN)
	@printf "\n\033[0;34m************** Main Test **************\033[0m\n"
	-${LDLINK} ./$(TEST_BIN) $(ARGS)
	@printf "\n\033[0;34m***************************************\033[0m\n"

$(TEST_BIN): $(TEST_SRC) $(NAME)
	$(CC) -o $@ $(CPPFLAGS) $(CFLAGS) $< $(TEST_LDFLAGS)

# -------------------------
# Valgrind helper
# Usage: make valgrind-test_runner
# Pass args: ARGS="..."
# -------------------------
.PHONY: valgrind
valgrind:
# 	${LDLINK} \

	valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes --verbose \
         --soname-synonyms=somalloc=libc.so.6:libft_malloc.so \
         --log-file=test-valgrind-out.txt \
         ./test_runner
	@echo "[MAKE] You can pass test args via ARGS=\"...\""

# -------------------------
# Housekeeping
# -------------------------
.PHONY: clean fclean re show check_malloc_lib
clean:
	$(RM) $(OBJ_DIR)
	@printf "\033[0;31mCleaning objs\033[0m\n"

fclean: clean
	$(RM) $(NAME) $(LINK_NAME) $(TEST_BIN) *valgrind-out.txt *.d
	@printf "\033[0;31mDeleted Everything\033[0m\n"

re: fclean all

check_malloc_lib:
	@printf "\033[0;32mChecking if malloc is loaded in $(LINK_NAME)\033[0m\n"
	-nm -D $(LINK_NAME) | grep 'malloc' --color=always
	@printf "\033[0;32m--------------------------------------------\033[0m\n"

show: check_malloc_lib
	@printf "\033[0;32m"
	@printf "OS        : $(OS)\n"
	@printf "ARCH      : $(ARCH)\n"
	@printf "HOSTTYPE  : $(HOSTTYPE)\n"
	@printf "CC        : $(CC)\n"
	@printf "CFLAGS    : $(CFLAGS)\n"
	@printf "LDFLAGS   : $(LDFLAGS)\n"
	@printf "LDLINK    : $(LDLINK)\n"
	@printf "SRC       : \033[0;33m$(SRC)\033[0;32m\n"
	@printf "OBJ       : \033[0;33m$(OBJ)\033[0m\n"

# Include auto-generated dependency files
-include $(DEP)
