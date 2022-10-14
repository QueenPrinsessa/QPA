#pragma once
#include <utility>
#include <iterator>
#include <cstddef>

namespace QPA
{
	template <typename T>
	class LinkedList
	{
		struct Node
		{
			friend class LinkedList<T>;
			friend struct Iterator;

			T Data;

			Node() = delete;

			private:
				Node(const T& aData, Node* aNext)
					: Data(aData), myNext(aNext) {}
				Node* myNext { nullptr };
		};

		struct Iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Node;
			using pointer = Node*;
			using reference = Node&;

			Iterator(pointer aPtr)
				: myPtr(aPtr) {}

			reference operator*() const { return *myPtr; }
			pointer operator->() { return myPtr; }
			Iterator& operator++() { myPtr = myPtr->myNext; return *this; }
			Iterator operator++(int) { Iterator prevValue = *this; myPtr = myPtr->myNext; return prevValue; }

			friend bool operator==(const Iterator& aFirst, const Iterator& aSecond) { return aFirst.myPtr == aSecond.myPtr; }
			friend bool operator!=(const Iterator& aFirst, const Iterator& aSecond) { return aFirst.myPtr != aSecond.myPtr; }
			private:
				pointer myPtr;
		};

		public:
			LinkedList() = default;
			LinkedList(std::initializer_list<T> aList);
			~LinkedList();

			void PushFront(const T& aData);

			template<typename ... TArgs>
			void EmplaceFront(TArgs&&... aArgs);

			void Clear();

			Iterator begin() { return Iterator { myHead }; }
			Iterator end() { return Iterator { nullptr }; }
		private:
			Node* myHead { nullptr };
	};

	template <typename T>
	LinkedList<T>::LinkedList(std::initializer_list<T> aList)
	{
		for(auto it = std::rbegin(aList); it != std::rend(aList); ++it)
		{
			PushFront(*it);
		}
	}

	template <typename T>
	LinkedList<T>::~LinkedList()
	{
		Clear();
	}

	template <typename T>
	void LinkedList<T>::PushFront(const T& aData)
	{
		Node* node = new Node { aData, myHead };
		myHead = node;
	}

	template <typename T>
	template <typename ... TArgs>
	void LinkedList<T>::EmplaceFront(TArgs&&... aArgs)
	{
		Node* node = new Node { T{ std::forward<TArgs>(aArgs)... }, myHead
	};
		myHead = node;
	}

	template <typename T>
	void LinkedList<T>::Clear()
	{
		Node* currentNode = myHead;
		while (currentNode)
		{
			Node* nextNode = currentNode->myNext;
			delete currentNode;
			currentNode = nextNode;
		}
	}
}
