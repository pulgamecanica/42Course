# ─────────────────────────────────────────────────────────────────────────────
# Project
# ─────────────────────────────────────────────────────────────────────────────
NAME        := test
BIN_DIR     := bin
TARGET      := $(BIN_DIR)/$(NAME)

SRC_DIR     := src
SRCS        := $(wildcard $(SRC_DIR)/*.c)


OBJ_DIR     := build
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

# ─────────────────────────────────────────────────────────────────────────────
# Toolchain
# ─────────────────────────────────────────────────────────────────────────────
CC          := cc
CSTD        := -std=c11
WARN        := -w#-Wall -Wextra -Werror
DEPFLAGS    := -MMD -MP

# ─────────────────────────────────────────────────────────────────────────────
# Vendored SDL (installed locally into dist/SDL3)
# ─────────────────────────────────────────────────────────────────────────────
LIBS_DIR        := dist
SDL_DIR         := vendored/SDL
SDL_INSTALL     := $(LIBS_DIR)/SDL3
DIR_SDL_LIB     := $(SDL_INSTALL)/lib
INC_SDL         := $(SDL_INSTALL)/include
SDL_SO          := $(DIR_SDL_LIB)/libSDL3.so


# ─────────────────────────────────────────────────────────────────────────────
# Libui
# ─────────────────────────────────────────────────────────────────────────────
LIBUI           := libui
INC_LIBUI       := libui/include


# Compile / Link flags
CFLAGS      := $(CSTD) $(WARN) -I$(INC_SDL) -I$(INC_LIBUI)
LDFLAGS     := -L$(DIR_SDL_LIB) -lm
# Make the built binary find libSDL3.so at runtime (bin/ -> dist/SDL3/lib)
LDFLAGS     += -Wl,-rpath,'$$ORIGIN/../dist/SDL3/lib'
LDLIBS      := -lSDL3

# ─────────────────────────────────────────────────────────────────────────────
# Documentation (Doxygen → Sphinx with Breathe)
# ─────────────────────────────────────────────────────────────────────────────
# You can set these from the environment/CLI if you like.
SPHINXOPTS   ?=
SPHINXBUILD  ?= sphinx-build
SPHINXAUTOB  ?= sphinx-autobuild
DOXYGEN      ?= doxygen

DOC_DIR       := doc
SPHINX_SRC    := $(DOC_DIR)
SPHINX_BUILD  := $(DOC_DIR)/_build
SPHINX_HTML   := $(SPHINX_BUILD)/html
DOXYFILE      := $(DOC_DIR)/Doxyfile
DOXY_BUILD    := $(SPHINX_BUILD)/doxygen
BREATHE_XML   := $(DOXY_BUILD)/xml

# What sources should trigger doc rebuilds? (override if you keep headers elsewhere)
HDRS          := $(shell find $(SRC_DIR) -name '*.h' 2>/dev/null)
DOC_SOURCES   := $(SRCS) $(HDRS) $(wildcard $(SPHINX_SRC)/**/*.rst) $(wildcard $(SPHINX_SRC)/*.rst) $(SPHINX_SRC)/conf.py $(DOXYFILE)

# ─────────────────────────────────────────────────────────────────────────────
# Phony targets
# ─────────────────────────────────────────────────────────────────────────────
.PHONY: all libs show clean fclean ffclean re dirs run \
        help docs dox html docs-clean docs-open docs-serve \
        sphinx-%

# Default target
all: $(SDL_SO) $(TARGET)

# Build SDL and install it into dist/SDL3
libs: $(SDL_SO)

$(SDL_SO): | $(SDL_DIR)
	@mkdir -p $(LIBS_DIR)
	@echo "[SDL] Configuring..."
	@cmake -S $(SDL_DIR) -B $(SDL_DIR)/build >/dev/null
	@echo "[SDL] Building..."
	@cmake --build $(SDL_DIR)/build >/dev/null
	@echo "[SDL] Installing to temporary prefix..."
	@cmake --install $(SDL_DIR)/build --prefix $(SDL_DIR)/tmp >/dev/null
	@rm -rf $(SDL_INSTALL)
	@mv $(SDL_DIR)/tmp $(SDL_INSTALL)
	@echo "[SDL] Installed to $(SDL_INSTALL)"

# Clone SDL if missing
$(SDL_DIR):
	git clone --depth=1 https://github.com/libsdl-org/SDL.git $(SDL_DIR)

# Link final executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@
	@echo "[LD] $@"

# Compile objects with auto-deps
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@
	@echo "[CC] $<"

# Ensure directories exist
dirs: | $(OBJ_DIR) $(BIN_DIR)
$(OBJ_DIR):
	mkdir -p $@
$(BIN_DIR):
	mkdir -p $@

# Run (useful during this phase when building an exe)
run: all
	./$(TARGET)

# Introspection
show:
	@echo "NAME         = $(NAME)"
	@echo "TARGET       = $(TARGET)"
	@echo "SRCS         = $(SRCS)"
	@echo "OBJS         = $(OBJS)"
	@echo "INC_SDL      = $(INC_SDL)"
	@echo "DIR_SDL_LIB  = $(DIR_SDL_LIB)"
	@echo "SDL_SO       = $(SDL_SO)"
	@echo "SPHINX_SRC   = $(SPHINX_SRC)"
	@echo "SPHINX_BUILD = $(SPHINX_BUILD)"
	@echo "BREATHE_XML  = $(BREATHE_XML)"
	@echo "DOXYFILE     = $(DOXYFILE)"

# ─────────────────────────────────────────────────────────────────────────────
# Documentation workflows
# ─────────────────────────────────────────────────────────────────────────────
help:  ## Sphinx help (same as `sphinx-build -M help`)
	@$(MAKE) sphinx-help

# One-shot: build Doxygen man
man: dox-man

# One-shot: build Doxygen XML then Sphinx HTML
docs: dox html  ## Build docs (Doxygen + Sphinx HTML)

# Clean the built documentation only
docs-clean:     ## Remove only documentation builds
	@rm -rf "$(SPHINX_BUILD)"
	@echo "[DOC] cleaned $(SPHINX_BUILD)"

# Open the generated HTML index in a browser (xdg-open/open/start)
docs-open: html ## Open docs in default browser
	@html_index="$(SPHINX_HTML)/index.html"; \
	if command -v xdg-open >/dev/null 2>&1; then xdg-open "$$html_index"; \
	elif command -v open >/dev/null 2>&1; then open "$$html_index"; \
	elif command -v start >/dev/null 2>&1; then start "$$html_index"; \
	else echo "Open $$html_index in your browser."; fi

# Live-reload docs if sphinx-autobuild is available
docs-serve: check-doxygen check-sphinx  ## Live docs server (auto-rebuild)
	@echo "[DOC] starting sphinx-autobuild (Ctrl+C to stop)"
	@$(SPHINXAUTOB) \
		-b html "$(SPHINX_SRC)" "$(SPHINX_HTML)" \
		--watch "$(SRC_DIR)" --watch "$(DOC_DIR)" \
		--pre-build '$(MAKE) -s dox' \
		$(SPHINXOPTS)

# Build (or refresh) Doxygen XML in a Sphinx-friendly place
dox: $(BREATHE_XML)/index.xml  ## Generate Doxygen XML

# Build (or refresh) Doxygen man
dox-man: $(DOXYGEN) man


$(BREATHE_XML)/index.xml: $(DOC_SOURCES) | $(SPHINX_BUILD)
	@mkdir -p "$(BREATHE_XML)"
	@echo "[DOC] Doxygen → $(BREATHE_XML)"
	@{ \
	  cat "$(DOXYFILE)"; \
	  echo ""; \
	  echo "OUTPUT_DIRECTORY = $(abspath $(DOXY_BUILD))"; \
	  echo "GENERATE_XML = YES"; \
	  echo "XML_OUTPUT = xml"; \
	} | $(DOXYGEN) - >/dev/null

# Sphinx HTML (depends on the Breathe XML and Sphinx sources)
html: dox  ## Build Sphinx HTML
	$(SPHINXBUILD) -M html "$(BREATHE_XML)" "$(SPHINX_BUILD)" $(SPHINXOPTS) $(O)

# Pass-through for other Sphinx builders: `make sphinx-linkcheck`, etc.
sphinx-%: check-sphinx dox
	$(SPHINXBUILD) -M $* "$(SPHINX_SRC)" "$(SPHINX_BUILD)" $(SPHINXOPTS) $(O)

# Ensure Sphinx build dir exists
$(SPHINX_BUILD):
	@mkdir -p "$@"

# ─────────────────────────────────────────────────────────────────────────────
# Cleaning
# ─────────────────────────────────────────────────────────────────────────────
clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "[CLEAN] objects"

fclean: clean
	@$(RM) -r $(BIN_DIR) $(SPHINX_BUILD)
	@echo "[CLEAN] binaries"

# Remove everything including vendored libs (keeps docs; use docs-clean for those)
ffclean: fclean
	@rm -rf $(LIBS_DIR)
	@echo "[CLEAN] $(LIBS_DIR)"

# Rebuild from scratch (keeps vendored libs)
re: fclean all

# Include generated dependency files
-include $(DEPS)
