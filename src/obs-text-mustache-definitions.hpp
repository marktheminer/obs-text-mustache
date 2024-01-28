#pragma once

#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <memory>
#include <string>
#include <map>

#include "ui_obs-text-mustache-definitions.h"

class QCloseEvent;

class OBSTextMustacheDefinitions : public QDialog {
	Q_OBJECT

public:
	std::unique_ptr<Ui_OBSTextMustacheDefinitions> ui;
	OBSTextMustacheDefinitions(QWidget *parent);

	void closeEvent(QCloseEvent *event) override;

public slots:
	void ShowDialog();
	void HideDialog();


private:
	std::map<QString, QLineEdit *> textLines;

};