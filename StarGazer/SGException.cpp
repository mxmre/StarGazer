#include "SGException.h"

sg::exceptions::SGException::SGException(const std::wstring& msg) :
	m_msg(msg)
{

}

const std::wstring sg::exceptions::SGException::what() const
{
	return this->m_msg;
}
