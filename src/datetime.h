//
// Created by Mark Jones New on 2/3/2024.
//
#include <string>

#ifndef DATETIME_H
#define DATETIME_H

std::wstring getCurrentMonthName(const tm *localTime);

std::wstring getCurrentYear(const tm *localTime);

std::wstring getCurrentDay(const tm *localTime);

std::wstring getCurrentDayOfWeek(const tm *localTime);

std::wstring getCurrent24Hour(const tm *localTime);

std::wstring getCurrent12Hour(const tm *localTime);

std::wstring getCurrentMinute(const tm *localTime);

std::wstring getCurrentSecond(const tm *localTime);

std::wstring getCurrentAmPm(const tm *localTime);

#endif //DATETIME_H
