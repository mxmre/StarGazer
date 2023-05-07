#include <iostream>
#include "DateTime.h"
#include "Logger.h"

using namespace std;
using namespace sg::utility;
int main()
{
	Logger<char> l1(ILogger::LogType::Info);
	Logger<wchar_t> l2(ILogger::LogType::Info);
	Logger<char16_t> l3(ILogger::LogType::Info);
	/*l1.Print("ffff");
	l2.Print(L"ffff");*/
	l3.Print(u"υσι");

	return 0;
}