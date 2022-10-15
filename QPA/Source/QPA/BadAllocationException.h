#pragma once
#include <exception>

namespace QPA
{
	class BadAllocation : public std::exception
	{
		public:
			BadAllocation()
				: std::exception("Bad allocation.") {}

			BadAllocation(char const* const aMessage)
				: std::exception(aMessage) {}
	};
}
