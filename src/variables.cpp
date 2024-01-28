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

	if (variablesAndValues.find(variable) == variablesAndValues.end()) {
		variablesAndValues[variable] = QString("");
		variables.insert(variable);
	}
}

bool VariablesAndValues::contains(const QString &variable)
{
	return variables.find(variable) != variables.end();
}

void VariablesAndValues::putValue(const QString& variable, const QString& value) {
	variablesAndValues[variable] = value;
	variables.insert(variable);
}

const set<QString>& VariablesAndValues::getVariables() {
	return variables;
}

const QString& VariablesAndValues::getValue(const QString& variable) {
	 return variablesAndValues[variable];
}

void VariablesAndValues::clear() {
	 variablesAndValues.clear();
	 variables.clear();
}