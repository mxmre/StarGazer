#pragma once
#include "stdafx.h"

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

			struct LogFlags
			{
				enum : uint32_t
				{
					Null = 0,
					FileOutput = 1,
					StdOutput = 2,
					Timed = 4
				};
			};
			

			explicit ILogger(const LogType log_type = ILogger::LogType::DefaultMessage,
				uint32_t logger_flags = 0x7);
			static void InitLogFileDirectory(const std::filesystem::path& path);
		protected:
			LogType m_log_type;
			uint32_t m_flags;
			
			static const std::string DEFAULT_LOGS_FILE_DIRECTORY;
			static std::string LOGS_FILE_DIRECTORY;
			static std::string LOGS_FILE_NAME;
			static bool ms_static_element_is_init;
			static std::mutex GLOBAL_LOGGER_MUTEX;
		};
	}
}