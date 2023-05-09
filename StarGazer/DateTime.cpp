#include "DateTime.h"
using namespace sg::utility;
DateTime::DateTime(const uint32_t hours, const uint32_t mins, const uint32_t secs,
	const uint32_t year, const uint32_t month, const uint32_t mday)
{
	this->SetDateTime(hours, mins, secs, year, month, mday);
}

void DateTime::SetDateTime(const uint32_t hours, const uint32_t mins, const uint32_t secs,
	const uint32_t year, const uint32_t month, const uint32_t mday)
{
	sg::exceptions::ErrorAssert(DateTime::IsCorrectDate(hours, mins, secs, year, month, mday), DEBUG_MSG("Wrong datetime format"));
	this->m_hours = hours;
	this->m_secs = secs;
	this->m_mday = mday;
	this->m_mins = mins;
	this->m_month = month;
	this->m_year = year;
}

DateTime::DateTime(const std::string& time)
{
	this->SetDateTime(time);
}

void DateTime::SetDateTime(const std::string& time)
{
	*this = DateTime::FromString(time);
}

void DateTime::PassDateTime(const std::string& time)
{
	static std::regex regex_expr("([0-9]{1,9}):\\1:\\1 \\1");
	sg::exceptions::ErrorAssert(std::regex_match(time, regex_expr), DEBUG_MSG("Wrong timer format: [h1-9:m1-9:s1-9 d1-9]"));
	std::istringstream sinput(time);
	uint32_t secs, mins, hours, days;
	sinput >> hours;
	sinput.ignore(1);
	sinput >> mins;
	sinput.ignore(1);
	sinput >> secs >> days;

	this->PassDateTime(secs, mins, hours, days);
}

void DateTime::PassDateTime(const time_t secs, const time_t mins, const time_t hours, const time_t days)
{
	time_t tmp_secs = this->m_secs + secs, tmp_mins = this->m_mins + mins, tmp_hours = this->m_hours + hours, tmp_days = days;
	this->m_secs = tmp_secs % 60; tmp_mins += tmp_secs / 60;
	this->m_mins = tmp_mins % 60; tmp_hours += tmp_mins / 60;
	this->m_hours = tmp_hours % 24; tmp_days += tmp_hours / 24;

	while (true)
	{
		time_t available_number_of_days_to_add = this->DaysInCurrentMonth() - this->m_mday;

		if (available_number_of_days_to_add >= tmp_days)
		{
			this->m_mday += static_cast<uint32_t>(tmp_days);
			break;
		}
		else
		{
			tmp_days -= available_number_of_days_to_add;
			if (this->m_month + 1 == 12)
			{
				this->m_month = 0;
				++this->m_year;
			}
			else ++this->m_month;
			this->m_mday = 0;
		}


	}
}

uint32_t DateTime::Year() const
{
	return this->m_year;
}

uint32_t DateTime::Month() const
{
	return this->m_month + 1;
}

uint32_t DateTime::YearDay() const
{
	uint32_t ydays = 0;
	for (uint32_t month = 0; month <= this->m_month; ++month)
	{
		if ((month + 1) <= this->m_month)
		{
			ydays += DateTime::DaysInMonth(month, this->m_year);
		}
		else
		{
			ydays += this->m_mday + 1;
		}
	}
	return ydays;
}

uint32_t DateTime::MonthDay() const
{
	return this->m_mday + 1;
}

uint32_t DateTime::Hour() const
{
	return this->m_hours;
}

uint32_t DateTime::Minute() const
{
	return this->m_mins;
}

uint32_t DateTime::Second() const
{
	return this->m_secs;
}




bool DateTime::IsLeapYear() const
{
	return DateTime::IsLeapYear(this->m_year);
}

bool DateTime::IsLeapYear(const uint32_t year)
{
	return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

uint32_t DateTime::DaysInCurrentMonth() const
{
	return DateTime::DaysInMonth(this->m_month, this->m_year);
}

bool DateTime::IsCorrectDate(const uint32_t hours, const uint32_t mins, const uint32_t secs,
	const uint32_t year, const uint32_t month, const uint32_t mday)
{
	return (secs < 60 && mins < 60 && hours < 24 && month < 12 && mday < DateTime::DaysInMonth(month, year));
}

bool DateTime::IsCorrectDate(const DateTime& time)
{
	return DateTime::IsCorrectDate(time.m_hours, time.m_mins, time.m_secs, time.m_year, time.m_month, time.m_mday);
}

uint32_t DateTime::DaysInMonth(const uint32_t month, const uint32_t year)
{
	static const uint8_t MONTH_DAYS[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	static const uint8_t FEB_SPEC_DAYS = 29;
	uint32_t days = ((month == 1 && IsLeapYear(year))
		? (FEB_SPEC_DAYS) : (MONTH_DAYS[month]));
	return days;
}

const std::string DateTime::DateTimeToString(const DateTime& time)
{
	return DateTime::TimeToString(time) + " " + DateTime::DateToString(time);
}

const std::string DateTime::DateToString(const DateTime& time)
{
	std::ostringstream result;
	result << std::setfill('0')
		<< std::setw(2) << time.Year() << '/'
		<< std::setw(2) << time.Month() << '/'
		<< std::setfill(' ') << time.MonthDay();
	return result.str();
}
const std::string DateTime::DateTimeToUniqueString(const DateTime& time)
{
	std::ostringstream stream;
	stream << std::setfill('0')
		<< std::setw(2) << time.Hour()
		<< std::setw(2) << time.Minute()
		<< std::setw(2) << time.Second()
		<< std::setw(2) << time.Year()
		<< std::setw(2) << time.Month()
		<< std::setw(2) << time.MonthDay() << std::setfill(' ');
	return stream.str();
}
const std::string DateTime::TimeToString(const DateTime& time)
{
	std::ostringstream result;
	result << std::setfill('0')
		<< std::setw(2) << time.Hour() << ':'
		<< std::setw(2) << time.Minute() << ':'
		<< std::setw(2) << time.Second() << std::setfill(' ');
	return result.str();
}

const DateTime DateTime::FromString(const std::string& time)
{
	static std::regex regex_expr("(0[0-9]|1[0-9]|2[0-3]|[0-9]):([0-5][0-9]):([0-5][0-9]) ([0-9]{2,5}|[1-9])/(0[1-9]|1[0-2])/(0[0-9]|1[0-9]|2[0-9]|3[01])");

	sg::exceptions::ErrorAssert(std::regex_match(time, regex_expr), DEBUG_MSG("Wrong datetime format: [hh:mm:ss yyyy/mm/dd]"));
	std::istringstream sinput(time);
	DateTime result_time;
	sinput >> result_time.m_hours; 
	sinput.ignore(1); 
	sinput >> result_time.m_mins; 
	sinput.ignore(1); 
	sinput >> result_time.m_secs >> result_time.m_year; 
	sinput.ignore(1); 
	sinput >> result_time.m_month; 
	sinput.ignore(1); 
	sinput >> result_time.m_mday;
	--result_time.m_month; --result_time.m_mday;
	DateTime::IsCorrectDate(result_time);

	return result_time;
}

DateTime DateTime::Now()
{
	
	DateTime result(0, 0, 0, 1970, 0, 0);
	tm tmp_tm;

	const auto now = std::chrono::system_clock::now();
	time_t pass = std::chrono::system_clock::to_time_t(now);

	localtime_s(&tmp_tm, &pass);
	pass = std::mktime(&tmp_tm);
	
	result.SetDateTime(tmp_tm.tm_hour, tmp_tm.tm_min, tmp_tm.tm_sec, 1900 + tmp_tm.tm_year, tmp_tm.tm_mon, tmp_tm.tm_mday - 1);
	return result;

}
