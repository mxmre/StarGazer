#include "Crush.h"


void sg::exceptions::Crush(int crush_code, const std::wstring& msg)
{
	std::wstring w_msg(msg.begin(), msg.end());
	MessageBox(NULL, w_msg.c_str(), L"Crush Report", MB_ICONERROR | MB_OK);
	std::wstring crush_reports_directory = L"crush_reports";
	std::filesystem::create_directory(crush_reports_directory);
	std::string unique_datetime_str = sg::utility::DateTime::DateTimeToUniqueString(sg::utility::DateTime::Now());
	std::wofstream crush_reports_file(crush_reports_directory+L"\\" + 
		std::wstring(unique_datetime_str.begin(), unique_datetime_str.end()) + L".txt");
	if (crush_reports_file.is_open())
	{
		crush_reports_file << L"Crush Report: " << msg << std::endl;
		crush_reports_file.close();
	}
	
	exit(crush_code);
}