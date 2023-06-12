#include "Crush.h"


void sg::exceptions::Crush(int crush_code, const std::wstring& msg)
{
	std::wstring w_msg(msg.begin(), msg.end());
	MessageBox(NULL, w_msg.c_str(), L"Crush Report", MB_ICONERROR | MB_OK);
	exit(crush_code);
}

void sg::exceptions::Crush(int crush_code, const std::string& msg)
{
	Crush(crush_code, std::wstring(msg.begin(), msg.end()));
}