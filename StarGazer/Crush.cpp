#include "Crush.h"


void sg::exceptions::Crush(int crush_code, const std::string& msg)
{
	std::wstring w_msg(msg.begin(), msg.end());
	MessageBox(NULL, w_msg.c_str(), L"Crush Report", MB_ICONERROR | MB_OK);
	std::string crush_reports_directory = "crush_reports";
	std::filesystem::create_directory(crush_reports_directory);
	std::ofstream crush_reports_file(crush_reports_directory+"\\" + 
		sg::utility::DateTime::DateTimeToUniqueString(sg::utility::DateTime::Now()) + ".txt");
	crush_reports_file << "Crush Report: " << msg << std::endl;
	exit(crush_code);
}