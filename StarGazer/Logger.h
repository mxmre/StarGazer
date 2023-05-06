#pragma once
#include <fstream>
#include <cstdint>
#include <string>
#include <iostream>


#include "DateTime.h"
#include "Asserts.h"
#include "ILogger.h"
namespace sg
{
	namespace utility
	{
		template<class _CharType>
		class Logger : public ILogger
		{
		public:
			using logger_file_stream = std::basic_ofstream<_CharType, std::char_traits<_CharType>>;
			using logger_string = std::basic_string<_CharType, std::char_traits<_CharType>, std::allocator<_CharType>>;

			

			explicit Logger(const LogType log_type = Logger::LogType::DefaultMessage,
				uint32_t logger_flags = LogFlags::Timed) : ILogger(log_type, logger_flags)
			{
				
			}
			virtual void Print(logger_string msg)
			{
				//types
				std::string _warn("Warning: "), _info("Info: "), _error("Error: ");
				switch (this->m_log_type)
				{
				case LogType::Warning:
					msg = logger_string(_warn.begin(), _warn.end()) + msg;
					break;
				case LogType::Info:
					msg = logger_string(_info.begin(), _info.end()) + msg;
					break;
				case LogType::Error:
					msg = logger_string(_error.begin(), _error.end()) + msg;
					break;
				case LogType::DefaultMessage:
				default:
					break;
				}
				//flags
				if (this->m_flags & Logger::LogFlags::Timed)
					msg = "[" + DateTime::DateTimeToString(DateTime::Now()) + "] " + msg;

				this->_RawPrint(msg);
			}
		private:
			

			void _RawPrint(const logger_string& msg)
			{
				std::cout << msg << std::endl;

				logger_file_stream ms_loggers_file(ILogger::ms_logfile_name, std::ios::app);
				exceptions::FatalErrorAssert(ms_loggers_file.is_open(), DEBUG_MSG("Log file cannot open!"));
				ms_loggers_file << msg << std::endl;
				ms_loggers_file.close();
			}
			
			
		};
	}
}


