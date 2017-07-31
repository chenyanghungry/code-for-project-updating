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
	QString pathQ;//保存正在处理图片的路径
	QStandardItemModel *model;//tableview的显示模型

	

public slots:
	void start();//读取指定路径的图片
	void shotimage();//截图函数
	void open();//选择文件并检测
	void close();//关闭窗口
	void operate_document();//简要的帮助文档
	void savepicture();//保存图片
	void savetext();//保存处理的 结果
	void pointimage();//点取四边形的四个点

private:
	Ui::MyClass ui;
	
};

#endif // MYCLASS_H
