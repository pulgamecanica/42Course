#include "webserv.hpp"
#include "HTTPServer.hpp"

static void help(char *prog_name) {
	std::cout << RED << "usage " << prog_name << " config_file.conf" << std::endl;
	std::cout << YELLOW << "hints: check if configuration file exists." << ENDC << std::endl;
}

int	main(int ac, char **av)
{
	if (ac != 2) {
		help(av[0]);
		return (1);
	}
	try {
		HTTPServer s(av[1]);
		s.run();
	} catch (std::exception & e) {
		std::cout << RED << "Closing webserv | Exception: " << e.what() << ENDC << std::endl;
	}
	return (0);
}
