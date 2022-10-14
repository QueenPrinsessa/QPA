#pragma once
#include <cassert>
#include <utility>
#include <iterator>
#include <cstddef>

namespace QPA
{
	template <typename T>
	class LinkedList
	{
		using Ref = T&;
		using ConstRef = const T&;
		struct Node
		{
			friend class LinkedList<T>;
			friend struct Iterator;

			T Data;

			Node() = delete;

			private:
				Node(ConstRef aData, Node* aNext)
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

			Ref Front();
			ConstRef Front() const;

			void PushFront(const T& aData);
			void PopFront();

			template<typename ... TArgs>
			void EmplaceFront(TArgs&&... aArgs);

			void Clear() noexcept;
			bool Empty() const;

			void Swap(LinkedList<T>& aOther) noexcept;
			void Reverse() noexcept;

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
	T& LinkedList<T>::Front()
	{
		assert(myHead && "Accessing nullptr element");
		return myHead->Data;
	}

	template <typename T>
	const T& LinkedList<T>::Front() const
	{
		return Front();
	}

	template <typename T>
	void LinkedList<T>::PushFront(const T& aData)
	{
		Node* node = new Node { aData, myHead };
		myHead = node;
	}

	template <typename T>
	void LinkedList<T>::PopFront()
	{
		if(myHead)
		{
			Node* next = myHead->myNext;
			delete myHead;
			myHead = next;
		}
	}

	template <typename T>
	template <typename ... TArgs>
	void LinkedList<T>::EmplaceFront(TArgs&&... aArgs)
	{
		Node* node = new Node { T{ std::forward<TArgs>(aArgs)... }, myHead };
		myHead = node;
	}

	template <typename T>
	void LinkedList<T>::Clear() noexcept
	{
		Node* currentNode = myHead;
		while (currentNode)
		{
			Node* nextNode = currentNode->myNext;
			delete currentNode;
			currentNode = nextNode;
		}
	}

	template <typename T>
	bool LinkedList<T>::Empty() const
	{
		return static_cast<bool>(myHead);
	}

	template <typename T>
	void LinkedList<T>::Swap(LinkedList<T>& aOther) noexcept
	{
		T* tempHead = aOther.myHead;
		aOther.myHead = myHead;
		myHead = tempHead;
	}

	template <typename T>
	void LinkedList<T>::Reverse() noexcept
	{
		Node* current = myHead;
		Node* previous = nullptr;
		while(current)
		{
			Node* next = current->myNext;
			current->myNext = previous;
			previous = current;
			current = next;
		}
		myHead = previous;
	}
}
