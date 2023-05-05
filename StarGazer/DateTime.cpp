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
	this->_hours = hours;
	this->_secs = secs;
	this->_mday = mday;
	this->_mins = mins;
	this->_month = month;
	this->_year = year;
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
	time_t tmp_secs = this->_secs + secs, tmp_mins = this->_mins + mins, tmp_hours = this->_hours + hours, tmp_days = days;
	this->_secs = tmp_secs % 60; tmp_mins += tmp_secs / 60;
	this->_mins = tmp_mins % 60; tmp_hours += tmp_mins / 60;
	this->_hours = tmp_hours % 24; tmp_days += tmp_hours / 24;

	while (true)
	{
		time_t available_number_of_days_to_add = this->DaysInCurrentMonth() - this->_mday;

		if (available_number_of_days_to_add >= tmp_days)
		{
			this->_mday += tmp_days;
			break;
		}
		else
		{
			tmp_days -= available_number_of_days_to_add;
			if (this->_month + 1 == 12)
			{
				this->_month = 0;
				++this->_year;
			}
			else ++this->_month;
			this->_mday = 0;
		}


	}
}

uint32_t DateTime::Year() const
{
	return this->_year + 1;
}

uint32_t DateTime::Month() const
{
	return this->_month + 1;
}

uint32_t DateTime::YearDay() const
{
	uint32_t ydays = 0;
	for (uint32_t month = 0; month <= this->_month; ++month)
	{
		if ((month + 1) <= this->_month)
		{
			ydays += DateTime::DaysInMonth(month, this->_year);
		}
		else
		{
			ydays += this->_mday + 1;
		}
	}
	return ydays;
}

uint32_t DateTime::MonthDay() const
{
	return this->_mday + 1;
}

uint32_t DateTime::Hour() const
{
	return this->_hours;
}

uint32_t DateTime::Minute() const
{
	return this->_mins;
}

uint32_t DateTime::Second() const
{
	return this->_secs;
}




bool DateTime::IsLeapYear() const
{
	return DateTime::IsLeapYear(this->_year);
}

bool DateTime::IsLeapYear(const uint32_t year)
{
	return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

uint32_t DateTime::DaysInCurrentMonth() const
{
	return DateTime::DaysInMonth(this->_month, this->_year);
}

bool DateTime::IsCorrectDate(const uint32_t hours, const uint32_t mins, const uint32_t secs,
	const uint32_t year, const uint32_t month, const uint32_t mday)
{
	return (secs < 60 && mins < 60 && hours < 24 && month < 12 && mday < DateTime::DaysInMonth(month, year));
}

bool DateTime::IsCorrectDate(const DateTime& time)
{
	return DateTime::IsCorrectDate(time._hours, time._mins, time._secs, time._year, time._month, time._mday);
}

const std::string DateTime::NumberToZeroString(uint32_t num, uint32_t zero_cnt)
{
	std::string result(zero_cnt, '0'), number_str = std::to_string(num);
	sg::exceptions::ErrorAssert(zero_cnt >= number_str.size(), DEBUG_MSG("Wrong number size"));
	for (size_t i = number_str.size() - 1;; --i)
	{
		result[i + zero_cnt - number_str.size()] = number_str[i];
		if (i == 0)
			break;
	}
	return result;
}

uint32_t DateTime::DaysInMonth(const uint32_t month, const uint32_t year)
{
	static const uint8_t MONTH_DAYS[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	static const uint8_t FEB_SPEC_DAYS = 29;
	uint32_t days = ((month == 1 && IsLeapYear(year))
		? (FEB_SPEC_DAYS) : (MONTH_DAYS[month]));
	return days;
}

const std::string DateTime::ToString(const DateTime& time)
{
	return DateTime::NumberToZeroString(time._hours, 2) + ":"
		+ DateTime::NumberToZeroString(time._mins, 2) + ":"
		+ DateTime::NumberToZeroString(time._secs, 2) + " "
		+ std::to_string(time._year) + "/" + DateTime::NumberToZeroString(time._month + 1, 2) + "/"
		+ DateTime::NumberToZeroString(time._mday + 1, 2);
}

const DateTime DateTime::FromString(const std::string& time)
{
	static std::regex regex_expr("(0[0-9]|1[0-9]|2[0-3]|[0-9]):([0-5][0-9]):([0-5][0-9]) ([0-9]{2,5}|[1-9])/(0[1-9]|1[0-2])/(0[0-9]|1[0-9]|2[0-9]|3[01])");

	sg::exceptions::ErrorAssert(std::regex_match(time, regex_expr), DEBUG_MSG("Wrong datetime format: [hh:mm:ss yyyy/mm/dd]"));
	std::istringstream sinput(time);
	DateTime result_time;
	sinput >> result_time._hours; 
	sinput.ignore(1); 
	sinput >> result_time._mins; 
	sinput.ignore(1); 
	sinput >> result_time._secs >> result_time._year; 
	sinput.ignore(1); 
	sinput >> result_time._month; 
	sinput.ignore(1); 
	sinput >> result_time._mday;
	--result_time._month; --result_time._mday;
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
