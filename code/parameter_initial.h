#ifndef PARAMETER_INITIAL_H
#define PARAMETER_INITIAL_H

#include <QWidget>
#include "ui_parameter_initial.h"

class parameter_initial : public QWidget
{
	Q_OBJECT

public:
	parameter_initial(QWidget *parent = 0);
	~parameter_initial();

private:
	void initial();

private slots:
	void completeinitial();

private:
	Ui::parameter_initial ui;
};

#endif // PARAMETER_INITIAL_H
