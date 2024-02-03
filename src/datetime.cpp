//
// Created by Mark Jones New on 2/3/2024.
//

#include <QString>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include "datetime.h"
using namespace std;

wstring getCurrentMonthName(const struct tm *const localTime)
{
	std::ostringstream oss;
	oss << std::put_time(localTime, "%B");

	return QString::fromStdString(oss.str()).toStdWString();
}

wstring getCurrentYear(const struct tm *const localTime)
{
	std::ostringstream oss;
	oss << std::put_time(localTime, "%EY");

	return QString::fromStdString(oss.str()).toStdWString();
}

wstring getCurrentDay(const struct tm *const localTime)
{
	std::ostringstream oss;
	oss << std::put_time(localTime, "%e");

	return QString::fromStdString(oss.str()).toStdWString();
}

wstring getCurrentDayOfWeek(const struct tm *const localTime)
{
	std::ostringstream oss;
	oss << std::put_time(localTime, "%A");

	return QString::fromStdString(oss.str()).toStdWString();
}

wstring getCurrent24Hour(const struct tm *const localTime)
{
	std::ostringstream oss;
	oss << std::put_time(localTime, "%OH");

	return QString::fromStdString(oss.str()).toStdWString();
}

wstring getCurrent12Hour(const struct tm *const localTime)
{
	std::ostringstream oss;
	oss << std::put_time(localTime, "%OI");

	return QString::fromStdString(oss.str()).toStdWString();
}

wstring getCurrentMinute(const struct tm *const localTime)
{
	std::ostringstream oss;
	oss << std::put_time(localTime, "%M");

	return QString::fromStdString(oss.str()).toStdWString();
}

wstring getCurrentSecond(const struct tm *const localTime)
{
	std::ostringstream oss;
	oss << std::put_time(localTime, "%S");

	return QString::fromStdString(oss.str()).toStdWString();
}

wstring getCurrentAmPm(const struct tm *const localTime)
{
	std::ostringstream oss;
	oss << std::put_time(localTime, "%p");

	return QString::fromStdString(oss.str()).toStdWString();
}
