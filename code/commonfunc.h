#pragma once

#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\types_c.h>
#include<opencv2\calib3d\calib3d.hpp>

#include "lsd.h"
#include<math.h>
#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<io.h>
#include<map>

using namespace std;
using namespace cv;

/********************************************************
**模型枚举，0表示不是模型，1是正方形，2是梯形，3是长方形，形状皆为模型的上面
*********************************************************/
enum Model
{
	NO = 0,//表示没有找到模型
	SQUARE = 1,//正方形
	TRAPEZOID=2,//梯形
	RECTANGLE=3//长方形
};
typedef Model model;

/*----------------下方函数和结构全部用于图片寻找模型的四边形---------------*/


/********************************************************
**线结构，保存线的两个点信息，以及厚度，k,b,dist是待求参数
*********************************************************/
struct Line
{
	double x1, y1, x2, y2;//线的两个点
	double width;//线的宽度
	double k, b;//线的斜率和截距
	double dist;//线的长度
};
typedef Line lineinfo;

/*******************************************************************
**函数功能：保存LSD提取的平行线
**
**说明：在最新程序中此部分没有利用
********************************************************************/
ntuple_list parallel(ntuple_list lines);

/*********************************************************************
**函数功能：识别四边形
**
**程序说明：通过之前求出的平行线，主要通过找存在两条线段与平行线分别相交
**********************************************************************/
model quad_recog(ntuple_list n_tuple, vector<lineinfo> lines, vector<Point2f>& pixel,model model_num);

/*********************************************************************
**函数功能：合并同一条直线上的线段
**********************************************************************/
void mergeline(ntuple_list ntl, ntuple_list n_tuple_merge);

/***************************************************************************
**函数说明：将LSD检测的线按从小到大排序，而且每条线的两个点也按从大到小排序
****************************************************************************/
void sortline(ntuple_list ntl);

/***************************************************************************
**函数说明：寻找模型的四边形
****************************************************************************/
void find_quad(ntuple_list n_tuple_merge, cv::Mat temp,vector<pair<vector<Point2f>,model> >& pixels);

/*************************************************************************
**函数说明：QT界面输入路径后的处理函数，与之前的C++程序接入的主要程序
**************************************************************************/
void procfunc(string path,string showpath, Point2f pts,vector<vector<string>>& results);

/****************************************************************************************
**函数说明：鼠标回调函数
*****************************************************************************************/
void on_mouse(int event, int x, int y, int flags, void *ustc);

/****************************************************************************************
**函数说明：四点确定四边形位置
**误差说明：四点确认是存在找不到物体四边形的情形下提出来的，但是点取本身存在误差，所以此部分
			误差可能很大，尝试过划区域找角点，但结果也不是很准。此处代码是另外添加，不够规范
*****************************************************************************************/
int pointshot(string path, vector<Point2f> points, vector<string> &result);

/*********************************************************************************************
**函数说明：根据求出的三个矩阵给出位姿的向量形式并保存结果
**********************************************************************************************/
vector<double> threeM2pose(cv::Mat end2base,cv::Mat handeye, cv::Mat b2c,int i);
