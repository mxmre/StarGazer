#pragma once
#include "stdafx.h"
namespace sg
{
	namespace exceptions
	{
		enum e_crush_code
		{
			Null = 0,
			WindowClassAlreadyExist
		};
		void Crush(int crush_code, const std::wstring& msg);
		void Crush(int crush_code, const std::string& msg);
	}
}