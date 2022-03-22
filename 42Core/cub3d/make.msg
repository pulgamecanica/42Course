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
	@echo  ""
	@echo "LLLLLLLLLLL               iiii bbbbbbbb               ffffffffffffffff           tttt          ";
	@echo "L:::::::::L              i::::ib::::::b              f::::::::::::::::f       ttt:::t          ";
	@echo "L:::::::::L               iiii b::::::b             f::::::::::::::::::f      t:::::t          ";
	@echo "LL:::::::LL                     b:::::b             f::::::fffffff:::::f      t:::::t          ";
	@echo "  L:::::L               iiiiiii b:::::bbbbbbbbb     f:::::f       ffffffttttttt:::::ttttttt    ";
	@echo "  L:::::L               i:::::i b::::::::::::::bb   f:::::f             t:::::::::::::::::t    ";
	@echo "  L:::::L                i::::i b::::::::::::::::b f:::::::ffffff       t:::::::::::::::::t    ";
	@echo "  L:::::L                i::::i b:::::bbbbb:::::::bf::::::::::::f       tttttt:::::::tttttt    ";
	@echo "  L:::::L                i::::i b:::::b    b::::::bf::::::::::::f             t:::::t          ";
	@echo "  L:::::L                i::::i b:::::b     b:::::bf:::::::ffffff             t:::::t          ";
	@echo "  L:::::L                i::::i b:::::b     b:::::b f:::::f                   t:::::t          ";
	@echo "  L:::::L         LLLLLL i::::i b:::::b     b:::::b f:::::f                   t:::::t    tttttt";
	@echo "LL:::::::LLLLLLLLL:::::Li::::::ib:::::bbbbbb::::::bf:::::::f                  t::::::tttt:::::t";
	@echo "L::::::::::::::::::::::Li::::::ib::::::::::::::::b f:::::::f                  tt::::::::::::::t";
	@echo "L::::::::::::::::::::::Li::::::ib:::::::::::::::b  f:::::::f                    tt:::::::::::tt";
	@echo "LLLLLLLLLLLLLLLLLLLLLLLLiiiiiiiibbbbbbbbbbbbbbbb   fffffffff                      ttttttttttt  ";
	@echo "     _          _   _ _    ";
	@echo "   _| |_   _  _| |_(_) |___";
	@echo "  |_   _| | || |  _| | (_-<";
	@echo "    |_|    \_,_|\__|_|_/__/";
	@echo "                           ";
	@cat libft/includes/libft.h | grep -v 'libft.h' | grep 'int	' | grep 'ft_*' | sort | sed 's/^/	->	/';
	@cat libft/includes/libft.h | grep -v 'libft.h' | grep 'char	' | grep 'ft_*' | sort | sed 's/^/	->	/';
	@cat libft/includes/libft.h | grep -v 'libft.h' | grep 'void	' | grep 'ft_*' | sort | sed 's/^/	->	/';
	@cat libft/includes/libft.h | grep -v 'libft.h' | grep 'size_t	' | grep 'ft_*' | sort | sed 's/^/	->	/';
	@cat libft/includes/libft.h | grep -v 'libft.h' | grep 't_list	' | grep 'ft_*' | sort | sed 's/^/	->	/';
	@echo "     _      __ _               _     _    __              ";
	@echo "   _| |_   / _| |_    _ __ _ _(_)_ _| |_ / _|             ";
	@echo "  |_   _| |  _|  _|  | '_ \ '_| | ' \  _|  _|             ";
	@echo "    |_|   |_|  \__|__| .__/_| |_|_||_\__|_|               ";
	@echo "                 |___|_|                                  ";
	@cat libft/includes/ft_printf.h | grep -v 'ft_printf.h' | grep 'ft_printf' | sed 's/^/	->	/';
	@echo "     _              _                  _     _ _          ";
	@echo "   _| |_   __ _ ___| |_   _ _  _____ _| |_  | (_)_ _  ___ ";
	@echo "  |_   _| / _\` / -_)  _| | ' \/ -_) \ /  _| | | | ' \/ -_)";
	@echo "    |_|   \__, \___|\__|_|_||_\___/_/_\\_|___|_|_|_||_\___|";
	@echo "          |___/       |___|              |___|            ";
	@cat libft/includes/get_next_line.h | grep -v 'get_next_line.h' | grep 'get_next_line*' | sed 's/^/	->	/';

msgft_mlx:
	@echo ""
	@echo "MMMMMMMM               MMMMMMMMLLLLLLLLLLL             XXXXXXX       XXXXXXX";
	@echo "M:::::::M             M:::::::ML:::::::::L             X:::::X       X:::::X";
	@echo "M::::::::M           M::::::::ML:::::::::L             X:::::X       X:::::X";
	@echo "M:::::::::M         M:::::::::MLL:::::::LL             X::::::X     X::::::X";
	@echo "M::::::::::M       M::::::::::M  L:::::L               XXX:::::X   X:::::XXX";
	@echo "M:::::::::::M     M:::::::::::M  L:::::L                  X:::::X X:::::X   ";
	@echo "M:::::::M::::M   M::::M:::::::M  L:::::L                   X:::::X:::::X    ";
	@echo "M::::::M M::::M M::::M M::::::M  L:::::L                    X:::::::::X     ";
	@echo "M::::::M  M::::M::::M  M::::::M  L:::::L                    X:::::::::X     ";
	@echo "M::::::M   M:::::::M   M::::::M  L:::::L                   X:::::X:::::X    ";
	@echo "M::::::M    M:::::M    M::::::M  L:::::L                  X:::::X X:::::X   ";
	@echo "M::::::M     MMMMM     M::::::M  L:::::L         LLLLLLXXX:::::X   X:::::XXX";
	@echo "M::::::M               M::::::MLL:::::::LLLLLLLLL:::::LX::::::X     X::::::X";
	@echo "M::::::M               M::::::ML::::::::::::::::::::::LX:::::X       X:::::X";
	@echo "M::::::M               M::::::ML::::::::::::::::::::::LX:::::X       X:::::X";
	@echo "MMMMMMMM               MMMMMMMMLLLLLLLLLLLLLLLLLLLLLLLLXXXXXXX       XXXXXXX";
	@echo ""
	@echo "	******************";
	@echo "	********MLX*******";
	@echo "	******************";
	@echo "		->	void	*mlx_init ();";
	@echo "	******************";
	@echo "	**** MLX LOOP ****";
	@echo "	******************";
	@echo "		->	int	mlx_loop ( void *mlx_ptr );";
	@echo "		->	int	mlx_key_hook ( void *win_ptr, int (*funct_ptr)(), void *param );";
	@echo "		->	int	mlx_mouse_hook ( void *win_ptr, int (*funct_ptr)(), void *param );";
	@echo "		->	int	mlx_expose_hook ( void *win_ptr, int (*funct_ptr)(), void *param );"
	@echo "		->	int	mlx_loop_hook ( void *mlx_ptr, int (*funct_ptr)(), void *param );";
	@echo "	******************";
	@echo "	*** MLX IMAGES ***";
	@echo "	******************";
	@echo "		->	void		*mlx_new_image ( void *mlx_ptr, int width, int height );";
	@echo "		->	char		*mlx_get_data_addr ( void *img_ptr, int *bits_per_pixel, int *size_line, int *endian );";
	@echo "		->	int		mlx_put_image_to_window ( void *mlx_ptr, void *win_ptr, void *img_ptr, int x, int y );";
	@echo "		->	unsigned int	mlx_get_color_value ( void *mlx_ptr, int color );";
	@echo "		->	void		*mlx_xpm_to_image ( void *mlx_ptr, char **xpm_data, int *width, int *height );";
	@echo "		->	void		*mlx_xpm_file_to_image ( void *mlx_ptr, char *filename, int *width, int *height );";
	@echo "		->	int		mlx_destroy_image ( void *mlx_ptr, void *img_ptr );";
	@echo "	******************";
	@echo "	*** MLX WINDOW ***";
	@echo "	******************";
	@echo "		->	void	*mlx_new_window ( void *mlx_ptr, int size_x, int size_y, char *title );"; 
	@echo "		->	int	mlx_clear_window ( void *mlx_ptr, void *win_ptr );";
	@echo "		->	int	mlx_destroy_window ( void *mlx_ptr, void *win_ptr );";
	@echo "	******************";
	@echo "	*** MLX  PIXEL ***";
	@echo "	******************";
	@echo "		->	int	mlx_pixel_put ( void *mlx_ptr, void *win_ptr, int x, int y, int color );";
	@echo "		->	int	mlx_string_put ( void *mlx_ptr, void *win_ptr, int x, int y, int color, char *string );";

.PHONY: all
