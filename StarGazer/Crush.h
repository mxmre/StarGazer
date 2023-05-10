#pragma once
#include "stdafx.h"

#include "DateTime.h"
namespace sg
{
	namespace exceptions
	{
		enum CrushCode
		{
			StdError = 1,
			LoggerError
		};
		void Crush(int crush_code, const std::wstring& msg);
	}
}