#pragma once


#include "DateTime.h"
#include "Asserts.h"
#include "ILogger.h"
namespace sg
{
	namespace utility
	{
		template<class _CharType> using base_lgofstream = std::basic_ofstream<_CharType, std::char_traits<_CharType>>;
		template<class _CharType> using base_lgstring = std::basic_string<_CharType, std::char_traits<_CharType>, std::allocator<_CharType>>;

		template<class _CharType>
		class Logger : public ILogger
		{
		public:
			using lgofstream = base_lgofstream<_CharType>;
			using lgstring = base_lgstring<_CharType>;
			

			explicit Logger(const LogType log_type = Logger::LogType::DefaultMessage,
				uint32_t logger_flags = 0x7) : ILogger(log_type, logger_flags)
			{
				/*lgofstream loggers_file(ILogger::ms_logfile_name, std::ios::app);
				exceptions::FatalErrorAssert(loggers_file.is_open(), DEBUG_MSG("Log file cannot create!"));
				loggers_file.close();*/
			}
			
			virtual void Print(lgstring msg)
			{
				//types
				std::string _warn("Warning: "), _info("Info: "), _error("Error: ");
				switch (this->m_log_type)
				{
				case LogType::Warning:
					msg = lgstring(_warn.begin(), _warn.end()) + msg;
					break;
				case LogType::Info:
					msg = lgstring(_info.begin(), _info.end()) + msg;
					break;
				case LogType::Error:
					msg = lgstring(_error.begin(), _error.end()) + msg;
					break;
				case LogType::DefaultMessage:
				default:
					break;
				}
				//flags
				if (this->m_flags & Logger::LogFlags::Timed)
				{
					std::string _time = "[" + DateTime::DateTimeToString(DateTime::Now()) + "] ";
					msg = lgstring(_time.begin(), _time.end()) + msg;
				}
				this->_RawPrint(msg);
			}
			static Logger Info, Warn, Error;
		private:
			
			void _StdOutputPrint(const lgstring& msg)
			{
				std::wstring w_msg(msg.begin(), msg.end());
				std::wcout << w_msg << std::endl;
			}

			void _RawPrint(const lgstring& msg)
			{
				ILogger::GLOBAL_LOGGER_MUTEX.lock();
				if (this->m_flags & ILogger::LogFlags::StdOutput)
				{
					this->_StdOutputPrint(msg);
				}
				if (this->m_flags & ILogger::LogFlags::FileOutput)
				{
					lgofstream ms_loggers_file(ILogger::LOGS_FILE_NAME, std::ios::app);
					if (ms_loggers_file.is_open())
					{
						ms_loggers_file << msg << std::endl;
						ms_loggers_file.close();
					}
				}
				ILogger::GLOBAL_LOGGER_MUTEX.unlock();
			}
			
			
		};
		template<class _CharType> Logger<_CharType> Logger<_CharType>::Info(Logger<_CharType>::LogType::Info);
		template<class _CharType> Logger<_CharType> Logger<_CharType>::Warn(Logger<_CharType>::LogType::Warning);
		template<class _CharType> Logger<_CharType> Logger<_CharType>::Error(Logger<_CharType>::LogType::Error);
	}
}


