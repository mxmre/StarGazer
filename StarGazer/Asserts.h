#pragma once
#include <string>
#define DEBUG_MSG(text) std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string(text)
namespace sg
{
	namespace exceptions
	{
		void ErrorAssert(bool condition, const std::string& msg);
		void Error(const std::string& msg);

	}
}