#pragma once
#include <string>
#include "SGException.h"
#include "SGFatalException.h"
#define DEBUG_MSG(text) (std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string((text)))
namespace sg
{
	namespace exceptions
	{
		void ErrorAssert(bool condition, const std::wstring& msg);
		void Error(const std::wstring& msg);
		void FatalErrorAssert(bool condition, const std::wstring& msg);
		void FatalError(const std::wstring& msg);

		void ErrorAssert(bool condition, const std::string& msg);
		void Error(const std::string& msg);
		void FatalErrorAssert(bool condition, const std::string& msg);
		void FatalError(const std::string& msg);
	}
}