//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __GRAPH_HPP__
# define __GRAPH_HPP__

#include <iostream>
#include <vector>

#include "Vector2.hpp"

class Graph {
	public:
		Graph(const Vector2& size);
		~Graph();
		const Vector2& get_size() const;
		bool add_point(const Vector2& p);
		void put_to_screen() const;
		bool has_point(const Vector2& p) const;
	private:
		const Vector2 size_;
		std::vector<Vector2> points_;
};

std::ostream&	operator<<(std::ostream&, const Graph&);

#endif

