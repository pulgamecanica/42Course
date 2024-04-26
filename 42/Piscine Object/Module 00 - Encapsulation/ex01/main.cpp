//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex01.inc"
#include "Vector2.hpp"
#include "Graph.hpp"

int	main(void)
{
	Vector2 v1(0, 0);
	Vector2 v2(1.1, 0.5);
	Vector2 v3(2.4, 5.2);
	Vector2 v4(4.2, 4.2);
	Vector2 v5(2.2, 8.9);

	std::cout << YELLOW << "- - - - - - - - - - - VECTORS - - - - - - - - - - " << ENDC << std::endl;
	std::cout << "(" << GREEN << v1 << ENDC << ")"<< std::endl;
	std::cout << "(" << RED << v2 << ENDC << ")"<< std::endl;
	std::cout << "(" << YELLOW << v3 << ENDC << ")"<< std::endl;
	std::cout << "(" << BLUE << v4 << ENDC << ")"<< std::endl;
	std::cout << "(" << WHITE << v5 << ENDC << ")"<< std::endl;
	std::cout << YELLOW << "- - - - - - - - - - GRAPH 42x10 - - - - - - - - - " << ENDC << std::endl;
	Vector2 s(42.0, 10.0);
	Graph g(s);
	g.add_point(v1);
	g.add_point(v2);
	g.add_point(v3);
	g.add_point(Vector2(42, 42));
	std::cout << YELLOW << "- - - - - - - - - - VISUALIZE - - - - - - - - - " << ENDC << std::endl;
	g.put_to_screen();
	std::cout << YELLOW << "- - - - - - - - - - - - - - - - - - - - - - - - " << ENDC << std::endl;
	// std::cout << g << std::endl;
	return (0);
}
