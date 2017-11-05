#include "convert.h"
#include<sstream>	

QString s2q(const string &s)
{
	return QString(QString::fromLocal8Bit(s.c_str()));
}
string q2s(const QString &s)
{
	return string((const char *)s.toLocal8Bit());
}

string d2s(double value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

vector<double> readData(string path)
{
	ifstream fin(path);
	vector<double> vec;
	double idata;
	while (fin)
	{
		fin >> idata;
		vec.push_back(idata);
	}
	return vec;
}

void vec2matrix(vector<double> vec, cv::Mat &M)
{
	double angle_z = vec[5] * CV_PI / 180;
	double angle_y = vec[4] * CV_PI / 180;
	double angle_x = vec[3] * CV_PI / 180;
	double dist_x = vec[0];
	double dist_y = vec[1];
	double dist_z = vec[2];
	double rotz[4][4] = { { cos(angle_z), -sin(angle_z), 0, 0 }, { sin(angle_z), cos(angle_z), 0, 0 }, { 0, 0, 1, 0 }, {0,0,0,1} };
	cv::Mat rotZ = cv::Mat(4, 4, CV_64FC1, rotz);
	double roty[4][4] = { { cos(angle_y), 0, sin(angle_y), 0 }, { 0, 1, 0, 0 }, { -sin(angle_y), 0, cos(angle_y), 0 }, { 0, 0, 0, 1 } };
	cv::Mat rotY = cv::Mat(4, 4, CV_64FC1, roty);
	double rotx[4][4] = { { 1, 0, 0, 0 }, { 0, cos(angle_x), -sin(angle_x), 0 }, { 0, sin(angle_x), cos(angle_x), 0 }, { 0, 0, 0, 1 } };
	cv::Mat rotX = cv::Mat(4,4,CV_64FC1,rotx);
	M = rotZ*rotY*rotX;
	M.at<double>(0, 3) = dist_x;
	M.at<double>(1, 3) = dist_y;
	M.at<double>(2, 3) = dist_z;
};

