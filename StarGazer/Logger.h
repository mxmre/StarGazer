#pragma once
#include <fstream>
#include <cstdint>
#include <string>
#include <iostream>
#include <typeinfo>

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
				if (!(this->m_flags & ILogger::LogFlags::StdOutput
					&& this->m_flags & ILogger::LogFlags::FileOutput))
				{
					Logger<char> logger(Logger::LogType::Warning);
					logger.Print("A try print message in unknown stream");
				}
				else
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
			}
		private:
			
			void _StdOutputPrint(const lgstring& msg);

			void _RawPrint(const lgstring& msg)
			{
				if (this->m_flags & ILogger::LogFlags::StdOutput)
				{
					this->_StdOutputPrint(msg);
				}
				if (this->m_flags & ILogger::LogFlags::FileOutput)
				{
					lgofstream ms_loggers_file(ILogger::ms_logfile_name, std::ios::app);
					exceptions::FatalErrorAssert(ms_loggers_file.is_open(), DEBUG_MSG("Log file cannot open!"));
					ms_loggers_file << msg << std::endl;
					ms_loggers_file.close();
				}
			}
			
			
		};
		void Logger<wchar_t>::_StdOutputPrint(const base_lgstring<wchar_t>& msg)
		{
			
		}
		void Logger<char>::_StdOutputPrint(const base_lgstring<char>& msg)
		{
			std::cout << msg << std::endl;
		}
		void Logger<char16_t>::_StdOutputPrint(const base_lgstring<char16_t>& msg)
		{
			
			//ADD char32_t support;
			// 
			//not work
			std::wstring w_msg(msg.begin(), msg.end());
			std::wcout << w_msg << std::endl;
		}
		template<class _CharType>
		void Logger<_CharType>::_StdOutputPrint(const base_lgstring<_CharType>& msg)
		{
			static_assert("Logger support only char, wchar_t, char16_t string types");
		}
	}
}


