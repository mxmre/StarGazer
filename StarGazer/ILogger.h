#pragma once
#include <string>
#include <filesystem>

#include "DateTime.h"

namespace sg
{
	namespace utility
	{
		class ILogger
		{
		public:
			enum class LogType
			{
				DefaultMessage,
				Warning,
				Info,
				Error
			};

			enum LogFlags : uint32_t
			{
				Null = 0,
				Timed = 1
			};

			explicit ILogger(const LogType log_type = ILogger::LogType::DefaultMessage,
				uint32_t logger_flags = ILogger::LogFlags::Timed);
		protected:
			LogType m_log_type;
			uint32_t m_flags;
			
			static const std::string LOGFILES_PATH;
			static std::string ms_logfile_name;
			static bool ms_static_element_is_init;
		};
	}
}