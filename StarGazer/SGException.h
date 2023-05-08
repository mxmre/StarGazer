#pragma once
#include <string>
namespace sg
{
	namespace exceptions
	{
		class SGException
		{
		public:
			SGException(const std::wstring& msg);
			const std::wstring what() const;
		private:
			std::wstring m_msg;
		};
	}
}


