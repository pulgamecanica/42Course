//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.hpp"

#define convTest(e0, e1, e2)	(e0 != 'f' && e0 != '\0') || (e0 == 'f' && e1 != '\0') || (e0 == '.' && e1 == 'f' && e2 != '\0')
#define numVals(num)		(num > std::numeric_limits<int>::max() || num < std::numeric_limits<int>::min())
#define floatVals(num)		(num + 0.00000001 > std::numeric_limits<float>::max() || num + 0.00000001 < std::numeric_limits<float>::min())
#define doubleVals(num)		(num + 0.00000001 > std::numeric_limits<double>::max() || num + 0.00000001 < std::numeric_limits<double>::min())
#define infVals(str)		(str == "nan" || str == "nanf" || str == "-inff" || str == "+inff" || str == "+inf" || str == "-inf")

bool isNumber(const std::string& str) {
	int len = str.size();
	for (int i = 0; i < len; i++) {
		if (std::isdigit(str[i]) == 0 && str[i] != '.' && str[i] != 'f')
			return false;
	}
	return true;
}

int	main(int ac, char **av) {
	char			*end;
	std::string		s;
	std::stringstream	ss;
	char			c;	
	int			num;
	float			numf;
	double			numd;

	if (ac != 2)
		return (0);
	long double tmp = strtod(av[1], &end);
	s = av[1];
	if (convTest(end[0], end[1], end[2]) || s == "inff" || s == "inf") {
		std::cout << "Invalid Parameter" << std::endl;
		return (0);
	}
	if (infVals(s))
	{
		numd = strtod(av[1], &end);
		numf = static_cast<float>(numd);

		std::cout << "char: Impossible!" << std::endl;
		std::cout << "int: Impossible!" << std::endl;
		std::cout << "float: " << numf << 'f' << std::endl;
		std::cout << "double: " << numd << std::endl;
		return (0);
	}
	// CHAR CONVERSION
	if (isNumber(s) || (s[0] == '-' && isNumber(&s[1]))) {
		c = static_cast<char>(tmp);
		if (isprint(c))
			std::cout << "char: '" << c << "'"<< std::endl;
		else
			std::cout << "char Non Printable" << std::endl;
	}
	else
		std::cout << "char: " << "Impossible!" << std::endl;
	// INT CONVERSION 
	if (numVals(tmp)) {
		std::cout << "int: overflow" << std::endl;
	}
	else {
		num = static_cast<int>(tmp);
		std::cout << "int: " << num << std::endl;
	}
	// FLOAT CONVERSION
	if (floatVals(tmp))
		std::cout << "float: overflow" << std::endl;
	else {
		numf = static_cast<float>(tmp);
		std::cout << "float: " << std::fixed << std::setprecision(1) << numf << "f" << std::endl;
	}
	// DOUBLE CONVERSION
	if (doubleVals(tmp))
		std::cout << "double: overflow" << std::endl;
	else {
		numd = static_cast<double>(tmp);
		std::cout << "double: " << std::fixed << std::setprecision(1) << numd << std::endl;
	}
	return (0);
}
