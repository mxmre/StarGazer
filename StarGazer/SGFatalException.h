#pragma once
#include "SGException.h"
namespace sg
{
	namespace exceptions
	{
		class SGFatalException : public SGException
		{
		public:
			SGFatalException(const std::string& msg) : SGException(msg){};
		};
	}
}