# **************************************************************************** #
#    SCOP - Basic GPU rendering with Vulkan                                    #
# **************************************************************************** #

NAME		= scop

# --------------------------------------------------------------------------- #
#  Directories                                                                 #
# --------------------------------------------------------------------------- #
SRC_DIR		= srcs
INC_DIR		= includes
OBJ_DIR		= objs
SHADER_DIR	= assets/shaders

# --------------------------------------------------------------------------- #
#  Sources & Objects                                                           #
# --------------------------------------------------------------------------- #
SRC			= $(wildcard $(SRC_DIR)/*.cpp)
OBJ			= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
# Auto-generated header dependency files (one .d per .o) so edits to headers
# trigger recompiles of every .cpp that includes them.
DEP			= $(OBJ:.o=.d)

# --------------------------------------------------------------------------- #
#  Shaders                                                                     #
# --------------------------------------------------------------------------- #
GLSLC		= glslc
VERT_SRC	= $(wildcard $(SHADER_DIR)/*.vert)
FRAG_SRC	= $(wildcard $(SHADER_DIR)/*.frag)
VERT_SPV	= $(VERT_SRC:.vert=.vert.spv)
FRAG_SPV	= $(FRAG_SRC:.frag=.frag.spv)
SHADER_SPV	= $(VERT_SPV) $(FRAG_SPV)

# --------------------------------------------------------------------------- #
#  Compiler & Flags                                                            #
# --------------------------------------------------------------------------- #
UNAME		:= $(shell uname)

ifeq ($(UNAME), Darwin)
	CPP		= c++
else ifeq ($(UNAME), FreeBSD)
	CPP		= clang++
else
	CPP		= c++
	CPPFLAGS += -D LINUX
endif

CPPFLAGS	+= -Wall -Wextra -Werror -std=c++17 -pedantic
# -MMD: emit a .d with header deps; -MP: add phony targets so a deleted header
# doesn't break the build.
CPPFLAGS	+= -MMD -MP
LDFLAGS		= -lglfw -lvulkan -ldl -lpthread

# Vulkan SDK (adjust if installed elsewhere)
VULKAN_SDK	?= /opt/vulkan-sdk/x86_64
INC			= -I $(INC_DIR) -I $(VULKAN_SDK)/include
LDFLAGS		+= -L $(VULKAN_SDK)/lib -Wl,-rpath,$(VULKAN_SDK)/lib

# --------------------------------------------------------------------------- #
#  External Dependencies                                                       #
# --------------------------------------------------------------------------- #
VULKAN_HPP		= Vulkan-Hpp
VULKAN_HPP_URL	= https://github.com/KhronosGroup/Vulkan-Hpp

# --------------------------------------------------------------------------- #
#  Install layout (per-user, no sudo)                                          #
# --------------------------------------------------------------------------- #
APP_ID			?= scop
PREFIX			?= $(HOME)/.local
BIN_DIR			?= $(PREFIX)/bin
DESKTOP_DIR		?= $(PREFIX)/share/applications
# Themed icon dir (hicolor/256x256/apps) -- compositor finds it via Icon=$(APP_ID)
ICON_DIR		?= $(PREFIX)/share/icons/hicolor/256x256/apps
# Asset bundle: the app uses relative paths like assets/shaders/, so we install
# the whole assets/ tree here and point the desktop file's Path= at it.
DATA_DIR		?= $(PREFIX)/share/$(APP_ID)

ICON_SRC		= assets/icons/42logo.png
ICON_INSTALLED	= $(ICON_DIR)/$(APP_ID).png
DESKTOP_FILE	= $(APP_ID).desktop
INSTALLED_BIN	= $(BIN_DIR)/$(APP_ID)

# --------------------------------------------------------------------------- #
#  Targets                                                                     #
# --------------------------------------------------------------------------- #
all: shaders $(NAME)

# -- Shader compilation ------------------------------------------------------ #
shaders: $(SHADER_SPV)

$(SHADER_DIR)/%.vert.spv: $(SHADER_DIR)/%.vert
	@printf "$(C_BLUE)Compiling shader $(C_YELLOW)$<$(C_END)\n"
	$(GLSLC) $< -o $@

$(SHADER_DIR)/%.frag.spv: $(SHADER_DIR)/%.frag
	@printf "$(C_BLUE)Compiling shader $(C_YELLOW)$<$(C_END)\n"
	$(GLSLC) $< -o $@

# -- C++ compilation --------------------------------------------------------- #
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(INC) -o $@ -c $<

$(NAME): $(OBJ)
	@printf "$(C_BLUE)Linking $(C_YELLOW)$(NAME)$(C_END) ...\n"
	$(CPP) $(CPPFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)
	@printf "$(C_GREEN)Build complete: $(NAME)$(C_END)\n"

# Pull in the generated header-dependency rules (silent on first build when
# none exist yet). clean removes $(OBJ_DIR) wholesale, so .d files go with it.
-include $(DEP)

# -- Setup targets ----------------------------------------------------------- #
$(VULKAN_HPP):
	git clone --recurse-submodules $(VULKAN_HPP_URL) $(VULKAN_HPP)

setup_ubuntu: $(VULKAN_HPP)
	sudo apt install libglfw3-dev vulkan-tools libvulkan-dev \
		vulkan-validationlayers-dev spirv-tools

setup_fedora: $(VULKAN_HPP)
	sudo dnf install glfw-devel vulkan-tools vulkan-loader-devel
	-sudo dnf install mesa-vulkan-devel vulkan-validation-layers-devel

# -- Install / Desktop entry ------------------------------------------------- #
# The desktop file is generated so paths reflect the chosen PREFIX. Icon is
# referenced by name (works because we install under hicolor/256x256/apps).
$(DESKTOP_FILE):
	@printf "$(C_BLUE)Generating $(C_YELLOW)$@$(C_END)\n"
	@printf "[Desktop Entry]\n"                          >  $@
	@printf "Type=Application\n"                         >> $@
	@printf "Version=1.0\n"                              >> $@
	@printf "Name=42 Scop\n"                             >> $@
	@printf "GenericName=OBJ Viewer\n"                   >> $@
	@printf "Comment=Vulkan-based 3D OBJ viewer (42)\n"  >> $@
	@printf "Exec=$(INSTALLED_BIN)\n"                    >> $@
	@printf "Path=$(DATA_DIR)\n"                         >> $@
	@printf "Icon=$(APP_ID)\n"                           >> $@
	@printf "Terminal=false\n"                           >> $@
	@printf "Categories=Graphics;3DGraphics;Viewer;\n"   >> $@
	@printf "StartupNotify=true\n"                       >> $@

install: $(NAME) $(DESKTOP_FILE)
	@mkdir -p $(BIN_DIR) $(DESKTOP_DIR) $(ICON_DIR) $(DATA_DIR)
	install -m 755 $(NAME)         $(INSTALLED_BIN)
	install -m 644 $(ICON_SRC)     $(ICON_INSTALLED)
	install -m 644 $(DESKTOP_FILE) $(DESKTOP_DIR)/$(DESKTOP_FILE)
	# Copy the whole assets tree (shaders/, models/, textures/, icons/)
	cp -r assets $(DATA_DIR)/
	@if command -v update-desktop-database >/dev/null 2>&1; then \
		update-desktop-database $(DESKTOP_DIR) || true; \
	fi
	@if command -v gtk-update-icon-cache >/dev/null 2>&1; then \
		gtk-update-icon-cache -q -t $(PREFIX)/share/icons/hicolor || true; \
	fi
	@printf "$(C_GREEN)Installed:$(C_END)\n"
	@printf "  bin     : $(INSTALLED_BIN)\n"
	@printf "  desktop : $(DESKTOP_DIR)/$(DESKTOP_FILE)\n"
	@printf "  icon    : $(ICON_INSTALLED)\n"
	@printf "  data    : $(DATA_DIR)/assets/\n"
	@printf "$(C_YELLOW)Note:$(C_END) ensure $(BIN_DIR) is in your PATH\n"

uninstall:
	rm -f $(INSTALLED_BIN)
	rm -f $(DESKTOP_DIR)/$(DESKTOP_FILE)
	rm -f $(ICON_INSTALLED)
	rm -rf $(DATA_DIR)
	@if command -v update-desktop-database >/dev/null 2>&1; then \
		update-desktop-database $(DESKTOP_DIR) || true; \
	fi
	@printf "$(C_RED)Uninstalled $(APP_ID)$(C_END)\n"

# -- Clean ------------------------------------------------------------------- #
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(SHADER_SPV)
	@printf "$(C_RED)Cleaned objects and compiled shaders$(C_END)\n"

fclean: clean
	rm -f $(NAME)
	rm -f $(DESKTOP_FILE)
	@printf "$(C_RED)Removed $(NAME)$(C_END)\n"

re: fclean all

# -- Info -------------------------------------------------------------------- #
show:
	@printf "$(C_GREEN)"
	@printf "UNAME    : $(UNAME)\n"
	@printf "NAME     : $(C_YELLOW)$(NAME)$(C_GREEN)\n"
	@printf "CPP      : $(CPP)\n"
	@printf "CPPFLAGS : $(CPPFLAGS)\n"
	@printf "LDFLAGS  : $(LDFLAGS)\n"
	@printf "INC      : $(INC)\n"
	@printf "SRC      : $(C_YELLOW)$(SRC)$(C_GREEN)\n"
	@printf "OBJ      : $(C_YELLOW)$(OBJ)$(C_GREEN)\n"
	@printf "SHADERS  : $(C_YELLOW)$(SHADER_SPV)$(C_END)\n"

.PHONY: all shaders clean fclean re show setup_ubuntu setup_fedora install uninstall

# --------------------------------------------------------------------------- #
#  Colors                                                                      #
# --------------------------------------------------------------------------- #
C_RED		= \033[0;31m
C_GREEN		= \033[0;32m
C_YELLOW	= \033[0;33m
C_BLUE		= \033[0;34m
C_END		= \033[0m
