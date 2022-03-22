#!/bin/bash
GREEN='\033[0;32m';
BLUE='\033[0;34m';
NC='\033[0m'
echo "Compiling libft..."
make -C ../../../42Core/Libft/
echo -e "${BLUE}Execve: Execute a program (ls program located in /usr/bin/ls [linux FEDORA 32])${NC}"
gcc main.c -I ../../../42Core/Libft/includes -L ../../../42Core/Libft/ -lft -o execve && ./execve ".."
rm execve
echo -e "${BLUE}Running program 2: ${NC}"
gcc main2.c -I ../../../42Core/Libft/includes/ -L ../../../42Core/Libft/ -lft -o execve && ./execve "."
rm execve
make fclean -C ../../../42Core/Libft/
echo -e "${GREEN}Finished!${NC}"
