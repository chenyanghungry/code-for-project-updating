#include <QString>  
#include <vector>  
#include <iostream>  
#include <string>  
#include <fstream>
#include <opencv2/core/core.hpp>
#include <math.h>
using namespace std;

//string转化为QString
QString s2q(const string &s);

//QString转化为string
string q2s(const QString &s);

//double转化为string
string d2s(double d);

//读取指定路径的TXT文件中数据
vector<double> readData(string path);

//旋转平移向量转化为矩阵
void vec2matrix(vector<double> vec, cv::Mat &M);