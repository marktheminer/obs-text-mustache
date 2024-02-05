#include <obs-frontend-api.h>
#include <obs-module.h>
#include <set>
#include <string>
#include <regex>
#include <iterator>
#include <obs.hpp>
#include <util/util.hpp>
#include <QAction>
#include <QMainWindow>
#include <QTimer>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "obs-text-mustache-definitions.hpp"
#include "text-source.hpp"
#include "variables.hpp"

using namespace std;

OBSTextMustacheDefinitions *dialog;

const wregex variable_regex(L"\\{\\{(\\w+)\\}\\}");

static bool findVariables(void *data, obs_source_t *source)
{
	VariablesAndValues *variablesAndValues =
		VariablesAndValues::getInstance();

	const char *id = obs_source_get_id(source);

	if (!strcmp("text_gdiplus_mustache_v2", id)) {
		TextSource *mySource = reinterpret_cast<TextSource *>(
			obs_obj_get_data(source));
		blog(LOG_DEBUG,
		     "findVariables: found text_gdiplus_mustache_v2 source with text %s",
		     QString::fromStdWString(mySource->text)
			     .toStdString()
			     .c_str());
		const auto variables_begin =
			wsregex_iterator(mySource->text.begin(),
					 mySource->text.end(), variable_regex);
		const auto variables_end = wsregex_iterator();
		for (wsregex_iterator i = variables_begin; i != variables_end;
		     ++i) {
			const wsmatch match = *i;
			const wstring match_str = match.str(1);
			const QString variable =
				QString::fromStdWString(match_str);
			blog(LOG_DEBUG,
			     "findVariables: found variable %s in the scene",
			     variable.toStdString().c_str());
			if (!variablesAndValues->contains(variable)) {
				blog(LOG_DEBUG,
				     "findVariables: adding variable %s",
				     variable.toStdString().c_str());
				variablesAndValues->putVariable(variable);
			}
		}
	}
	return true;
}

static bool updateText(void *data, obs_source_t *source)
{
	const char *id = obs_source_get_id(source);
	if (!strcmp("text_gdiplus_mustache_v2", id)) {
		TextSource *mySource = reinterpret_cast<TextSource *>(
			obs_obj_get_data(source));
		mySource->UpdateTextToRender();
	}
	return true;
}

static void loadVariablesAndValues(obs_data_t *data, void *param)
{
	VariablesAndValues *const variablesAndValues =
		VariablesAndValues::getInstance();
	const auto variables = variablesAndValues->getVariables();

	variablesAndValues->putValue(obs_data_get_string(data, "variable"),
				     obs_data_get_string(data, "value"));
}

OBSTextMustacheDefinitions::OBSTextMustacheDefinitions(QWidget *parent)
	: QDialog(parent),
	  ui(new Ui_OBSTextMustacheDefinitions)
{
	ui->setupUi(this);

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	QObject::connect(ui->buttonBox->button(QDialogButtonBox::Close),
			 &QPushButton::clicked, this,
			 &OBSTextMustacheDefinitions::hide);
	QObject::connect(ui->buttonBox->button(QDialogButtonBox::Close),
			 &QPushButton::clicked, this,
			 &OBSTextMustacheDefinitions::HideDialog);
	QObject::connect(&timer, SIGNAL(timeout()), SLOT(TimerTextUpdate()));
	timer.start(250);
}

void OBSTextMustacheDefinitions::closeEvent(QCloseEvent *)
{
	obs_frontend_save();
}

void OBSTextMustacheDefinitions::ShowDialog()
{
	VariablesAndValues *const variablesAndValues =
		VariablesAndValues::getInstance();
	ui->gridLayout->setColumnStretch(0, 1);
	ui->gridLayout->setColumnStretch(1, 2);
	obs_enum_sources(findVariables, NULL);
	const auto variables = variablesAndValues->getVariables();
	int currentRow = 0;
	textLines.clear();
	for (auto it = variables.begin(); it != variables.end();
	     ++it, ++currentRow) {

		QLabel *label = new QLabel(*it);
		label->setAlignment(Qt::AlignVCenter);
		ui->gridLayout->addWidget(label, currentRow, 0);
		QLineEdit *lineEdit =
			new QLineEdit(variablesAndValues->getValue(*it));
		textLines[*it] = lineEdit;
		ui->gridLayout->addWidget(lineEdit, currentRow, 1);
	}
	setVisible(true);
	QTimer::singleShot(250, this, &OBSTextMustacheDefinitions::show);
}

void OBSTextMustacheDefinitions::HideDialog()
{
	VariablesAndValues *const variablesAndValues =
		VariablesAndValues::getInstance();

	setVisible(false);
	const auto variables = variablesAndValues->getVariables();
	for (auto it = variables.begin(); it != variables.end(); ++it) {
		const auto variable = *it;
		const auto value = textLines[*it]->text();
		variablesAndValues->putValue(variable, value);
		blog(LOG_DEBUG, "HideDialog: Setting variable %s to %s",
		     variable.toStdString().c_str(),
		     value.toStdString().c_str());
	}
	obs_enum_sources(updateText, NULL);

	QTimer::singleShot(250, this, &OBSTextMustacheDefinitions::hide);
}

void OBSTextMustacheDefinitions::TimerTextUpdate()
{
	obs_enum_sources(updateText, NULL);
	timer.start(250);
}

static void SaveOBSTextMustacheDefinitions(obs_data_t *save_data, bool saving,
					   void *)
{
	VariablesAndValues *const variablesAndValues =
		VariablesAndValues::getInstance();
	if (saving) {
		const OBSDataAutoRelease obj = obs_data_create();
		const OBSDataArrayAutoRelease array = obs_data_array_create();
		const auto variables = variablesAndValues->getVariables();
		for (auto it = variables.begin(); it != variables.end(); ++it) {
			const QString variable = *it;
			const QString value = variablesAndValues->getValue(*it);
			blog(LOG_DEBUG,
			     "SaveOBSTextMustacheDefinitions: Considering saving variable %s",
			     variable.toStdString().c_str());
			if (value.size() > 0) {
				const OBSDataAutoRelease keyValue =
					obs_data_create();
				blog(LOG_DEBUG,
				     "SaveOBSTextMustacheDefinitions: Saving variable %s as %s",
				     variable.toStdString().c_str(),
				     value.toStdString().c_str());
				obs_data_set_string(
					keyValue, "variable",
					variable.toStdString().c_str());
				obs_data_set_string(
					keyValue, "value",
					value.toStdString().c_str());

				obs_data_array_push_back(array, keyValue);

				blog(LOG_DEBUG,
				     "SaveOBSTextMustacheDefinitions: Done saving variable %s as %s",
				     variable.toStdString().c_str(),
				     value.toStdString().c_str());
			}
		}
		obs_data_set_array(obj, "variablesAndValues", array);
		blog(LOG_DEBUG,
		     "SaveOBSTextMustacheDefinitions: About to save data");
		obs_data_set_obj(save_data, "obs-text-mustache", obj);
		blog(LOG_DEBUG,
		     "SaveOBSTextMustacheDefinitions: Done saving data");
	} else {
		OBSDataAutoRelease obj =
			obs_data_get_obj(save_data, "obs-text-mustache");

		if (!obj)
			obj = obs_data_create();

		obs_data_array_enum(obs_data_get_array(obj,
						       "variablesAndValues"),
				    loadVariablesAndValues, NULL);
	}
}

extern "C" void FreeOBSTextMustacheDefinitions() {}

static void OBSEvent(enum obs_frontend_event event, void *)
{
	blog(LOG_DEBUG, "OBSEvent: %d", event);
	switch (event) {
	case OBS_FRONTEND_EVENT_EXIT: {
		obs_frontend_save();
		FreeOBSTextMustacheDefinitions();
		break;
	}
	case OBS_FRONTEND_EVENT_FINISHED_LOADING: {
		obs_enum_sources(updateText, NULL);
		break;
	}
	}
}

extern "C" void InitOBSTextMustacheDefinitions()
{
	QAction *const action = (QAction *)obs_frontend_add_tools_menu_qaction(
		obs_module_text("TextGDIPlusDefinitions"));

	obs_frontend_push_ui_translation(obs_module_get_string);

	QMainWindow *const window =
		(QMainWindow *)obs_frontend_get_main_window();

	dialog = new OBSTextMustacheDefinitions(window);

	const auto cb = []() {
		dialog->ShowDialog();
	};

	obs_frontend_pop_ui_translation();

	obs_frontend_add_save_callback(SaveOBSTextMustacheDefinitions, nullptr);
	obs_frontend_add_event_callback(OBSEvent, nullptr);

	action->connect(action, &QAction::triggered, cb);
}