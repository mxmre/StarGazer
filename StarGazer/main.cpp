#include <iostream>
#include "DateTime.h"
#include "Logger.h"

using namespace std;
using namespace sg::utility;
int main()
{
	Logger<char> l(Logger<char>::LogType::Info);
	
	l.Print("ffff");
	l.Print("ffff");
	return 0;
}