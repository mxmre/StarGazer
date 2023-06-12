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
			SGFatalException(const std::wstring& msg, int code) : SGException(msg), _code{code} {};
			int code() const { return this->_code; }
		protected:
			int _code;
		};
	}
}