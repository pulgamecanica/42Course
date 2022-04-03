
//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "sed_is_for_losers.hpp"

int	main(int ac, char **av)
{
	std::size_t sub;
	std::string line;
	std::string filename;
	int i(0);
	if (ac != 4)
	{
		std::cout << "Usage ./sed_is_for_losers <filename> <word:string> <replace:string>" << std::endl;
		return (0);
	}
	filename = av[1];
	filename += ".replace";
	std::ifstream file_r(av[1]);
	if (!file_r.is_open() || file_r.fail())
	{
		std::cout << "File cannot be opened/doesn't exist..." << std::endl;
		return (0);
	}
	std::ofstream file_w(filename);
	if (!file_w.is_open() || file_w.fail())
	{
		std::cout << "File cannot be opened/doesn't exist..." << std::endl;
		return (0);
	}
	std::cout << "Reading File... Replacing all..." << std::endl;
	std::string word(av[2]);
	std::string replace(av[3]);
	while (getline(file_r, line))
	{
		i = 0;
		while (1)
		{
			sub = line.find(av[2], i);
			if  (sub != std::string::npos)
			{
				file_w << line.substr(i, sub - i) << replace;
				i = sub + word.length();
			}
			else
				break ;
		}
		file_w << &line[i] << std::endl;
	}
	std::cout << "Finished :D" << std::endl;
	return (0);
}
