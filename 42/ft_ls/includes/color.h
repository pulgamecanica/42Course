#ifndef __COLOR__
# define __COLOR__

// Verified
// \033[ ESCAPE
// ESCAPE - SET BOLD - SET COLOR BLUE
# define BLUEBOLD "\033[01;34m"
# define GREENBOLD "\033[01;32m"
# define CYANBOLD "\033[01;36m"
# define PURPLEBOLD "\033[01;35m"
# define YELLOW_DARK_BG "\033[40;33m"
# define GRAY_RED_BG "\033[37;41m"
# define BLACK_YELLOW_BG "\033[30;43m"

// Not verified
# define BOLD "\033[1m"
# define BLACK  "\033[0;30m"
# define BLACKBOLD  "\033[0;30m\033[1m"
# define RED  "\033[0;31m"
# define REDBOLD  "\033[0;31m\033[1m"
# define GREEN  "\033[0;32m"
# define YELLOW "\033[0;33m"
# define YELLOWBOLD "\033[0;33m\033[1m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE  "\033[0;37m"
# define WHITEBOLD  "\033[0;37m\033[1m"
# define ENDC "\033[0m"

#endif
