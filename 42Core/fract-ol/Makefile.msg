#https://patorjk.com/software/taag/#p=display&c=echo&f=Small&t=%20%2B%20utils
#TITLE: DOH
#SUBTITLE: SMALL SLANT
BLACK=\e[1;30m
RED=\e[1;31m
GREEN=\e[92m
YELLOW=\e[1;33m
BLUE=\e[1;34m
PURPLE=\e[1;35m
CYAN=\e[1;36m
WHITE=\e[1;37m
RESET=\e[0m

all:
	@echo  "Messages Makefile"
msglibft:
	@sleep 0.1
	@echo  "                               bbbbbbbb                                                        ";
	@echo  "LLLLLLLLLLL               iiii b::::::b               ffffffffffffffff           tttt          ";
	@echo  "L:::::::::L              i::::ib::::::b              f::::::::::::::::f       ttt:::t          ";
	@echo  "L:::::::::L               iiii b::::::b             f::::::::::::::::::f      t:::::t          ";
	@echo  "LL:::::::LL                     b:::::b             f::::::fffffff:::::f      t:::::t          ";
	@echo  "  L:::::L               iiiiiii b:::::bbbbbbbbb     f:::::f       ffffffttttttt:::::ttttttt    ";
	@echo  "  L:::::L               i:::::i b::::::::::::::bb   f:::::f             t:::::::::::::::::t    ";
	@echo  "  L:::::L                i::::i b::::::::::::::::b f:::::::ffffff       t:::::::::::::::::t    ";
	@echo  "  L:::::L                i::::i b:::::bbbbb:::::::bf::::::::::::f       tttttt:::::::tttttt    ";
	@echo  "  L:::::L                i::::i b:::::b    b::::::bf::::::::::::f             t:::::t          ";
	@echo  "  L:::::L                i::::i b:::::b     b:::::bf:::::::ffffff             t:::::t          ";
	@echo  "  L:::::L                i::::i b:::::b     b:::::b f:::::f                   t:::::t          ";
	@echo  "  L:::::L         LLLLLL i::::i b:::::b     b:::::b f:::::f                   t:::::t    tttttt";
	@echo  "LL:::::::LLLLLLLLL:::::Li::::::ib:::::bbbbbb::::::bf:::::::f                  t::::::tttt:::::t";
	@echo  "L::::::::::::::::::::::Li::::::ib::::::::::::::::b f:::::::f                  tt::::::::::::::t";
	@echo  "L::::::::::::::::::::::Li::::::ib:::::::::::::::b  f:::::::f                    tt:::::::::::tt";
	@echo  "LLLLLLLLLLLLLLLLLLLLLLLLiiiiiiiibbbbbbbbbbbbbbbb   fffffffff                      ttttttttttt  ";
	@sleep 0.1
	@echo  "     _          _   _ _    ";
	@echo  "   _| |_   _  _| |_(_) |___";
	@echo  "  |_   _| | || |  _| | (_-<";
	@echo  "    |_|    \_,_|\__|_|_/__/";
	@echo  "                           ";
	@cat libft/includes/libft.h | grep -v 'libft.h' | grep 'int	' |grep 'ft_*' | sed 's/^/	+ /';
	@sleep 0.1;
	@cat libft/includes/libft.h | grep -v 'libft.h' | grep 'char	' | grep 'ft_*' | sed 's/^/	+ /';
	@sleep 0.1;
	@cat libft/includes/libft.h | grep -v 'libft.h' | grep 'void	' | grep 'ft_*' | sed 's/^/	+ /';
	@sleep 0.1;
	@cat libft/includes/libft.h | grep -v 'libft.h' | grep 'size_t	' | grep 'ft_*' | sed 's/^/	+ /';
	@sleep 0.1;
	@cat libft/includes/libft.h | grep -v 'libft.h' | grep 't_list	' | grep 'ft_*' | sed 's/^/	+ /';
	@sleep 0.1;
	@echo  "     _      __ _               _     _    __              ";
	@echo  "   _| |_   / _| |_    _ __ _ _(_)_ _| |_ / _|             ";
	@echo  "  |_   _| |  _|  _|  | '_ \ '_| | ' \  _|  _|             ";
	@echo  "    |_|   |_|  \__|__| .__/_| |_|_||_\__|_|               ";
	@echo  "                 |___|_|                                  ";
	@cat libft/includes/ft_printf.h | grep -v 'ft_printf.h' | grep 'ft_printf' | sed 's/^/	+ /';
	@sleep 0.1
	@echo  "     _              _                  _     _ _          ";
	@echo  "   _| |_   __ _ ___| |_   _ _  _____ _| |_  | (_)_ _  ___ ";
	@echo  "  |_   _| / _\` / -_)  _| | ' \/ -_) \ /  _| | | | ' \/ -_)";
	@echo  "    |_|   \__, \___|\__|_|_||_\___/_/_\\_|___|_|_|_||_\___|";
	@echo  "          |___/       |___|              |___|            ";
	@cat libft/includes/get_next_line.h | grep -v 'get_next_line.h' | grep 'get_next_line*' | sed 's/^/	+ /';
	@echo  "${RESET}";

msgft_mlx:
	@echo "                                                                                                                                                 ";
	@echo "                                                                                                                                                 ";
	@echo "FFFFFFFFFFFFFFFFFFFFFFTTTTTTTTTTTTTTTTTTTTTTT                        MMMMMMMM               MMMMMMMMLLLLLLLLLLL             XXXXXXX       XXXXXXX";
	@echo "F::::::::::::::::::::FT:::::::::::::::::::::T                        M:::::::M             M:::::::ML:::::::::L             X:::::X       X:::::X";
	@echo "F::::::::::::::::::::FT:::::::::::::::::::::T                        M::::::::M           M::::::::ML:::::::::L             X:::::X       X:::::X";
	@echo "FF::::::FFFFFFFFF::::FT:::::TT:::::::TT:::::T                        M:::::::::M         M:::::::::MLL:::::::LL             X::::::X     X::::::X";
	@echo "  F:::::F       FFFFFFTTTTTT  T:::::T  TTTTTT                        M::::::::::M       M::::::::::M  L:::::L               XXX:::::X   X:::::XXX";
	@echo "  F:::::F                     T:::::T                                M:::::::::::M     M:::::::::::M  L:::::L                  X:::::X X:::::X   ";
	@echo "  F::::::FFFFFFFFFF           T:::::T                                M:::::::M::::M   M::::M:::::::M  L:::::L                   X:::::X:::::X    ";
	@echo "  F:::::::::::::::F           T:::::T                                M::::::M M::::M M::::M M::::::M  L:::::L                    X:::::::::X     ";
	@echo "  F:::::::::::::::F           T:::::T                                M::::::M  M::::M::::M  M::::::M  L:::::L                    X:::::::::X     ";
	@echo "  F::::::FFFFFFFFFF           T:::::T                                M::::::M   M:::::::M   M::::::M  L:::::L                   X:::::X:::::X    ";
	@echo "  F:::::F                     T:::::T                                M::::::M    M:::::M    M::::::M  L:::::L                  X:::::X X:::::X   ";
	@echo "  F:::::F                     T:::::T                                M::::::M     MMMMM     M::::::M  L:::::L         LLLLLLXXX:::::X   X:::::XXX";
	@echo "FF:::::::FF                 TT:::::::TT                              M::::::M               M::::::MLL:::::::LLLLLLLLL:::::LX::::::X     X::::::X";
	@echo "F::::::::FF                 T:::::::::T                              M::::::M               M::::::ML::::::::::::::::::::::LX:::::X       X:::::X";
	@echo "F::::::::FF                 T:::::::::T                              M::::::M               M::::::ML::::::::::::::::::::::LX:::::X       X:::::X";
	@echo "FFFFFFFFFFF                 TTTTTTTTTTT                              MMMMMMMM               MMMMMMMMLLLLLLLLLLLLLLLLLLLLLLLLXXXXXXX       XXXXXXX";
	@echo "                                             ________________________                                                                            ";
	@echo "                                             _::::::::::::::::::::::_                                                                            ";
	@echo "                                             ________________________                                                                            ";
	@echo  "${RESET}";

.PHONY: all
