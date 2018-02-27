#include "parameter_initial.h"
#include <string>
#include<vector>
#include <fstream>
#include "convert.h"
using namespace std;

parameter_initial::parameter_initial(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initial();
	connect(ui.completeButton, SIGNAL(clicked()), this, SLOT(completeinitial()));
}

parameter_initial::~parameter_initial()
{
	
}

void parameter_initial::completeinitial()
{
	string hand2eyePath = "hand_to_eye.txt";
	string intrinsicMPath = "intrinsicM.txt";
	//保存手眼标定矩阵
	ofstream ofn1(hand2eyePath.c_str());
	ofn1 << q2s(ui.lineEdit_1->text()) << "\t" << q2s(ui.lineEdit_2->text()) << "\t" << q2s(ui.lineEdit_3->text()) << "\t" << q2s(ui.lineEdit_4->text()) << "\t\n"
		<< q2s(ui.lineEdit_5->text()) << "\t" << q2s(ui.lineEdit_6->text()) << "\t" << q2s(ui.lineEdit_7->text()) << "\t" << q2s(ui.lineEdit_8->text()) << "\t\n"
		<< q2s(ui.lineEdit_9->text()) << "\t" << q2s(ui.lineEdit_10->text()) << "\t" << q2s(ui.lineEdit_11->text()) << "\t" << q2s(ui.lineEdit_12->text()) << "\t\n"
		<< q2s(ui.lineEdit_13->text()) << "\t" << q2s(ui.lineEdit_14->text()) << "\t" << q2s(ui.lineEdit_15->text()) << "\t" << q2s(ui.lineEdit_16->text()) << "\t\n";
	ofn1.close();
	//保存相机内参矩阵
	ofstream ofn2(intrinsicMPath.c_str());
	ofn2 << q2s(ui.intrinsicLine1->text()) << "\t" << q2s(ui.intrinsicLine2->text()) << "\t" << q2s(ui.intrinsicLine3->text()) << "\t" << q2s(ui.intrinsicLine4->text()) << "\t"
		<< q2s(ui.intrinsicLine5->text()) << "\t" << q2s(ui.intrinsicLine6->text()) << "\t" << q2s(ui.intrinsicLine7->text()) << "\t" << q2s(ui.intrinsicLine8->text()) << "\t"
		<< q2s(ui.intrinsicLine9->text());
	ofn2.close();
	this->close();
}

void parameter_initial::initial()
{
	string hand2eyePath = "hand_to_eye.txt";
	string intrinsicMPath = "intrinsicM.txt";
	vector<double> hand2eyeV = readData(hand2eyePath);
	vector<double> intrinsicV = readData(intrinsicMPath);
	//手眼标定
	ui.lineEdit_1->setText(s2q(d2s(hand2eyeV[0])));
	ui.lineEdit_2->setText(s2q(d2s(hand2eyeV[1])));
	ui.lineEdit_3->setText(s2q(d2s(hand2eyeV[2])));
	ui.lineEdit_4->setText(s2q(d2s(hand2eyeV[3])));
	ui.lineEdit_5->setText(s2q(d2s(hand2eyeV[4])));
	ui.lineEdit_6->setText(s2q(d2s(hand2eyeV[5])));
	ui.lineEdit_7->setText(s2q(d2s(hand2eyeV[6])));
	ui.lineEdit_8->setText(s2q(d2s(hand2eyeV[7])));
	ui.lineEdit_9->setText(s2q(d2s(hand2eyeV[8])));
	ui.lineEdit_10->setText(s2q(d2s(hand2eyeV[9])));
	ui.lineEdit_11->setText(s2q(d2s(hand2eyeV[10])));
	ui.lineEdit_12->setText(s2q(d2s(hand2eyeV[11])));
	ui.lineEdit_13->setText(s2q(d2s(hand2eyeV[12])));
	ui.lineEdit_14->setText(s2q(d2s(hand2eyeV[13])));
	ui.lineEdit_15->setText(s2q(d2s(hand2eyeV[14])));
	ui.lineEdit_16->setText(s2q(d2s(hand2eyeV[15])));
	//相机内参
	ui.intrinsicLine1->setText(s2q(d2s(intrinsicV[0])));
	ui.intrinsicLine2->setText(s2q(d2s(intrinsicV[1])));
	ui.intrinsicLine3->setText(s2q(d2s(intrinsicV[2])));
	ui.intrinsicLine4->setText(s2q(d2s(intrinsicV[3])));
	ui.intrinsicLine5->setText(s2q(d2s(intrinsicV[4])));
	ui.intrinsicLine6->setText(s2q(d2s(intrinsicV[5])));
	ui.intrinsicLine7->setText(s2q(d2s(intrinsicV[6])));
	ui.intrinsicLine8->setText(s2q(d2s(intrinsicV[7])));
	ui.intrinsicLine9->setText(s2q(d2s(intrinsicV[8])));
}
