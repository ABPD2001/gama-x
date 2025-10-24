#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <string>
#include <vector>
#include <cstdint>

using std::string;
using std::uint8_t;
using std::vector;

template <typename T = string>
signed int find(vector<T> arr, T elm)
{
	for (uint8_t i = 0; i < arr.size(); i++)
	{
		if (arr[i] == elm)
			return i;
	}
	return -1;
};

#endif