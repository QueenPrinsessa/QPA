
#include "QPA/LinkedList.hpp"
#include <cstdlib>

int main()
{
	QPA::LinkedList<int> list{ 1, 2, 3, 4, 5, 6, 7, 8};

	for(const auto& node : list)
	{
		printf("%d", node.Data);
	}
	return EXIT_SUCCESS;
}
