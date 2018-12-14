#include <iostream>
#include <array>

template<
	typename T,
	unsigned int firstSize,
	unsigned int... sizes
>
class Matrix{
	std::array<Matrix<T, sizes...>, firstSize> data;
	
public:
	
	Matrix<T, sizes...>& operator[](unsigned int index){
		return data[index];
	}

	constexpr unsigned int getDimensionCount() const {
		return sizeof...(sizes) + 1;
	}
	
};

template<
	typename T,
	unsigned int size
>
class Matrix<T, size> {
	std::array<T, size> data;
	
public:

	T& operator[](unsigned int index){
		return data[index];
	}

	constexpr unsigned int getDimensionCount() const {
		return 1;
	}
	
};