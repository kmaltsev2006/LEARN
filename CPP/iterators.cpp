#include <iostream>
#include <iterator>
#include <forward_list>
#include <vector>

template <typename Iterator>
typename std::iterator_traits<Iterator>::difference_type
distance(Iterator first, Iterator last)
{
	if constexpr (std::is_base_of_v<
			std::random_access_iterator_tag,
			typename  std::iterator_traits<Iterator>::iterator_category // Iterator::iterator_category
			>)
	{
		return last - first;
	}

	size_t n = 0;
	for (; first != last; ++n, ++first);
	return n;
}

template<typename T>
void foo(T) = delete; // get type

int main()
{
	std::vector<int> v{5};
	std::cout << ::distance(v.begin(), v.end()) << std::endl; // Const
	
	std::forward_list<int> m = {1, 2, 3};
	std::cout << ::distance(m.begin(), m.end()) << std::endl; // Linear
}