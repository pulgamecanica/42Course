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

# -- Setup targets ----------------------------------------------------------- #
$(VULKAN_HPP):
	git clone --recurse-submodules $(VULKAN_HPP_URL) $(VULKAN_HPP)

setup_ubuntu: $(VULKAN_HPP)
	sudo apt install libglfw3-dev vulkan-tools libvulkan-dev \
		vulkan-validationlayers-dev spirv-tools

setup_fedora: $(VULKAN_HPP)
	sudo dnf install glfw-devel vulkan-tools vulkan-loader-devel
	-sudo dnf install mesa-vulkan-devel vulkan-validation-layers-devel

# -- Clean ------------------------------------------------------------------- #
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(SHADER_SPV)
	@printf "$(C_RED)Cleaned objects and compiled shaders$(C_END)\n"

fclean: clean
	rm -f $(NAME)
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

.PHONY: all shaders clean fclean re show setup_ubuntu setup_fedora

# --------------------------------------------------------------------------- #
#  Colors                                                                      #
# --------------------------------------------------------------------------- #
C_RED		= \033[0;31m
C_GREEN		= \033[0;32m
C_YELLOW	= \033[0;33m
C_BLUE		= \033[0;34m
C_END		= \033[0m
