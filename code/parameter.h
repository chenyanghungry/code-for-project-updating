#pragma once
#include <opencv2/core/core.hpp>
#include <vector>

using namespace std;
/******************************************************
**��˵�����������ڳ���ĳ�ʼ����Ϣ
*******************************************************/
class parameter
{
public:
	parameter();
	~parameter();

	//���۱궨����
	cv::Mat hand_to_eye;
	//����ڲ�
	vector<double> intrinsicV;
};

