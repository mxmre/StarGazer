#include "Asserts.h"


void sg::exceptions::ErrorAssert(bool condition, const std::wstring& msg)
{
	if (!condition)
		throw SGException(msg);
}

void sg::exceptions::Error(const std::wstring& msg)
{
	sg::exceptions::ErrorAssert(true, msg);
}

void sg::exceptions::FatalErrorAssert(bool condition, const std::wstring& msg)
{
	if (!condition)
		throw SGFatalException(msg);
}

void sg::exceptions::FatalError(const std::wstring& msg)
{
	sg::exceptions::FatalErrorAssert(true, msg);
}

void sg::exceptions::ErrorAssert(bool condition, const std::string& msg)
{
	sg::exceptions::ErrorAssert(condition, std::wstring(msg.begin(), msg.end()));
}

void sg::exceptions::Error(const std::string& msg)
{
	sg::exceptions::ErrorAssert(true, msg);
}

void sg::exceptions::FatalErrorAssert(bool condition, const std::string& msg)
{
	sg::exceptions::FatalErrorAssert(condition, std::wstring(msg.begin(), msg.end()));
}

void sg::exceptions::FatalError(const std::string& msg)
{
	sg::exceptions::FatalErrorAssert(true, msg);
}

