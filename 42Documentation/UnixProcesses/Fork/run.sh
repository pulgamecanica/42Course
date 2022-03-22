#!/bin/bash
GREEN='\033[0;32m';
BLUE='\033[0;34m';
NC='\033[0m'
echo "Compiling libft..."
make -C ../../../42Core/Libft/
echo -e "${BLUE}Fork: Create a new process${NC}"
gcc main.c -I ../../../42Core/Libft/includes/ -L ../../../42Core/Libft/ -lft -o fork && ./fork 
rm fork
make fclean -C ../../../42Core/Libft/
echo -e "${GREEN}Finished!${NC}"
