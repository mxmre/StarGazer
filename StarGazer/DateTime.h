#pragma once
#include "stdafx.h"

#include "Asserts.h"
namespace sg
{
	namespace utility
	{
		class DateTime
		{
		public:

			explicit DateTime(const std::string& time);
			void SetDateTime(const std::string& time);
			void PassDateTime(const std::string& time);

			uint32_t Year() const;
			uint32_t Month() const;
			uint32_t YearDay() const;
			uint32_t MonthDay() const;
			//uint32_t WeekDay() const;
			uint32_t Hour() const;
			uint32_t Minute() const;
			uint32_t Second() const;


			bool IsLeapYear() const;
			static bool IsLeapYear(const uint32_t year);
			uint32_t DaysInCurrentMonth() const;
			

			static const std::string DateTimeToString(const DateTime& time);
			static const std::string DateToString(const DateTime& time);
			static const std::string TimeToString(const DateTime& time);
			static const std::string DateTimeToUniqueString(const DateTime& time);
			static const DateTime FromString(const std::string& time);

			static DateTime Now();
		private:
			uint32_t m_secs, m_mins, m_hours, m_mday, m_month, m_year;

			void PassDateTime(const time_t secs, const time_t mins, const time_t hours, const time_t days);
			explicit DateTime(const uint32_t hours = 0, const uint32_t mins = 0, const uint32_t secs = 0,
				const uint32_t year = 0, const uint32_t month = 0, const uint32_t mday = 0);
			void SetDateTime(const uint32_t hours = 0, const uint32_t mins = 0, const uint32_t secs = 0,
				const uint32_t year = 0, const uint32_t month = 0, const uint32_t mday = 0);
			static bool IsCorrectDate(const uint32_t hours, const uint32_t mins, const uint32_t secs,
				const uint32_t year, const uint32_t month, const uint32_t mday);
			static bool IsCorrectDate(const DateTime& time);
			static uint32_t DaysInMonth(const uint32_t month, const uint32_t year);
		};
	}
}


