//***************************//
//*Template by pulgamecanica*//
//***************************//
extern "C" {
	#include <sys/ioctl.h>
	#include <unistd.h>
}

#include <sstream>

#include "ex01.inc"
#include "Graph.hpp"

Graph::Graph(const Vector2& size): size_(size) {
	std::cout << BLUE << "Graph\t" << GREEN << "NEW\t" << ENDC << size_ << std::endl;
}

Graph::~Graph() {
	points_.clear();
	std::cout << BLUE << "Graph\t" << GREEN << "DELETE\t" << ENDC << size_ << std::endl;
}

const Vector2& Graph::get_size() const {
	return size_;
}

bool Graph::has_point(const Vector2& p) const {
	(void)p;
	for (std::vector<Vector2>::const_iterator i = points_.begin(); i != points_.end(); ++i) {
		if (*i == p)
			return true;
	}
	return false;
}

void Graph::put_to_screen() const {
	struct winsize w;
	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
		std::cerr << "ioctl could not find a valid window" << std::endl;
	Vector2 tsize(w.ws_col, w.ws_row);
	if (tsize.x() <= 0 || tsize.y() <= 0 || tsize.x() < size_.x() * 2 || tsize.y() < size_.y() * 2)
		std::cerr << YELLOW << "WARNING!\t" << ENDC << "The output resolution is not big enough" << std::endl;
	int padding_y = 2;
	int padding_x = 2;
	for (int y = size_.y(); y >= 0; --y) {
		std::cout << "&";
		std::cout.width(padding_y);
		std::cout << y;
		std::cout.width(0);
		for (int x = 0; x <= size_.x(); ++x) {
			std::cout << " ";
			if (has_point(Vector2(x, y)))
				std::cout << "X";
			else
				std::cout << ".";
		}
		std::cout << std::endl;
	}
	// Footer
	for (int i = 0; i < padding_x; ++i) {
		std::cout << "&   ";
		for (int j = 0; j <= size_.x(); ++j) {
			std::ostringstream sstr;
			sstr << std::dec << j;
			std::string snum = sstr.str();
			if (i < (int)snum.length())
				std::cout << snum[i];
			else
				std::cout << " ";
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}

/**
 * Returns true if the point was added succesfully
 * Returns false if the point is not in the bounds of the graph
 **/
bool Graph::add_point(const Vector2 & p) {
	if (p.x() > size_.x() || p.x() < 0 || p.y() > size_.y() || p.x() < 0) {
		std::cout << BLUE << "Graph\t" << RED << "ADD FAIL\t" << ENDC << p << std::endl;
		return false;
	}
	std::cout << BLUE << "Graph\t" << GREEN << "ADD\t" << ENDC << p << std::endl;
	points_.push_back(p);
	return true;
}

std::ostream& operator<<(std::ostream& s, const Graph& g) {
	s << BLUE << "Graph " << ENDC << "size: " << g.get_size();
	return (s);
}

