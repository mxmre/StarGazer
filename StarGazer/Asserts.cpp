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

void sg::exceptions::FatalErrorAssert(bool condition, const std::wstring& msg, int crush_code)
{
	if (!condition)
		throw SGFatalException(msg, crush_code);
}

void sg::exceptions::FatalError(const std::wstring& msg, int crush_code)
{
	sg::exceptions::FatalErrorAssert(true, msg, crush_code);
}

void sg::exceptions::ErrorAssert(bool condition, const std::string& msg)
{
	sg::exceptions::ErrorAssert(condition, std::wstring(msg.begin(), msg.end()));
}

void sg::exceptions::Error(const std::string& msg)
{
	sg::exceptions::ErrorAssert(true, msg);
}

void sg::exceptions::FatalErrorAssert(bool condition, const std::string& msg, int crush_code)
{
	sg::exceptions::FatalErrorAssert(condition, std::wstring(msg.begin(), msg.end()), crush_code);
}

void sg::exceptions::FatalError(const std::string& msg, int crush_code)
{
	sg::exceptions::FatalErrorAssert(true, msg, crush_code);
}

