#include <util/util.hpp>
#include "variables.hpp"

using namespace std;

VariablesAndValues *VariablesAndValues::self = nullptr;

VariablesAndValues *VariablesAndValues::getInstance()
{
	if (self == nullptr) {
		self = new VariablesAndValues();
	}
	return self;
}

void VariablesAndValues::putVariable(const QString &variable)
{
	blog(LOG_INFO, "VariablesAndValues::putVariable %s",
	     variable.toStdString().c_str());
	if (variablesAndValues.find(variable) == variablesAndValues.end()) {
		variablesAndValues[variable] = QString("");
		variables.insert(variable);
	}
}

bool VariablesAndValues::contains(const QString &variable)
{
	return variables.find(variable) != variables.end();
}

void VariablesAndValues::putValue(const QString &variable, const QString &value)
{
	blog(LOG_INFO, "VariablesAndValues::putValue %s %s",
	     variable.toStdString().c_str(), value.toStdString().c_str());
	variablesAndValues[variable] = value;
	variables.insert(variable);
}

const set<QString> &VariablesAndValues::getVariables()
{
	return variables;
}

const QString &VariablesAndValues::getValue(const QString &variable)
{
	return variablesAndValues[variable];
}

void VariablesAndValues::clear()
{
	blog(LOG_INFO, "VariablesAndValues::clear");

	variablesAndValues.clear();
	variables.clear();
}