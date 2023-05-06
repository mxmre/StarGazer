#include "ILogger.h"

using namespace sg::utility;

const std::string ILogger::LOGFILES_PATH = "logs";
std::string ILogger::ms_logfile_name;

ILogger::ILogger(const LogType log_type,
	uint32_t logger_flags) : m_flags{ logger_flags }, m_log_type{ log_type }
{
	if (ILogger::ms_logfile_name.empty())
	{
		ILogger::ms_logfile_name = ILogger::LOGFILES_PATH + "\\" +
			DateTime::DateTimeToUniqueString(DateTime::Now()) + ".log";
		std::filesystem::create_directory(ILogger::LOGFILES_PATH);
	}
}