#pragma once
#include "BadAllocationException.h"
#include "LinkedList.hpp"
#include "Types.h"
#include <cmath>

namespace QPA
{
	/**
	 * \brief A pool allocator.
	 * \tparam TChunk Object type to pool allocate
	 * \tparam TChunkCount Object count per block
	 * \tparam Growable Is the pool allowed to automatically allocate more blocks?
	 */
	template<typename TChunk, u64 TChunkCount, bool Growable = true>
	class PoolAllocator final
	{
		struct Block
		{
			TChunk* Chunks {};
		};
		public:
			constexpr PoolAllocator();
			explicit constexpr PoolAllocator(u64 aStartChunkCount);
			~PoolAllocator();

			/**
			 * \brief Constructs an object from this pool
			 * \param aArgs Arguments passed to the constructor of the object.
			 * \return Pointer to an object constructed by this pool
			 */
			template <typename ... TArgs>
			constexpr [[nodiscard]] TChunk* Construct(TArgs&&... aArgs);

			/**
			 * \brief Deconstructs and deallocates a chunk of memory constructed by this allocator
			 * \param aObject Pointer to a chunk of memory constructed by this allocator
			 */
			constexpr void Destroy(TChunk* aObject);

			/**
			 * \brief Gets the next chunk of memory available
			 * \return Pointer to the uninitialized chunk memory.
			 */
			constexpr [[nodiscard]] TChunk* Allocate();

			/**
			 * \brief Makes a chunk of memory available again.
			 * \param aChunk Pointer to a chunk of memory allocated by this allocator
			 */
			constexpr void Deallocate(TChunk* aChunk);

			/**
			 * \brief Allocates blocks of memory until the allocators capacity is >= aChunkCount.
			 * \param aChunkCount A chunk count to reserve memory for.
			 */
			constexpr void Reserve(u64 aChunkCount);

			/**
			 * \return Number of free chunks left
			 */
			constexpr u64 Size() const;

			/**
			 * \return Total number of chunks that can be allocated without allocating a new block.
			 */
			constexpr u64 Capacity() const;

			/**
			 * \return Whether pool can automatically allocate more blocks if it runs out of free chunks.
			 */
			constexpr bool IsGrowable() const;
		private:
			LinkedList<Block> myBlocks {};
			LinkedList<TChunk*> myFreeChunks {};

			constexpr void AllocateBlock();
			
	};

	template <typename TChunk, u64 TChunkCount, bool Growable>
	constexpr PoolAllocator<TChunk, TChunkCount, Growable>::PoolAllocator()
	{
		AllocateBlock();
	}

	template <typename TChunk, u64 TChunkCount, bool Growable>
	constexpr PoolAllocator<TChunk, TChunkCount, Growable>::PoolAllocator(u64 aStartChunkCount)
	{
		Reserve(aStartChunkCount);
	}

	template <typename TChunk, u64 TChunkCount, bool Growable>
	PoolAllocator<TChunk, TChunkCount, Growable>::~PoolAllocator()
	{
		for(typename LinkedList<Block>::Node node: myBlocks)
		{
			free(node.Data.Chunks);
		}
	}

	template <typename TChunk, u64 TChunkCount, bool Growable>
	template <typename ... TArgs>
	constexpr TChunk* PoolAllocator<TChunk, TChunkCount, Growable>::Construct(TArgs&&... aArgs)
	{
		TChunk* object = Allocate();
		new(object) TChunk{ std::forward<TArgs>(aArgs)... };
		return object;
	}

	template <typename TChunk, u64 TChunkCount, bool Growable>
	constexpr void PoolAllocator<TChunk, TChunkCount, Growable>::Destroy(TChunk* aObject)
	{
		if(!aObject)
		{
			return;
		}

		aObject->~TChunk();
		Deallocate(aObject);
	}

	template <typename TChunk, u64 TChunkCount, bool Growable>
	constexpr TChunk* PoolAllocator<TChunk, TChunkCount, Growable>::Allocate()
	{
		if (myFreeChunks.Empty())
		{
			if constexpr (Growable)
			{
				AllocateBlock();
			}
			else
			{
				throw BadAllocation("Bad allocation. Pool is out of memory.");
			}
		}
		
		return myFreeChunks.FrontThenPop();
	}

	template <typename TChunk, u64 TChunkCount, bool Growable>
	constexpr void PoolAllocator<TChunk, TChunkCount, Growable>::Deallocate(TChunk* aChunk)
	{
		if(aChunk)
		{
			myFreeChunks.EmplaceFront(aChunk);
		}

	}

	template <typename TChunk, u64 TChunkCount, bool Growable>
	constexpr void PoolAllocator<TChunk, TChunkCount, Growable>::Reserve(u64 aChunkCount)
	{
		while(Capacity() < aChunkCount)
		{
			AllocateBlock();
		}
	}

	template <typename TChunk, u64 TChunkCount, bool Growable>
	constexpr u64 PoolAllocator<TChunk, TChunkCount, Growable>::Size() const
	{
		return myFreeChunks.Size();
	}

	template <typename TChunk, u64 TChunkCount, bool Growable>
	constexpr u64 PoolAllocator<TChunk, TChunkCount, Growable>::Capacity() const
	{
		return TChunkCount * myBlocks.Size();
	}

	template <typename TChunk, u64 TChunkCount, bool Growable>
	constexpr bool PoolAllocator<TChunk, TChunkCount, Growable>::IsGrowable() const
	{
		return Growable;
	}

	template <typename TChunk, u64 TChunkCount, bool Growable>
	constexpr void PoolAllocator<TChunk, TChunkCount, Growable>::AllocateBlock()
	{
		Block& block = myBlocks.EmplaceFront();
		block.Chunks = static_cast<TChunk*>(malloc(sizeof(TChunk) * TChunkCount));

		if(block.Chunks)
		{
			for(u64 chunkIndex = 0; chunkIndex < TChunkCount; ++chunkIndex)
			{
				myFreeChunks.EmplaceFront(&block.Chunks[chunkIndex]);
			}
		}
		else
		{
			myBlocks.PopFront();
			throw BadAllocation("Bad allocation. Failed to allocate memory for block.");
		}
	}
}
