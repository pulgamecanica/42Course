//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex01.hpp"


uintptr_t serialize(Data* ptr) {
	return reinterpret_cast<uintptr_t>(ptr);
}

Data* deserialize(uintptr_t raw) {
	return reinterpret_cast<Data*>(raw);
}

int	main(void)
{
	{
		Data d;
		d.lang = "rust";
		std::cout << "Original: " <<  d.lang << std::endl;
		std::cout << "Serialization: " << serialize(&d) << std::endl;
		std::cout << "Desserializarion: " << deserialize(serialize(&d))->lang << std::endl;
	}
	{
		Data d;
		d.lang = "ruby";
		std::cout << "Original: " << d.lang << std::endl;
		std::cout << "Serialization: " << serialize(&d) << std::endl;
                std::cout << "Desserializarion: " << deserialize(serialize(&d))->lang << std::endl;
	}
	{
		Data d;
		Data *d2;
		d.lang = "perl";
		std::cout << "Original: " << d.lang << std::endl;
		std::cout << "Serialization: " << serialize(&d) << std::endl;
		d2 = deserialize(serialize(&d)); 
		std::cout << "Desserializarion: " << d2->lang << std::endl;
	}
	return (0);
}
