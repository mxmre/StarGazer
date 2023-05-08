#pragma once
#include <string>
#include <fstream>
#include <filesystem>

#include <Windows.h>

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