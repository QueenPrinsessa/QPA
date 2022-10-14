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
			T Data;
			Node* Next { nullptr };
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
			Iterator& operator++() { myPtr = myPtr->Next; return *this; }
			Iterator operator++(int) { Iterator prevValue = *this; myPtr = myPtr->Next; return prevValue; }

			friend bool operator==(const Iterator& aFirst, const Iterator& aSecond) { return aFirst.myPtr == aSecond.myPtr; }
			friend bool operator!=(const Iterator& aFirst, const Iterator& aSecond) { return aFirst.myPtr != aSecond.myPtr; }
			private:
				pointer myPtr;
		};

		public:
			LinkedList() = default;
			LinkedList(std::initializer_list<T> aList);
			~LinkedList();

			const Node* Head() { return myHead; }

			void Push(const T& aData);

			template<typename ... TArgs>
			void Emplace(TArgs&&... aArgs);

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
			Push(*it);
		}
	}

	template <typename T>
	LinkedList<T>::~LinkedList()
	{
		Clear();
	}

	template <typename T>
	void LinkedList<T>::Push(const T& aData)
	{
		Node* node = new Node { aData, myHead };
		myHead = node;
	}

	template <typename T>
	template <typename ... TArgs>
	void LinkedList<T>::Emplace(TArgs&&... aArgs)
	{
		Node* node = new Node { std::move(T(std::forward<TArgs>(aArgs)...)), myHead };
		myHead = node;
	}

	template <typename T>
	void LinkedList<T>::Clear()
	{
		Node* currentNode = myHead;
		while (currentNode)
		{
			Node* nextNode = currentNode->Next;
			delete currentNode;
			currentNode = nextNode;
		}
	}
}
