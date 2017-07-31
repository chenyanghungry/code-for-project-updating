#ifndef MYCLASS_H
#define MYCLASS_H

#include <QtWidgets/QMainWindow>
#include "ui_myclass.h"
#include<qstandarditemmodel.h>

#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace cv;

class MyClass : public QMainWindow
{
	Q_OBJECT

public:
	MyClass(QWidget *parent = 0);
	~MyClass();
	QString pathQ;//�������ڴ���ͼƬ��·��
	QStandardItemModel *model;//tableview����ʾģ��

	

public slots:
	void start();//��ȡָ��·����ͼƬ
	void shotimage();//��ͼ����
	void open();//ѡ���ļ������
	void close();//�رմ���
	void operate_document();//��Ҫ�İ����ĵ�
	void savepicture();//����ͼƬ
	void savetext();//���洦��� ���
	void pointimage();//��ȡ�ı��ε��ĸ���

private:
	Ui::MyClass ui;
	
};

#endif // MYCLASS_H
