#include <iostream>
#include "DateTime.h"

using namespace std;
using namespace sg::utility;
int main()
{
	while(true)
	std::cout << DateTime::ToString(DateTime::Now()) << std::endl;
	return 0;
}