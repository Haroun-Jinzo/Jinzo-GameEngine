#pragma once

namespace List
{
	//find index of item in vector (list)
	template <typename T>
	int getIndexOf(std::vector<T> list, T item)
	{
		return std::find(list.begin(), list.end(), item) - list.begin();
	}

	//test if list contains item
	template <typename T>
	bool contains(std::vector<T> list, T item)
	{
		return std::find(list.begin(), list.end(), item) != list.end();
	}
	;
}