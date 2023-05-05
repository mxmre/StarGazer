#include "SGException.h"

sg::exceptions::SGException::SGException(const std::string& msg) :
	m_msg(msg)
{

}

const std::string sg::exceptions::SGException::what() const
{
	return this->m_msg;
}
