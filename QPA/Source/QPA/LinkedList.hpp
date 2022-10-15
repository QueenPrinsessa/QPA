#pragma once
#include "Types.h"
#include <cassert>
#include <utility>
#include <iterator>
#include <cstddef>

namespace QPA
{
	template <typename T>
	class LinkedList
	{
		using Value = T;
		using Ref = T&;
		using ConstRef = const T&;
		public:
			struct Node
			{
				friend class LinkedList<T>;
				friend struct Iterator;

				T Data;

				Node() = delete;

				private:
					template <typename ... TArgs>
					Node(Node* aNext, TArgs&&... aArgs)
						: Data(std::forward<TArgs>(aArgs)...), myNext(aNext) {}

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

			LinkedList() = default;
			LinkedList(std::initializer_list<T> aList);
			~LinkedList();

			Ref Front();
			ConstRef Front() const;
			Value FrontThenPop();

			void PushFront(const T& aData);
			void PopFront();

			template<typename ... TArgs>
			Ref EmplaceFront(TArgs&&... aArgs);
			
			void Clear() noexcept;
			bool Empty() const;

			void Swap(LinkedList<T>& aOther) noexcept;
			void Reverse() noexcept;

			u64 Size() const;

			Iterator begin() { return Iterator { myHead }; }
			Iterator end() { return Iterator { nullptr }; }
		private:
			Node* myHead { nullptr };
			u64 mySize { 0 };
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
	typename LinkedList<T>::Ref LinkedList<T>::Front()
	{
		assert(myHead && "Accessing nullptr element");
		return myHead->Data;
	}

	template <typename T>
	typename LinkedList<T>::ConstRef LinkedList<T>::Front() const
	{
		return Front();
	}

	template <typename T>
	typename LinkedList<T>::Value LinkedList<T>::FrontThenPop()
	{
		Value value = Front();
		PopFront();
		return value;
	}

	template <typename T>
	void LinkedList<T>::PushFront(const T& aData)
	{
		EmplaceFront(aData);
	}

	template <typename T>
	void LinkedList<T>::PopFront()
	{
		if(myHead)
		{
			Node* next = myHead->myNext;
			delete myHead;
			myHead = next;
			--mySize;
		}
	}

	template <typename T>
	template <typename ... TArgs>
	typename LinkedList<T>::Ref LinkedList<T>::EmplaceFront(TArgs&&... aArgs)
	{
		Node* node = new Node { myHead, std::forward<TArgs>(aArgs)... };
		myHead = node;
		++mySize;
		return myHead->Data;
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
		return mySize == 0;
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

	template <typename T>
	u64 LinkedList<T>::Size() const
	{
		return mySize;
	}
}
