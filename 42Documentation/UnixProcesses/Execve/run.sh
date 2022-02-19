#!/bin/bash
GREEN='\033[0;32m';
BLUE='\033[0;34m';
NC='\033[0m'
echo "Compiling libft..."
make -C ../C_Libft
echo -e "${BLUE}Execve: Execute a program (ls program located in /usr/bin/ls [linux FEDORA 32])${NC}"
gcc main.c -I ../C_Libft/includes/ -L ../C_Libft/ -lft -o execve && ./execve ".."
rm execve
echo -e "${BLUE}Running program 2: ${NC}"
gcc main2.c -I ../C_Libft/includes/ -L ../C_Libft/ -lft -o execve && ./execve "."
rm execve
echo -e "${GREEN}Finished!${NC}"
