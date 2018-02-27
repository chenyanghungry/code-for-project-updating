#include <QString>  
#include <vector>  
#include <iostream>  
#include <string>  
#include <fstream>
#include <opencv2/core/core.hpp>
#include <math.h>
using namespace std;

//stringת��ΪQString
QString s2q(const string &s);

//QStringת��Ϊstring
string q2s(const QString &s);

//doubleת��Ϊstring
string d2s(double d);

//��ȡָ��·����TXT�ļ�������
vector<double> readData(string path);

//��תƽ������ת��Ϊ����
void vec2matrix(vector<double> vec, cv::Mat &M);