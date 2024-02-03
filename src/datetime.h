//
// Created by Mark Jones New on 2/3/2024.
//
#include <string>

#ifndef DATETIME_H
#define DATETIME_H

std::wstring getCurrentMonthName(const struct tm *  localTime);

std::wstring getCurrentYear(const struct tm *  localTime);

std::wstring getCurrentDay(const struct tm *  localTime);

std::wstring getCurrentDayOfWeek(const struct tm *  localTime);

std::wstring getCurrent24Hour(const struct tm *  localTime);

std::wstring getCurrent12Hour(const struct tm *  localTime);

std::wstring getCurrentMinute(const struct tm *  localTime);

std::wstring getCurrentSecond(const struct tm *  localTime);

std::wstring getCurrentAmPm(const struct tm *  localTime);

#endif //DATETIME_H
