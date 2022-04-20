//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Span.hpp"

Span::Span(): n(42) {
	v.reserve(42);
}

Span::Span(int num): n(num) {
	v.reserve(num);
}

Span::Span(const Span& param): n(param.n), v(param.v) {}

Span::~Span() {
	std::cout << "Span" << " destroyed" << std::endl;
}

Span& Span::operator= (const Span& param) {
	n = param.n;
	return (*this);
}

unsigned int Span::getN() const {return (n);}

void Span::addNumber(int num) {
	if (v.size() < n)
        v.push_back(num);
    else
        throw std::exception();
	std::sort(v.begin(), v.begin() + v.size());
}

void Span::printVec() const{
	std::vector<int> myvector(v);
	
	if (v.size() <= 0)
		return;
	std::cout << "	Vector: [ ";
	for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
		std::cout << "'" << *it << "' ";
	std::cout << "]" << std::endl;
}

void Span::addNumbers(std::vector<int> array) {
	v.insert(v.begin(), array.begin(), array.end());
	std::sort(v.begin(), v.begin() + v.size());
}


int Span::longestSpan() const {
	if (v.size() < 2)
		throw std::exception();
	return (v.back() - v.front());
}

int Span::shortestSpan(void) const {
	std::vector<int> myvector(v);
	int shortest = std::numeric_limits<int>::max();

	if (v.size() < 2)
		throw std::exception();
	for (std::vector<int>::iterator it = myvector.begin() ; it != myvector.end() - 1; ++it)
    	if (shortest > (*(it + 1)) - *it )
    		shortest = (*(it + 1) - *it);
  	return (shortest);
}

std::ostream& operator<<(std::ostream& s, const Span& param) {
	s << "Span<" << param.getN() << "> : [BIGGEST: " << param.longestSpan() << " | SMALLEST: "<< param.shortestSpan() << "]" << std::endl;
	param.printVec();
	return (s);
}
