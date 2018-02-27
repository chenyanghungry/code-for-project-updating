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
	void initial();//程序开始，需要初始化相机内参矩阵和手眼标定矩阵

private slots:
	void completeinitial();//可修改初始化参数

private:
	Ui::parameter_initial ui;
};

#endif // PARAMETER_INITIAL_H
