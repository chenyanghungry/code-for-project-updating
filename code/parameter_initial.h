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
	void initial();//����ʼ����Ҫ��ʼ������ڲξ�������۱궨����

private slots:
	void completeinitial();//���޸ĳ�ʼ������

private:
	Ui::parameter_initial ui;
};

#endif // PARAMETER_INITIAL_H
