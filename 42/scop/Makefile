# Makefile created automatically from script\n
# C++ scop [Thu Nov 16 08:07:50 PM CET 2023]\n
#MAKEFLAGS += --silent	# Silence makefile [Commented by default]\n
NAME	=	scop

DEBUG = 0

IMGUI_GIT = https://github.com/ocornut/imgui.git

IMGUI = imgui

IMGUI_INC = $(IMGUI)

IMGUI_BACKENDS_INC = $(IMGUI)/backends

IMGUI_SRC = backends/imgui_impl_glfw.cpp \
						backends/imgui_impl_vulkan.cpp \
						imgui.cpp \
						imgui_demo.cpp \
						imgui_tables.cpp \
						imgui_draw.cpp \
						imgui_widgets.cpp \

IMGUI_DUMP = imgui_objs

IMGUI_OBJ = $(IMGUI_SRC:%.cpp=$(IMGUI_DUMP)/%.o)

SRC	=	$(wildcard srcs/*.cpp srcs/figures/*.cpp)

CFLAGS	=	-g -Wall -Wextra -Werror -std=c++17 -pedantic -fsanitize=address -fno-omit-frame-pointer

LDFLAGS = -lGL -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

INC	=	-Iincludes -Iincludes/figures -I$(IMGUI_INC) -I$(IMGUI_BACKENDS_INC)

OBJS_DUMP	=	objs

OBJ	=	$(SRC:srcs/%.cpp=$(OBJS_DUMP)/%.o)

UNAME	:=	$(shell uname)

RM		= rm -rf

VULKAN_HPP = Vulkan-Hpp

VULKAN_HPP_GIT = https://github.com/KhronosGroup/Vulkan-Hpp

CFLAGS += -D SCOP_DEBUG=$(DEBUG)

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

$(IMGUI):
	git clone $(IMGUI_GIT)

$(IMGUI_DUMP)/%.o:$(IMGUI)/%.cpp
	@mkdir -p $(IMGUI_DUMP)
	@mkdir -p $(IMGUI_DUMP)/backends
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(OBJS_DUMP)/%.o:srcs/%.cpp
	@mkdir -p $(OBJS_DUMP)
	@mkdir -p $(OBJS_DUMP)/figures
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(NAME):	$(OBJ) $(IMGUI_OBJ) includes/Vulkan42.hpp includes/Scop.hpp includes/GUIContainer.hpp
	@printf "Compiling $(C_YELLOW)$(NAME)$(C_END) ... \n"
	$(CC) $(CFLAGS) -o $(NAME) $(IMGUI_OBJ) $(OBJ) $(LDFLAGS)
	@printf "$(C_GREEN)DONE$(C_END)\n"

build_ubuntu: $(VULKAN_HPP)
# 	sudo apt update
	sudo apt install libglfw3-dev \
										libglm-dev
	sudo apt install vulkan-tools \
										libvulkan-dev \
										vulkan-validationlayers-dev\
										spirv-tools
	rm -rf imgui
	git clone $(IMGUI_GIT)
	cd $(VULKAN_HPP)
	cmake  -DVULKAN_HPP_SAMPLES_BUILD=ON -DVULKAN_HPP_SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP=ON -DVULKAN_HPP_TESTS_BUILD=ON -DVULKAN_HPP_TESTS_BUILD_WITH_LOCAL_VULKAN_HPP=ON -B build

build_fedora: $(VULKAN_HPP)
	sudo dnf install glfw-devel \
										glm-devel
	sudo dnf install vulkan-tools
	sudo dnf install vulkan-loader-devel
	-sudo dnf install mesa-vulkan-devel
	-sudo dnf install vulkan-validation-layers-devel
	rm -rf imgui
	git clone $(IMGUI_GIT)
	cd $(VULKAN_HPP)
	cmake  -DVULKAN_HPP_SAMPLES_BUILD=ON -DVULKAN_HPP_SAMPLES_BUILD_WITH_LOCAL_VULKAN_HPP=ON -DVULKAN_HPP_TESTS_BUILD=ON -DVULKAN_HPP_TESTS_BUILD_WITH_LOCAL_VULKAN_HPP=ON -B build

test:	re
	@printf "$(C_BLUE)Testing $(C_YELLOW)$(NAME)$(C_END)\n"
	@printf "\n$(C_BLUE)********************************************$(C_END)\n"
	-LSAN_OPTIONS=suppressions=suppressions.supp ./$(NAME)
	@printf "\n$(C_BLUE)********************************************$(C_END)\n"
	@printf "\n$(C_BLUE)Finished Test...$(C_END)\n"
	@$(MAKE) show --no-print-directory

clean:
	$(RM) $(OBJS_DUMP) #$(IMGUI_DUMP)
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
	@printf "OBJ		: $(C_YELLOW)[$(OBJS_DUMP)] --> $(OBJ)$(C_END)\n"

.PHONY: all test re

C_BLACK		= \033[0;30m
C_RED		= \033[0;31m
C_GREEN		= \033[0;32m
C_YELLOW	= \033[0;33m
C_BLUE		= \033[0;34m
C_WHITE		= \033[0;37m
C_END		= \033[0m
