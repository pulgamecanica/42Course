#!/bin/bash
GREEN='\033[0;32m';
BLUE='\033[0;34m';
NC='\033[0m'
echo "Compiling libft..."
make -C ../C_Libft
echo -e "${BLUE}Fork: Create a new process${NC}"
gcc main.c -I ../C_Libft/includes/ -L ../C_Libft/ -lft -o fork && ./fork 
rm fork
echo -e "${GREEN}Finished!${NC}"
