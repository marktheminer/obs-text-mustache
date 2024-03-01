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

const wregex find_conditional_regex(
	L"\\{\\{#if (\\w+)\\}\\}(.*?)\\{\\{/if \\1\\}\\}");

wstring evaluateConditionals(const std::wstring &initial)
{
	wstring text_to_render = initial;
	const auto conditions_begin = wsregex_iterator(text_to_render.begin(),
						       text_to_render.end(),
						       find_conditional_regex);
	const auto conditions_end = wsregex_iterator();
	for (wsregex_iterator i = conditions_begin; i != conditions_end; ++i) {
		const wsmatch match = *i;
		const wstring match_str = match.str(1);
		const QString variable = QString::fromStdWString(match_str);
		VariablesAndValues *variablesAndValues =
			VariablesAndValues::getInstance();
		// blog(LOG_INFO, "found conditional for %s", variable.toStdString().c_str());
		if (variablesAndValues->contains(variable) &&
		    !variablesAndValues->getValue(variable)
			     .toStdWString()
			     .empty()) {
			// blog(LOG_INFO, "replacing conditional for %s", variable.toStdString().c_str());
			wstringstream buf;
			buf << L"\\{\\{#if " << variable.toStdWString()
			    << L"\\}\\}(.*)\\{\\{/if "
			    << variable.toStdWString() << "\\}\\}";
			const wregex re(buf.str());
			const auto variable_condition_begin =
				wsregex_iterator(text_to_render.begin(),
						 text_to_render.end(), re);
			const auto variable_condition_end = wsregex_iterator();
			for (wsregex_iterator j = variable_condition_begin;
			     j != variable_condition_end; ++j) {
				const wsmatch render_match = *j;
				const wstring render_str = render_match.str(1);
				text_to_render = regex_replace(
					text_to_render, re,
					evaluateConditionals(render_str));
			}
		} else {
			wstringstream buf;
			buf << L"\\{\\{#if " << variable.toStdWString()
			    << L"\\}\\}(.*)\\{\\{/if "
			    << variable.toStdWString() << "\\}\\}";
			const wregex re(buf.str());
			text_to_render = regex_replace(text_to_render, re, L"");
		}
	}
	return text_to_render;
}

wstring replaceVariables(const wstring &initial)
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
	blog(LOG_DEBUG, "replaceVariables: final text_to_render %s",
	     QString::fromStdWString(text_to_render).toStdString().c_str());
	return text_to_render;
}

wstring replaceDateTimes(const wstring &initial)
{
	wstring text_to_render = initial;
	time_t currentTime = time(nullptr);
	struct tm *localTime = localtime(&currentTime);

	text_to_render = regex_replace(text_to_render,
				       wregex(L"\\{\\{DateTime month\\}\\}"),
				       getCurrentMonthName(localTime));
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

	blog(LOG_DEBUG, "replaceDateTimes: final text_to_render %s",
	     QString::fromStdWString(text_to_render).toStdString().c_str());
	return text_to_render;
}
