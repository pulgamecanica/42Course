# Makefile created automatically from script\n
# C++ scop [Thu Nov 16 08:07:50 PM CET 2023]\n
#MAKEFLAGS += --silent	# Silence makefile [Commented by default]\n
NAME	=	scop

SRC	=	$(wildcard srcs/*.cpp)

CFLAGS	=	-Wall -Wextra -Werror -std=c++17 -pedantic -fsanitize=address

LDFLAGS = -lGL -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

INC	=	-I includes

OBJS_D	=	objs

OBJ	=	$(SRC:srcs/%.cpp=$(OBJS_D)/%.o)

UNAME	:=	$(shell uname)

RM		= rm -rf

VULKAN_HPP = Vulkan-Hpp

VULKAN_HPP_GIT = https://github.com/KhronosGroup/Vulkan-Hpp

ifeq ($(UNAME), Darwin)
	CC = c++
else ifeq ($(UNAME), FreeBSD)
	CC = clang++
else
	CC	= c++
	CFLAGS += -D LINUX
endif

all: $(NAME)

$(VULKAN_HPP):
	git clone --recurse-submodules https://github.com/KhronosGroup/Vulkan-Hpp $(VULKAN_HPP)

$(OBJS_D)/%.o:srcs/%.cpp
	@mkdir -p $(OBJS_D)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(NAME):	$(OBJ)
	@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)
	@printf "$(C_GREEN)DONE$(C_END)\n"

build_ubuntu: $(VULKAN_HPP)
# 	sudo apt update
	sudo apt install libglfw3-dev \
										libglm-dev
	sudo apt install vulkan-tools \
										libvulkan-dev \
										vulkan-validationlayers-dev\
										spirv-tools
	cd $(VULKAN_HPP)
	cmake  -DVULKAN_HPP_SAMPLES_BUILD=ON -DVULKAN_HPP_SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP=ON -DVULKAN_HPP_TESTS_BUILD=ON -DVULKAN_HPP_TESTS_BUILD_WITH_LOCAL_VULKAN_HPP=ON -B build


build_fedora: $(VULKAN_HPP)
	sudo dnf install glfw-devel \
										glm-devel
	sudo dnf install vulkan-tools
	sudo dnf install vulkan-loader-devel
	-sudo dnf install mesa-vulkan-devel
	-sudo dnf install vulkan-validation-layers-devel
	cd $(VULKAN_HPP)
	cmake  -DVULKAN_HPP_SAMPLES_BUILD=ON -DVULKAN_HPP_SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP=ON -DVULKAN_HPP_TESTS_BUILD=ON -DVULKAN_HPP_TESTS_BUILD_WITH_LOCAL_VULKAN_HPP=ON -B build

# build_mac:
# 	brew install glfw
# 	brew install glm

test:	re
	@printf "$(C_BLUE)Testing $(C_YELLOW)$(NAME)$(C_END)\n"
	@printf "\n$(C_BLUE)********************************************$(C_END)\n"
	-@./$(NAME)
	@printf "\n$(C_BLUE)********************************************$(C_END)\n"
	@printf "\n$(C_BLUE)Finished Test...$(C_END)\n"
	@$(MAKE) show --no-print-directory
	@$(MAKE) fclean --no-print-directory

clean:
	$(RM) $(OBJS_D)
	@printf "$(C_RED)Cleaning objs$(C_END)\n"

fclean:	clean
	$(RM) $(NAME)
	@printf "$(C_RED)Deleted Everything$(C_END)\n"

re:	fclean all

show:
	@printf "$(C_GREEN)"
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME  		: $(C_YELLOW)$(NAME)$(C_GREEN)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "LDFLAGS		: $(LDFLAGS)\n"
	@printf "INCLUDES	: $(INC)\n"
	@printf "SRC		: $(C_YELLOW)$(SRC)$(C_GREEN)\n"
	@printf "OBJ		: $(C_YELLOW)[$(OBJS_D)] --> $(OBJ)$(C_END)\n"

.PHONY: all test re

C_BLACK		= \033[0;30m
C_RED		= \033[0;31m
C_GREEN		= \033[0;32m
C_YELLOW	= \033[0;33m
C_BLUE		= \033[0;34m
C_WHITE		= \033[0;37m
C_END		= \033[0m
