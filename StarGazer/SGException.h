#pragma once
#include <string>
namespace sg
{
	namespace exceptions
	{
		class SGException
		{
		public:
			SGException(const std::string& msg);
			const std::string what() const;
		private:
			std::string m_msg;
		};
	}
}


