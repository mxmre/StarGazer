#pragma once
#include "stdafx.h"
#include "SGException.h"
namespace sg
{
	namespace exceptions
	{
		class SGFatalException : public SGException
		{
		public:
			SGFatalException(const std::wstring& msg) : SGException(msg){};
		};
	}
}