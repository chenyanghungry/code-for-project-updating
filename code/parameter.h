#pragma once
#include <opencv2/core/core.hpp>
#include <vector>

using namespace std;
/******************************************************
**类说明：此类用于程序的初始化信息
*******************************************************/
class parameter
{
public:
	parameter();
	~parameter();

	//手眼标定矩阵
	cv::Mat hand_to_eye;
	//相机内参
	vector<double> intrinsicV;
};

