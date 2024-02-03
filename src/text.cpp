//
// Created by Mark Jones New on 2/3/2024.
//
#include <sstream>
#include <regex>
#include <util/util.hpp>
#include "text.h"
#include "datetime.h"
#include "variables.hpp"

using namespace std;

wstring replaceVariables(const wstring& initial)
{
	wstring text_to_render = initial;
	VariablesAndValues *variablesAndValues =
	VariablesAndValues::getInstance();
	const auto variables = variablesAndValues->getVariables();
	for (auto it = variables.begin(); it != variables.end(); ++it) {
		const wstring value =
			variablesAndValues->getValue(*it).toStdWString();
		if (value.size() > 0) {
			const wstring variable = (*it).toStdWString();
			wstringstream buf;
			buf << L"\\{\\{" << variable << L"\\}\\}";
			const wregex re(buf.str());
			text_to_render =
				regex_replace(text_to_render, re, value);
			blog(LOG_DEBUG, "UpdateTextToRender: text_to_render %s",
			     QString::fromStdWString(text_to_render)
				     .toStdString()
				     .c_str());
		}
	}
	blog(LOG_DEBUG, "UpdateTextToRender: final text_to_render %s",
	     QString::fromStdWString(text_to_render).toStdString().c_str());
	text_to_render = text_to_render.append(L"\n");
	return text_to_render;
}

wstring replaceDateTimes(const wstring& initial)
{
	wstring text_to_render = initial;
	time_t currentTime = time(nullptr);
	struct tm* localTime = localtime(&currentTime);

	text_to_render =
		regex_replace(text_to_render, wregex(L"\\{\\{DateTime month\\}\\}"), getCurrentMonthName(localTime));
	text_to_render = regex_replace(text_to_render,
				       wregex(L"\\{\\{DateTime month\\}\\}"),
				       getCurrentMonthName(localTime));
	text_to_render = regex_replace(text_to_render,
				       wregex(L"\\{\\{DateTime day\\}\\}"),
				       getCurrentDay(localTime));
	text_to_render = regex_replace(text_to_render,
				       wregex(L"\\{\\{DateTime weekday\\}\\}"),
				       getCurrentDayOfWeek(localTime));
	text_to_render = regex_replace(text_to_render,
				       wregex(L"\\{\\{DateTime year\\}\\}"),
				       getCurrentYear(localTime));
	text_to_render = regex_replace(text_to_render,
				       wregex(L"\\{\\{DateTime hour24\\}\\}"),
				       getCurrent24Hour(localTime));
	text_to_render = regex_replace(text_to_render,
			       wregex(L"\\{\\{DateTime hour12\\}\\}"),
			       getCurrent12Hour(localTime));
	text_to_render = regex_replace(text_to_render,
				       wregex(L"\\{\\{DateTime minute\\}\\}"),
				       getCurrentMinute(localTime));
	text_to_render = regex_replace(text_to_render,
				       wregex(L"\\{\\{DateTime second\\}\\}"),
				       getCurrentSecond(localTime));
	text_to_render = regex_replace(text_to_render,
				       wregex(L"\\{\\{DateTime ampm\\}\\}"),
				       getCurrentAmPm(localTime));

	blog(LOG_DEBUG, "UpdateTextToRender: final text_to_render %s",
	     QString::fromStdWString(text_to_render).toStdString().c_str());
	text_to_render = text_to_render.append(L"\n");
	return text_to_render;
}
