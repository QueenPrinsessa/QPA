
#include "QPA/LinkedList.hpp"
#include <cstdlib>

int main()
{

	struct Test
	{
		int x;
		int y;

		Test(const Test&) = default;

		Test(int aX, int aY)
		{
			x = aX;
			y = aY;
			static int constructorCalls = 0;
			constructorCalls++;
			printf("Constructor Calls: %d\n", constructorCalls);
		}
		~Test()
		{
			static int deconstructorCalls = 0;
			deconstructorCalls++;
			printf("Deconstructor Calls: %d\n", deconstructorCalls);
		}
	};

	printf("--------Test 1---------\n");
	QPA::LinkedList<Test> list { { 1, 2 }, { 3, 4 }, { 5, 6}, { 7, 8 }, { 9, 10 } };

	for(const auto& node : list)
	{
		printf("x: %d y: %d\n", node.Data.x, node.Data.y);
	}

	printf("--------Test 2---------\n");
	QPA::LinkedList<Test> list2;
	list2.EmplaceFront(3, 9);
	list2.EmplaceFront(9, 7);
	list2.EmplaceFront(5, 9);
	list2.EmplaceFront(9, 3);
	list2.EmplaceFront(1, 9);

	for (const auto& node : list2)
	{
		printf("x: %d y: %d\n", node.Data.x, node.Data.y);
	}

	printf("--------Test 3---------\n");
	QPA::LinkedList<Test> list3;
	list3.PushFront(Test{3, 9});
	list3.PushFront(Test{9, 7});
	list3.PushFront(Test{5, 9});
	list3.PushFront(Test{9, 3});
	list3.PushFront(Test { 1, 9 });

	for (const auto& node : list3)
	{
		printf("x: %d y: %d\n", node.Data.x, node.Data.y);
	}
	return EXIT_SUCCESS;
}
