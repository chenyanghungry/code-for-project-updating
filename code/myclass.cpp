#include "myclass.h"
#include<qfiledialog.h>
#include<qmessagebox.h>

#include <fstream>

#include "convert.h"
#include "commonfunc.h"

MyClass::MyClass(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this); 
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui.beginButton, SIGNAL(clicked()), this, SLOT(start()));
	connect(ui.shotButton, SIGNAL(clicked()), this, SLOT(shotimage()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionOperate_document,SIGNAL(triggered()),this,SLOT(operate_document()));
	connect(ui.actionPicture,SIGNAL(triggered()),this,SLOT(savepicture()));
	connect(ui.actiontext,SIGNAL(triggered()),this,SLOT(savetext()));
	connect(ui.pointButton,SIGNAL(clicked()),this,SLOT(pointimage()));
	//机械臂操作
	connect(ui.initializeButton, SIGNAL(clicked()), this, SLOT(arm_initial()));
	connect(ui.operateButton,SIGNAL(clicked()),this,SLOT(arm_operate()));

	//tableview显示结果窗口设置
	model = new QStandardItemModel(ui.tableView);
	model->setHorizontalHeaderItem(0, new QStandardItem(s2q("平移与旋转")));
	model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("X")));
	model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("Y")));
	model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("Z")));
	ui.tableView->setModel(model);
	//设置选中时为整行选中        
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	//设置表格的单元为只读属性，即不能编辑        
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->setColumnWidth(0, 140);
	ui.tableView->setColumnWidth(1, 120);
	ui.tableView->setColumnWidth(2, 120);
	ui.tableView->setColumnWidth(3, 120);


	
}

MyClass::~MyClass()
{

}

void MyClass::start()
{
	string imagepath="..\\start.bmp";
	pathQ=s2q(imagepath);
	string resultshow;
	Point2f pts = Point2f(0,0);
	vector<vector<string>> results;
	procfunc(imagepath,imagepath,pts,results);
	Mat temp = imread("..\\resultshow.bmp");
	Mat temp1;
	cvtColor(temp, temp1, CV_BGR2RGB);
	QImage showImageQ = QImage((const unsigned char*)(temp1.data), temp1.cols, temp1.rows, QImage::Format_RGB888);
	this->ui.imagelabel->setPixmap(QPixmap::fromImage(showImageQ));
	if (model->rowCount()==0)
	{
		for (size_t i = 0; i < results.size(); i++)
		{
			model->setItem(2 * i, 0, new QStandardItem(s2q(results[i][0])));
			model->setItem(2 * i, 1, new QStandardItem(s2q(results[i][1])));
			model->setItem(2 * i, 2, new QStandardItem(s2q(results[i][2])));
			model->setItem(2 * i, 3, new QStandardItem(s2q(results[i][3])));
			model->setItem(2 * i + 1, 1, new QStandardItem(s2q(results[i][4])));
			model->setItem(2 * i + 1, 2, new QStandardItem(s2q(results[i][5])));
			model->setItem(2 * i + 1, 3, new QStandardItem(s2q(results[i][6])));
		}
	}
	else
	{
		model->removeRows(0, model->rowCount());
		for (size_t i = 0; i < results.size(); i++)
		{
			model->setItem(2 * i, 0, new QStandardItem(s2q(results[i][0])));
			model->setItem(2 * i, 1, new QStandardItem(s2q(results[i][1])));
			model->setItem(2 * i, 2, new QStandardItem(s2q(results[i][2])));
			model->setItem(2 * i, 3, new QStandardItem(s2q(results[i][3])));
			model->setItem(2 * i + 1, 1, new QStandardItem(s2q(results[i][4])));
			model->setItem(2 * i + 1, 2, new QStandardItem(s2q(results[i][5])));
			model->setItem(2 * i + 1, 3, new QStandardItem(s2q(results[i][6])));
		}
	}
}

void MyClass::open()
{
	QString imagepathQ = QFileDialog::getOpenFileName(this, tr("select image"), "..", tr("Image files(*.bmp *.png)"));
	pathQ = imagepathQ;
	if (imagepathQ.length()==0)
	{
		//QMessageBox::information(NULL, QStringLiteral("警告"), QStringLiteral("没有选中任何的文件！"));
		QMessageBox::warning(NULL,QStringLiteral("路径"),QStringLiteral("没有选中任何文件！"));
	}
	else
	{
		string imagepath = q2s(imagepathQ);
		string resultshow;
		Point2f pts = Point2f(0,0);
		vector<vector<string>> results;
		procfunc(imagepath,imagepath,pts,results);
	
		Mat temp = imread("..\\resultshow.bmp");
		Mat temp1;
		cvtColor(temp, temp1, CV_BGR2RGB);
		//cvtColor(temp, test3, CV_GRAY2RGB);
		QImage showImageQ = QImage((const unsigned char*)(temp1.data), temp1.cols, temp1.rows, QImage::Format_RGB888);
		this->ui.imagelabel->setPixmap(QPixmap::fromImage(showImageQ));
		if (model->rowCount()==0)
		{
			for (size_t i = 0; i < results.size(); i++)
			{
				model->setItem(2 * i, 0, new QStandardItem(s2q(results[i][0])));
				model->setItem(2 * i, 1, new QStandardItem(s2q(results[i][1])));
				model->setItem(2 * i, 2, new QStandardItem(s2q(results[i][2])));
				model->setItem(2 * i, 3, new QStandardItem(s2q(results[i][3])));
				model->setItem(2 * i + 1, 1, new QStandardItem(s2q(results[i][4])));
				model->setItem(2 * i + 1, 2, new QStandardItem(s2q(results[i][5])));
				model->setItem(2 * i + 1, 3, new QStandardItem(s2q(results[i][6])));
			}
		}
		else
		{
			model->removeRows(0, model->rowCount());
			for (size_t i = 0; i < results.size(); i++)
			{
				model->setItem(2 * i, 0, new QStandardItem(s2q(results[i][0])));
				model->setItem(2 * i, 1, new QStandardItem(s2q(results[i][1])));
				model->setItem(2 * i, 2, new QStandardItem(s2q(results[i][2])));
				model->setItem(2 * i, 3, new QStandardItem(s2q(results[i][3])));
				model->setItem(2 * i + 1, 1, new QStandardItem(s2q(results[i][4])));
				model->setItem(2 * i + 1, 2, new QStandardItem(s2q(results[i][5])));
				model->setItem(2 * i + 1, 3, new QStandardItem(s2q(results[i][6])));
			}
		}
		
	}
}

void MyClass::close()
{
	/*if (!(QMessageBox::information(this,tr("CT Control View"),tr("Do you really want to log out CT Control View?"),tr("Yes"),tr("No"))))
	{
		this->close();
	}*/
	QApplication *p;
	p->exit(0);
}

void MyClass::shotimage()
{
	string path = q2s(pathQ);
	if (path.size()==0)
	{
		QMessageBox::information(NULL, QStringLiteral("警告"), QStringLiteral("没有选中任何文件"));
	}
	else
	{
		IplImage *srcImg = cvLoadImage(path.c_str());//定义为指针的形式方便传入指针回调函数
		namedWindow("img",CV_WINDOW_NORMAL);//定义一个img窗口 
		Point3f pt_temp = Point3f(0, 0, 0);
		Point3f *pt = &pt_temp;
		pair<IplImage*, Point3f*> *temp = new pair<IplImage*, Point3f*>(srcImg, pt);
		Point2f pt1;
		string resultshow;
		vector<vector<string>> results;
		setMouseCallback("img", on_mouse, (void*)temp);
		/*原先这里使用的是while循环，一直重复截图，直到是划了矩形，但是问题却很多，不知道为什么
		只要在setMouseCallback之后加上waitKey（0）后程序便能够正常运行*/
		waitKey(0);
		
		if ((*pt).z != 0)
		{
			pt1.x = (*pt).x;
			pt1.y = (*pt).y;
			procfunc("..\\shot.bmp",path, pt1,results);
			/*QString resultshowQ = s2q(resultshow);
			ui.resulttextEdit->setText(resultshowQ);*/
			Mat temp = imread("..\\resultshow.bmp",CV_LOAD_IMAGE_COLOR);
			Mat temp1;
			cvtColor(temp, temp1, CV_BGR2RGB);
				
			QImage showImageQ = QImage((const unsigned char*)(temp1.data), temp1.cols, temp1.rows, QImage::Format_RGB888);
			this->ui.imagelabel->setPixmap(QPixmap::fromImage(showImageQ));
			if (model->rowCount() == 0)
			{
				for (size_t i = 0; i < results.size(); i++)
				{
					model->setItem(2 * i, 0, new QStandardItem(s2q(results[i][0])));
					model->setItem(2 * i, 1, new QStandardItem(s2q(results[i][1])));
					model->setItem(2 * i, 2, new QStandardItem(s2q(results[i][2])));
					model->setItem(2 * i, 3, new QStandardItem(s2q(results[i][3])));
					model->setItem(2 * i + 1, 1, new QStandardItem(s2q(results[i][4])));
					model->setItem(2 * i + 1, 2, new QStandardItem(s2q(results[i][5])));
					model->setItem(2 * i + 1, 3, new QStandardItem(s2q(results[i][6])));
				}
			}
			else
			{
				model->removeRows(0, model->rowCount());
				for (size_t i = 0; i < results.size(); i++)
				{
					model->setItem(2 * i, 0, new QStandardItem(s2q(results[i][0])));
					model->setItem(2 * i, 1, new QStandardItem(s2q(results[i][1])));
					model->setItem(2 * i, 2, new QStandardItem(s2q(results[i][2])));
					model->setItem(2 * i, 3, new QStandardItem(s2q(results[i][3])));
					model->setItem(2 * i + 1, 1, new QStandardItem(s2q(results[i][4])));
					model->setItem(2 * i + 1, 2, new QStandardItem(s2q(results[i][5])));
					model->setItem(2 * i + 1, 3, new QStandardItem(s2q(results[i][6])));
				}
			}
			/*break;*/
		}
		//一旦窗口直接关闭，因为不知道会返回什么，程序会一直死循环下去，下面的程序并没有什么作用
		//但是不可或缺，缺了程序还是会一直执行，不会出现截图界面，猜测是因为有waitkey()函数所以可以显示
		/*if (waitKey(10) == 27)
		{
			break;
		}*/
		
		delete temp;
		cvReleaseImage(&srcImg);
	}
}

void MyClass::operate_document()
{
	QMessageBox::information(NULL,"help",QStringLiteral("操作说明在此目录下"));
}

void MyClass::savepicture()
{
	QString filename= QFileDialog::getSaveFileName(this,QStringLiteral("保存图片"),"..",tr("*.bmp"));
	if (filename.isEmpty())
	{
		QMessageBox::information(NULL,QStringLiteral("提示："),QStringLiteral("没有路径被选中！！！"));
	}
	else	
	{
		if (pathQ.isEmpty())
		{
			QMessageBox::information(NULL,QStringLiteral("提示："),QStringLiteral("没有展示的图片！！！"));
		}
		else
		{
			QImage image=ui.imagelabel->pixmap()->toImage();
			if (!image.isNull())
			{
				if (!(image.save(filename,"bmp")))
				{
					QMessageBox::information(NULL,QStringLiteral("提示："),QStringLiteral("图片保存失败！！！"));
				}
			}
			else	
			{
				QMessageBox::information(NULL,QStringLiteral("提示："),QStringLiteral("保存图片！！！"));
			}
		}
	}
}

void MyClass::savetext()
{
	QString filename= QFileDialog::getSaveFileName(this,QStringLiteral("保存图片"),"..",tr("*.txt"));
	if (filename.isEmpty())
	{
		QMessageBox::information(NULL,QStringLiteral("提示："),QStringLiteral("没有路径被选中！！！"));
	}
	string path=q2s(filename);
	const char* one=path.c_str();
	ofstream ofn(one);
	QString temptext;
	stringstream temp;
	QModelIndex index;
	for (int i=0;i<model->rowCount();i++)
	{
		for (int j=0;j<model->rowCount();j++)
		{
			index=model->index(i,j);
			temptext=model->data(index).toString();
			ofn<<q2s(temptext)<<"\t";
		}
		ofn<<"\t\n";
	}
	ofn.close();
}

void MyClass::arm_initial()
{

}

void MyClass::arm_operate()
{

}
/*********************************************************************
**不规范说明：此处的on_mouse1加添了全局变量，没有像前面那样规范，主要是为了编程的简便
**
**该处尝试过先点取，再在周围寻找角点，结果使得结果的旋转角度出现较大的偏差
**********************************************************************/
Mat org;
int n=0;
vector<Point2f> capturePoint;
void on_mouse1(int event, int x,int y,int flags,void *ustc)
{
	Point2f pt;
	Mat crop;
	Mat cropgray;
	vector<Point2f> corners;
	//char coordinateName[16];
	if (event==CV_EVENT_LBUTTONDOWN)
	{
		org(Rect(x - 10, y - 10, 20, 20)).copyTo(crop);
		cvtColor(crop, cropgray, CV_RGB2GRAY);
		//GaussianBlur(cropgray, cropgray, Size(3, 3), 0); // 滤波 
		goodFeaturesToTrack(cropgray, corners, 1, 0.01, 1, Mat());
		pt = Point2f(corners[0].x + x - 10, corners[0].y + y - 10);

		pt=Point2f(x,y);
		capturePoint.push_back(pt);
		n++;
		circle(org,pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);
		//sprintf(coordinateName,"(%d,%d)",x,y);
		//putText(org,coordinateName,pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),1,8);
		imshow("org",org);
		if (n>=4)
		{
			imshow("org",org);
			waitKey(2);
			cvDestroyAllWindows();
		}
	}
}
void MyClass::pointimage()
{
	string path = q2s(pathQ);
	int flag;
	vector<string> result;
	if (path.size()==0)
	{
		QMessageBox::information(NULL, QStringLiteral("警告"), QStringLiteral("没有选中任何文件"));
	}
	else
	{
		org=imread(path,1);
		namedWindow("org");
		setMouseCallback("org",on_mouse1,0);
		imshow("org",org);
		waitKey(0);//这条语句很重要，原因不明
		if (n>=4)
		{
			n=0;
			flag=pointshot(path, capturePoint, result);
			if (flag==0)
			{
				QMessageBox::information(NULL, QStringLiteral("警告"), QStringLiteral("点选择的不合适，请重新选取！"));
			}
			else
			{
				Mat temp = imread("..\\resultshow.bmp", CV_LOAD_IMAGE_COLOR);
				Mat temp1;
				cvtColor(temp, temp1, CV_BGR2RGB);

				QImage showImageQ = QImage((const unsigned char*)(temp1.data), temp1.cols, temp1.rows, QImage::Format_RGB888);
				this->ui.imagelabel->setPixmap(QPixmap::fromImage(showImageQ));
				if (model->rowCount() == 0)
				{
					model->setItem(0, 0, new QStandardItem(s2q(result[0])));
					model->setItem(0, 1, new QStandardItem(s2q(result[1])));
					model->setItem(0, 2, new QStandardItem(s2q(result[2])));
					model->setItem(0, 3, new QStandardItem(s2q(result[3])));
					model->setItem(0 + 1, 1, new QStandardItem(s2q(result[4])));
					model->setItem(0 + 1, 2, new QStandardItem(s2q(result[5])));
					model->setItem(0 + 1, 3, new QStandardItem(s2q(result[6])));
				
				}
				else
				{
					model->removeRows(0, model->rowCount());
					model->setItem(0, 0, new QStandardItem(s2q(result[0])));
					model->setItem(0, 1, new QStandardItem(s2q(result[1])));
					model->setItem(0, 2, new QStandardItem(s2q(result[2])));
					model->setItem(0, 3, new QStandardItem(s2q(result[3])));
					model->setItem(0 + 1, 1, new QStandardItem(s2q(result[4])));
					model->setItem(0 + 1, 2, new QStandardItem(s2q(result[5])));
					model->setItem(0 + 1, 3, new QStandardItem(s2q(result[6])));
				}
			}
		}
		capturePoint.clear();

	}
}