//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ARRAY_HPP__
# define __ARRAY_HPP__

# define BLACK	"\033[0;30m"
# define RED	"\033[0;31m"
# define GREEN	"\033[0;32m"
# define YELLOW	"\033[0;33m"
# define BLUE	"\033[0;34m"
# define WHITE	"\033[0;37m"
# define ENDC	"\033[0m"

#include <iostream>
#include <exception>

template <typename type>
class Array {
	type *array;
	std::size_t len;
	public:
		Array();
		Array(std::size_t);
		Array(const Array&);
		~Array();
		Array&	operator= (const Array&);
		type &operator[](unsigned int i);
    	type const &operator[](unsigned int i) const;
		std::size_t size(void) const;
    	const type *ptr(void) const;
};

template <typename type>
std::ostream&	operator<<(std::ostream& ss, const Array<type>& param) {
	ss << "Array: [";

	for (std::size_t i = 0; i < param.size(); i++)
	{
		ss << "'" << param[i] << "'";
		if (i != param.size() - 1)
			ss << ", ";
	}
	ss << "]" << std::endl;
	return (ss);
}

template <typename type>
Array<type>::Array(): array(new type[0]), len(0) {}

template <typename type>
Array<type>::Array(std::size_t num): array(new type[num]), len(num) {}

template <typename type>
Array<type>::Array(const Array& param): array(new type[param.len]), len(param.len) {
	for (std::size_t i = 0; i < len; i++)
		array[i] = param.array[i];
}

template <typename type>
Array<type>::~Array() {
	delete[](array);
}

template <typename type>
Array<type>& Array<type>::operator= (const Array& param) {
	len = param.len;
	delete[] (array);
	for (int i = 0; i < len; i++)
		array[i] = param.array[i];
}

template <typename type>
std::size_t Array<type>::size() const {
	return (len);
}

template <typename type>
const type *Array<type>::ptr(void) const {
	return array;
}

template <typename type>
type &Array<type>::operator[](unsigned int i) {
	if (i >= len)
		throw std::exception();
	return (array[i]);
}

template <typename type>
type const &Array<type>::operator[](unsigned int i) const {
	if (i >= len)
		throw std::exception();
	return (array[i]);
}

#endif
