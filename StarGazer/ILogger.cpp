#include "ILogger.h"

using namespace sg::utility;

const std::string ILogger::DEFAULT_LOGS_FILE_DIRECTORY = "logs";

std::string ILogger::LOGS_FILE_DIRECTORY = "";
std::string ILogger::LOGS_FILE_NAME;
std::mutex ILogger::GLOBAL_LOGGER_MUTEX;

ILogger::ILogger(const LogType log_type,
	uint32_t logger_flags) : m_flags{ logger_flags }, m_log_type{ log_type }, isDebug_{false}
{
	if (ILogger::LOGS_FILE_NAME.empty())
	{
		std::string logs_directory;
		if(ILogger::LOGS_FILE_DIRECTORY.empty())	logs_directory = ILogger::DEFAULT_LOGS_FILE_DIRECTORY;
		else										logs_directory = ILogger::LOGS_FILE_DIRECTORY;
			
		ILogger::LOGS_FILE_NAME = logs_directory + "\\" +
			DateTime::DateTimeToUniqueString(DateTime::Now()) + ".log";
		if(!logs_directory.empty())
			std::filesystem::create_directory(ILogger::DEFAULT_LOGS_FILE_DIRECTORY);
	}
}

void sg::utility::ILogger::InitLogFileDirectory(const std::filesystem::path& path)
{
	ILogger::LOGS_FILE_DIRECTORY = path.string();
}

const ILogger sg::utility::ILogger::GetDebugLogger(const LogType log_type)
{
	return ILogger(log_type);
}
