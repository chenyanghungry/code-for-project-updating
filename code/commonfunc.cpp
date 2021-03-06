#include "commonfunc.h"

#include "PNPSolver.h"
#include "convert.h"

/*--------------------------------------------------------------
**函数说明：计算两条近似平行的线段中，其中一条线段中俩端点到另一条线段的最小距离
----------------------------------------------------------------*/
int linetoline_dist(lineinfo line1,lineinfo line2)
{
	int dist1, dist2;
	dist1= abs(line1.k*line2.x2 + line1.b - line2.y2) / sqrt(line1.k*line1.k + 1);
	dist2 = abs(line1.k*line2.x1 + line1.b - line2.y1) / sqrt(line1.k*line1.k + 1);
	if (dist1 > dist2)
	{
		return dist2;
	}
	else
		return dist1;
}

/*****************************************************************
**函数说明：求两条直线的交点
******************************************************************/
Point2f getcrosspoint(lineinfo line1, lineinfo line2)
{
	double a1 = line1.y1 - line1.y2;
	double b1 = line1.x2 - line1.x1;
	double c1 = line1.x1*line1.y2 - line1.y1*line1.x2;
	double a2 = line2.y1 - line2.y2;
	double b2 = line2.x2 - line2.x1;
	double c2 = line2.x1*line2.y2 - line2.y1*line2.x2;
	double d = a1*b2 - a2*b1;
	Point2f pt;
	pt.x = (b1*c2 - b2*c1) / d;
	pt.y = (c1*a2 - c2*a1) / d;
	return pt;
}

/*********************************************************************
**函数功能：识别四边形
**
**程序说明：通过之前求出的平行线，主要通过找存在两条线段与平行线分别相交
**********************************************************************/
model quad_recog(ntuple_list n_tuple, vector<lineinfo> lines, vector<Point2f>& pixel, model model_num)
{
	lineinfo line1,line2;
	double angle;
	int lline = 0, rline = 0;
	vector<lineinfo> lines_cross;
	#pragma region 判定四边形是那种模型
	if (model_num == RECTANGLE)
	{
		model_num = NO;
		for (size_t i = 0; i < n_tuple->size; i++)
		{
			line1.x1 = n_tuple->values[0 + i*n_tuple->dim];
			line1.y1 = n_tuple->values[1 + i * n_tuple->dim];
			line1.x2 = n_tuple->values[2 + i * n_tuple->dim];
			line1.y2 = n_tuple->values[3 + i * n_tuple->dim];
			line1.k = (line1.y1 - line1.y2) / (line1.x1 - line1.x2);
			line1.b = line1.y1 - line1.k*line1.x1;
			line1.dist = sqrt((line1.y1 - line1.y2)*(line1.y1 - line1.y2) + (line1.x1 - line1.x2) *(line1.x1 - line1.x2));
			angle = atan(abs((lines[0].k - line1.k) / (1 + lines[0].k*line1.k)));
			//angle角度非常影响结果，因为正方体在不同角度下，垂直角变化很大，此处1.13是65度
			if (angle > 1.13)//1、3模型只要要求大于80度差不多，不必要非要垂直，而且因为角度问题，可能80度都达不到，还是要考虑点相交的条件
			{
				//相交的情况要求一点与平行线一端重合，另一点与另一条一端重合
				if ((abs(line1.x1 - lines[0].x1) < 6 && abs(line1.y1 - lines[0].y1) < 6) ||
					abs(line1.x1 - lines[0].x2) < 6 && abs(line1.y1 - lines[0].y2) < 6)
				{
					if ((abs(line1.x2 - lines[1].x1) < 6 && abs(line1.y2 - lines[1].y1) < 6) ||
						abs(line1.x2 - lines[1].x2) < 6 && abs(line1.y2 - lines[1].y2) < 6)
					{
						lines_cross.push_back(line1);
						if (lines_cross.size() == 2)
						{
							if (abs(line1.dist-lines[0].dist)<25)
							{
								model_num = SQUARE;
								break;
							}
							else if (abs(line1.dist - 3 * lines[0].dist)<25 || abs(3*line1.dist - lines[0].dist)<25)
							{
								model_num = RECTANGLE;
								break;
							}
						}
					}
				}
				else if ((abs(line1.x1 - lines[1].x1) < 6 && abs(line1.y1 - lines[1].y1) < 6) ||
					abs(line1.x1 - lines[1].x2) < 6 && abs(line1.y1 - lines[01].y2) < 6)
				{
					if ((abs(line1.x2 - lines[0].x1) < 6 && abs(line1.y2 - lines[0].y1) < 6) ||
						abs(line1.x2 - lines[0].x2) < 6 && abs(line1.y2 - lines[0].y2) < 6)
					{
						lines_cross.push_back(line1);
						if (lines_cross.size() == 2)
						{
							if (abs(line1.dist - lines[0].dist)<15)
							{
								model_num = SQUARE;
								break;
							}
							else if (abs(line1.dist - 3 * lines[0].dist)<15 || abs(3 * line1.dist - lines[0].dist)<15)
							{
								model_num = RECTANGLE;
								break;
							}
						}
					}
				}
			}

		}
	}
	else if (model_num == TRAPEZOID)//根据平行但不相等的线段找梯形的另外两个线段
	{
		model_num = NO;
		for (size_t i = 0; i < n_tuple->size; i++)
		{
			line1.x1 = n_tuple->values[0 + i*n_tuple->dim];
			line1.y1 = n_tuple->values[1 + i * n_tuple->dim];
			line1.x2 = n_tuple->values[2 + i * n_tuple->dim];
			line1.y2 = n_tuple->values[3 + i * n_tuple->dim];
			line1.k = (line1.y1 - line1.y2) / (line1.x1 - line1.x2);
			line1.b = line1.y1 - line1.k*line1.x1;
			line1.dist = sqrt((line1.y1 - line1.y2)*(line1.y1 - line1.y2) + (line1.x1 - line1.x2) *(line1.x1 - line1.x2));
			
			if ((abs(line1.x1 - lines[0].x1)<6 && abs(line1.y1 - lines[0].y1)<6) ||
				abs(line1.x1 - lines[0].x2)<6 && abs(line1.y1 - lines[0].y2)<6)
			{ 
				if ((abs(line1.x2 - lines[1].x1)<6 && abs(line1.y2 - lines[1].y1)<6) ||
					abs(line1.x2 - lines[1].x2)<6 && abs(line1.y2 - lines[1].y2)<6)
				{
					lines_cross.push_back(line1);
					if (lines_cross.size() == 2)
					{
						model_num = TRAPEZOID;
						break;
					}
				}
			}
			else if ((abs(line1.x1 - lines[1].x1)<6 && abs(line1.y1 - lines[1].y1)<6) ||
				abs(line1.x1 - lines[1].x2)<6 && abs(line1.y1 - lines[01].y2)<6)
			{
				if ((abs(line1.x2 - lines[0].x1)<6 && abs(line1.y2 - lines[0].y1)<6) ||
					abs(line1.x2 - lines[0].x2)<6 && abs(line1.y2 - lines[0].y2)<6)
				{
					lines_cross.push_back(line1);
					if (lines_cross.size() == 2)
					{
						model_num = TRAPEZOID;
						break;
					}
				}
			}
		}
	}
#pragma endregion

	/*点的顺序还有待进一步确定，程序中都是右手坐标系，如果这个不是的话，对结果的影响是很大的*/
	Point2f pt;
	if (lines_cross.size() == 2)
	{
		if (model_num == SQUARE)
		{
			pt = getcrosspoint(lines[0], lines_cross[0]);
			pixel.push_back(pt);
			pt = getcrosspoint(lines_cross[0], lines[1]);
			pixel.push_back(pt);
			pt = getcrosspoint(lines[1], lines_cross[1]);
			pixel.push_back(pt);
			pt = getcrosspoint(lines_cross[1], lines[0]);
			pixel.push_back(pt);
			return SQUARE;
		}
		else if (model_num ==	TRAPEZOID)
		{
			pt = getcrosspoint(lines[0], lines_cross[0]);
			pixel.push_back(pt);
			pt = getcrosspoint(lines_cross[0], lines[1]);
			pixel.push_back(pt);
			pt = getcrosspoint(lines[1], lines_cross[1]);
			pixel.push_back(pt);
			pt = getcrosspoint(lines_cross[1], lines[0]);
			pixel.push_back(pt);
			return TRAPEZOID;
		}
		else if (model_num == RECTANGLE)
		{
			if (lines[0].dist > 2 * lines_cross[0].dist)
			{
				pt = getcrosspoint(lines[0], lines_cross[0]);
				pixel.push_back(pt);
				pt = getcrosspoint(lines_cross[0], lines[1]);
				pixel.push_back(pt);
				pt = getcrosspoint(lines[1], lines_cross[1]);
				pixel.push_back(pt);
				pt = getcrosspoint(lines_cross[1], lines[0]);
				pixel.push_back(pt);
				return RECTANGLE;
			}
			else
			{
				pt = getcrosspoint(lines_cross[0], lines[0]);
				pixel.push_back(pt);
				pt = getcrosspoint(lines[0], lines_cross[1]);
				pixel.push_back(pt);
				pt = getcrosspoint(lines_cross[1], lines[1]);
				pixel.push_back(pt);
				pt = getcrosspoint(lines[1], lines_cross[0]);
				pixel.push_back(pt);
				return RECTANGLE;
			}
		}
		else
			cout << "四边形提取有误" << endl;
		return NO;
	}
	else
	{
		return NO;
	}
}

/***************************************************************************
**函数说明：将LSD检测的线按X轴的从大到小排序，而且每条线的两个点也按从大到小排序
****************************************************************************/
void sortline(ntuple_list ntl)
{
	double temp_x1, temp_y1, temp_x2, temp_y2;
	for (size_t i = 0; i < ntl->size; i++)
	{
		if (ntl->values[0 + i*ntl->dim]<ntl->values[2 + i*ntl->dim])
		{
			temp_x1 = ntl->values[0 + i*ntl->dim];
			temp_y1 = ntl->values[1 + i*ntl->dim];
			ntl->values[0 + i*ntl->dim] = ntl->values[2 + i*ntl->dim];
			ntl->values[1 + i*ntl->dim] = ntl->values[3 + i*ntl->dim];
			ntl->values[2 + i*ntl->dim] = temp_x1;
			ntl->values[3 + i*ntl->dim] = temp_y1;
		}
	}
	for (size_t i = 0; i < ntl->size - 1; i++)
	{
		for (int k = i + 1; k < ntl->size; ++k)
		{
			if (ntl->values[0 + i*ntl->dim]< ntl->values[0 + k*ntl->dim])
			{
				temp_x1 = ntl->values[0 + i*ntl->dim];
				temp_y1 = ntl->values[1 + i*ntl->dim];
				temp_x2 = ntl->values[2 + i*ntl->dim];
				temp_y2 = ntl->values[3 + i*ntl->dim];
				ntl->values[0 + i*ntl->dim] = ntl->values[0 + k*ntl->dim];
				ntl->values[1 + i*ntl->dim] = ntl->values[1 + k*ntl->dim];
				ntl->values[2 + i*ntl->dim] = ntl->values[2 + k*ntl->dim];
				ntl->values[3 + i*ntl->dim] = ntl->values[3 + k*ntl->dim];
				ntl->values[0 + k*ntl->dim] = temp_x1;
				ntl->values[1 + k*ntl->dim] = temp_y1;
				ntl->values[2 + k*ntl->dim] = temp_x2;
				ntl->values[3 + k*ntl->dim] = temp_y2;
			}
		}
	}
}

/*********************************************************************
**函数功能：合并同一条直线上的线段
**********************************************************************/
void mergeline(ntuple_list ntl,ntuple_list n_tuple_merge)
{
	lineinfo line1, line2;
	double slope_t;//两条线斜率的角度
	int *p = new int[ntl->size];//为每条线段设置标志，如果已经融入了其他线，就不再计算
	double line_dist;//计算合并后线的长度，防止过长错误
	for (size_t i = 0; i < ntl->size; i++)
	{
		p[i] = 0;
	}
	for (int i = 0; i < ntl->size; ++i)
	{
		if (p[i] == 1)
		{
			continue;
		}
		line1.x1 = ntl->values[0 + i*ntl->dim];
		line1.y1 = ntl->values[1 + i * ntl->dim];
		line1.x2 = ntl->values[2 + i * ntl->dim];
		line1.y2 = ntl->values[3 + i * ntl->dim];
		line1.k = (line1.y1 - line1.y2) / (line1.x1 - line1.x2);
		line1.b = line1.y1 - line1.k*line1.x1;
		line1.dist = sqrt((line1.y1 - line1.y2)*(line1.y1 - line1.y2) + (line1.x1 - line1.x2) *(line1.x1 - line1.x2));
		for (int k = i + 1; k != ntl->size; ++k)
		{
			line2.x1 = ntl->values[0 + k*ntl->dim];
			line2.y1 = ntl->values[1 + k * ntl->dim];
			line2.x2 = ntl->values[2 + k * ntl->dim];
			line2.y2 = ntl->values[3 + k * ntl->dim];
			line2.k = (line2.y1 - line2.y2) / (line2.x1 - line2.x2);
			line2.b = line2.y1 - line2.k*line2.x1;
			slope_t = atan(abs((line2.k - line1.k) / (1 + line2.k*line1.k)));
			if (slope_t<0.05)//重合线相差角度不差过2.86度
			{
				if (linetoline_dist(line1,line2)<5)//两条线之间的距离小于5像素
				{
					//两条挨近的线端点距离
					if (sqrt((line1.x2 - line2.x1)*(line1.x2 - line2.x1) + (line1.y2 - line2.y1)*(line1.y2 - line2.y1))<20)
					{
						p[k] = 1;
						line1.x2 = line2.x2;
						line1.y2 = line2.y2;
					}				
				}

			}
		}
		line_dist = sqrt((line1.y1 - line1.y2)*(line1.y1 - line1.y2) + (line1.x1 - line1.x2) *(line1.x1 - line1.x2));
		if (line_dist>250||line_dist<15)
		{
			continue;
		}
		add_5tuple(n_tuple_merge, line1.x1, line1.y1, line1.x2, line1.y2, 2);
	}
	delete p;
}

/***************************************************************************
**函数说明：寻找模型的四边形
****************************************************************************/
void find_quad(ntuple_list n_tuple_merge, cv::Mat temp,vector<pair<vector<Point2f>, model> >& pixels)
{
	model model_num=NO;//模型枚举
	lineinfo line1, line2;
	vector<lineinfo> lines1;//保存两条平行线段
	vector<Point2f> pixel;//保存每次提取出四边形的四个点且有顺序，所以后面可以直接匹配相应的 点
	Point2f pt1,pt2;//用于检测出来的模型是否是已经检测过的
	//模型重复标志
	int num = 0;
	int square1 = 0, trapezoid1 = 0, rectangle1 = 0;
	
	for (size_t i = 0; i < n_tuple_merge->size - 1; i++)
	{
		line1.x1 = n_tuple_merge->values[i*n_tuple_merge->dim + 0];
		line1.y1 = n_tuple_merge->values[i*n_tuple_merge->dim + 1];
		line1.x2 = n_tuple_merge->values[i*n_tuple_merge->dim + 2];
		line1.y2 = n_tuple_merge->values[i*n_tuple_merge->dim + 3];
		line1.k = (line1.y1 - line1.y2) / (line1.x1 - line1.x2);
		line1.b = line1.y1 - line1.k*line1.x1;
		line1.dist = sqrt((line1.y1 - line1.y2)*(line1.y1 - line1.y2) + (line1.x1 - line1.x2) * (line1.x1 - line1.x2));
		
		for (size_t j = i + 1; j < n_tuple_merge->size; j++)
		{
			line2.x1 = n_tuple_merge->values[j*n_tuple_merge->dim + 0];
			line2.y1 = n_tuple_merge->values[j*n_tuple_merge->dim + 1];
			line2.x2 = n_tuple_merge->values[j*n_tuple_merge->dim + 2];
			line2.y2 = n_tuple_merge->values[j*n_tuple_merge->dim + 3];
			line2.k = (line2.y1 - line2.y2) / (line2.x1 - line2.x2);
			line2.b = line2.y1 - line2.k*line2.x1;
			line2.dist = sqrt((line2.y1 - line2.y2)*(line2.y1 - line2.y2) + (line2.x1 - line2.x2) * (line2.x1 - line2.x2));
		
			//此处平行的精度没有合并时那么严格，存在光照的影响导致不是特别平行
			if (atan(abs((line1.k - line2.k) / (1 + line1.k*line2.k))) < 0.087)//小于5度
			{
				//！！！此处受图片尺寸影响较大，里面的平行线匹配，理论上也只有1倍和两倍的差别，暂时没有出错，
				//不知道模型受光照影响尺寸会不会很大
				if ((linetoline_dist(line1, line2) < 250) && (linetoline_dist(line1, line2) > 15))
				{
					if (abs(line1.dist - line2.dist) < 20)
					{
						lines1.push_back(line1);
						lines1.push_back(line2);
						model_num = quad_recog(n_tuple_merge, lines1, pixel, RECTANGLE);
					}
					else if (abs(line1.dist - 2 * line2.dist) < 20)
					{
						lines1.push_back(line2);
						lines1.push_back(line1);
						model_num = quad_recog(n_tuple_merge, lines1, pixel, TRAPEZOID);
					}
					else if (abs(2 * line1.dist - line2.dist) < 20)
					{
						lines1.push_back(line1);
						lines1.push_back(line2);
						model_num = quad_recog(n_tuple_merge, lines1, pixel,TRAPEZOID);
					}
					else
					{
						continue;
					}
				}
			}
			switch (model_num)
			{
			case SQUARE:
				if (square1)
				{
					pt1.x = pixel[0].x;
					pt1.y = pixel[0].y;
					for (size_t i = 0; i < pixels[pixels.size()-1].first.size(); i++)
					{
						pt2.x = pixels[pixels.size() - 1].first[i].x;
						pt2.y = pixels[pixels.size() - 1].first[i].y;
						if (abs(pt1.x-pt2.x)<6&&abs(pt1.y-pt2.y)<6)
						{
							num++;
						}
					}
					if (num == 0)
					{
						pixels.push_back(pair<vector<Point2f>, model>(pixel, SQUARE));
						square1++;
					}
				}
				else
				{
					pixels.push_back(pair<vector<Point2f>, model>(pixel, SQUARE));
					square1++;
				}
				
				break;
			case TRAPEZOID:
				if (trapezoid1)
				{
					pt1.x = pixel[0].x;
					pt1.y = pixel[0].y;
					for (size_t i = 0; i < pixels[pixels.size() - 1].first.size(); i++)
					{
						pt2.x = pixels[pixels.size() - 1].first[i].x;
						pt2.y = pixels[pixels.size() - 1].first[i].y;
						if (abs(pt1.x - pt2.x)<6 && abs(pt1.y - pt2.y)<6)
						{
							num++;
						}
					}
					if (num == 0)
					{
						pixels.push_back(pair<vector<Point2f>, model>(pixel, TRAPEZOID));
						trapezoid1++;
					}
				}
				else
				{
					pixels.push_back(pair<vector<Point2f>, model>(pixel, TRAPEZOID));
					trapezoid1++;
				}
				break;
			case RECTANGLE:
				if (rectangle1)
				{
					pt1.x = pixel[0].x;
					pt1.y = pixel[0].y;
					for (size_t i = 0; i < pixels[pixels.size() - 1].first.size(); i++)
					{
						pt2.x = pixels[pixels.size() - 1].first[i].x;
						pt2.y = pixels[pixels.size() - 1].first[i].y;
						if (abs(pt1.x - pt2.x)<10 && abs(pt1.y - pt2.y)<10)
						{
							num++;
						}
					}
					if (num == 0)
					{
						pixels.push_back(pair<vector<Point2f>, model>(pixel, RECTANGLE));
						rectangle1++;
					}
				}
				else
				{
					pixels.push_back(pair<vector<Point2f>, model>(pixel, RECTANGLE));
					rectangle1++;
				}
				break;
			default:
				if (lines1.size()!=0)
				{
					lines1.clear();
				}
				break;
			}
			num = 0;
			if (model_num!=NO)
			{
				model_num = NO;
				lines1.clear();
				pixel.clear();
				break;
			}
		}
	}
	
}

/*************************************************************************
**函数说明：QT界面输入路径后的处理函数，与之前的C++程序接入的主要程序
**
**参数说明：path：待处理图片的路径 showpath是处理图片路径，很多时候这个是一样的，只是后来加入了截图功能，所以把截图后的图片另存为path了，所以才有了区别
			pts：图片的起始点坐标，为了与截图功能区别
			results：保存需要在QT上显示的数据
**************************************************************************/
void procfunc(string path,string showpath, Point2f pts, vector<vector<string>>& results)
{
	vector<string> result;//每个result保存一个物体的位姿，多个物体存入results中
	Mat src = imread(path, CV_LOAD_IMAGE_COLOR);
	Mat temp, src_gray;
	cvtColor(src, temp, CV_RGB2GRAY);
	temp.convertTo(src_gray, CV_64FC1);
	int cols = src_gray.cols;
	int rows = src_gray.rows;
	image_double image = new_image_double(cols, rows);
	image->data = src_gray.ptr<double>(0);
	ntuple_list ntl = lsd(image);

	sortline(ntl);

	ntuple_list n_tuple_merge = new_ntuple_list(5);
	mergeline(ntl, n_tuple_merge);
	free_ntuple_list(ntl);

	vector<pair<vector<Point2f>, model>> pixels;//保存目标物体以及对应的四个点坐标
	find_quad(n_tuple_merge, src, pixels);
	free_ntuple_list(n_tuple_merge);

	//定义手眼矩阵
	//double m[4][4] = { { 0.9998, -0.0047, -0.0192, 2.5602 }, { 0.0046, 1, -0.0029, -117.9984 }, { 0.0192, 0.0028, 0.9998, 221.0612 }, {0,0,0,1} };
	cv::Mat hand_eye_M = cv::Mat(4, 4, CV_64FC1);//手眼标定矩阵
	string handtoeyepath = "hand_to_eye.txt";
	vector<double> hand_to_eye_vec = readData(handtoeyepath);
	for (int i = 0; i < 4;i++)
	{
		for (int j = 0; j < 4;j++)
		{
			hand_eye_M.at<double>(i, j) = hand_to_eye_vec[i*4+j];
		}
	}
	//计算基座与末端矩阵
	string path_base = "end_to_base.txt";
	vector<double> end_to_base_vec = readData(path_base);//基座与末端矩阵
	cv::Mat end_to_base_matrix(4, 4, CV_64FC1);
	vec2matrix(end_to_base_vec, end_to_base_matrix);

	//PNP
	/*************相机物理参数***********/
	string intrinsicPath = "intrinsicM.txt";
	vector<double> intrinsicV = readData(intrinsicPath);
	//内参数
	double fx = intrinsicV[0];
	double fy = intrinsicV[1];
	double u0 = intrinsicV[2];
	double v0 = intrinsicV[3];
	//镜头畸变参数
	double k1 = intrinsicV[4];
	double k2 = intrinsicV[5];
	double p1 = intrinsicV[6];
	double p2 = intrinsicV[7];
	double k3 = intrinsicV[8];

	vector<Point3f> Point3D;
	PNPSolver p4psolver;
	//初始化相机参数
	p4psolver.SetCameraMatrix(fx, fy, u0, v0);
	//设置畸变参数
	p4psolver.SetDistortionCoefficients(k1, k2, p1, p2, k3);
	
	Mat imageshow;//保存检测后画线的图片
	if (pts==Point2f(0,0))
	{
		imageshow=imread(path,CV_LOAD_IMAGE_COLOR);
	}
	else
		imageshow=imread(showpath,CV_LOAD_IMAGE_COLOR);
	
	Point2f pt11,pt12;
	int width=0;
	for (size_t i = 0; i < pixels.size(); i++)
	{
		switch (pixels[i].second)
		{
		case SQUARE:
			result.push_back("SQUARE:");
			p4psolver.Points3D.push_back(Point3f(5, -5, 0));
			p4psolver.Points3D.push_back(Point3f(5, 5, 0));
			p4psolver.Points3D.push_back(Point3f(-5, 5, 0));
			p4psolver.Points3D.push_back(Point3f(-5, -5, 0));
			break;
		case TRAPEZOID:
			result.push_back("TRAPEZOID:");
			p4psolver.Points3D.push_back(Point3f(5, -5, 0));
			p4psolver.Points3D.push_back(Point3f(11, 5, 0));
			p4psolver.Points3D.push_back(Point3f(-11, 5, 0));
			p4psolver.Points3D.push_back(Point3f(-5, -5, 0));
			break;
		case RECTANGLE:
			result.push_back("RECTANGLE:");
			p4psolver.Points3D.push_back(Point3f(-5, -15, 0));
			p4psolver.Points3D.push_back(Point3f(5, -15, 0));
			p4psolver.Points3D.push_back(Point3f(5, 15, 0));
			p4psolver.Points3D.push_back(Point3f(-5, 15, 0));
			break;
		default:
			break;
		}
		if (pts==Point2f(0,0))
		{
			for (size_t j = 0; j < pixels[i].first.size(); j++)
			{
				p4psolver.Points2D.push_back(Point2f(pixels[i].first[j].x, pixels[i].first[j].y));
			}
			for (int tt = 0; tt !=  pixels[i].first.size() - 1; ++tt)
			{
				pt11.x = int(pixels[i].first[tt].x);
				pt11.y = int(pixels[i].first[tt].y);
				pt12.x = int(pixels[i].first[tt+1].x);
				pt12.y = int(pixels[i].first[tt+1].y);
				width = 3;
				cv::line(imageshow, pt11, pt12, cv::Scalar(0,255,255), width, CV_AA);
			}
			pt11.x = int(pixels[i].first[3].x);
			pt11.y = int(pixels[i].first[3].y);
			pt12.x = int(pixels[i].first[0].x);
			pt12.y = int(pixels[i].first[0].y);
			width = 3;
			cv::line(imageshow, pt11, pt12, cv::Scalar(0,255,255), width, CV_AA);
		}
		else
		{
			for (size_t j = 0; j < pixels[i].first.size(); j++)
			{
				p4psolver.Points2D.push_back(Point2f(pixels[i].first[j].x+pts.x, pixels[i].first[j].y+pts.y));
			}
			for (int tt = 0; tt !=  pixels[i].first.size() - 1; ++tt)
			{
				pt11.x = int(pixels[i].first[tt].x+pts.x);
				pt11.y = int(pixels[i].first[tt].y+pts.y);
				pt12.x = int(pixels[i].first[tt+1].x+pts.x);
				pt12.y = int(pixels[i].first[tt+1].y+pts.y);
				width = 3;
				cv::line(imageshow, pt11, pt12, cv::Scalar(255,255,0), width, CV_AA);
			}
			pt11.x = int(pixels[i].first[3].x+pts.x);
			pt11.y = int(pixels[i].first[3].y+pts.y);
			pt12.x = int(pixels[i].first[0].x+pts.x);
			pt12.y = int(pixels[i].first[0].y+pts.y);
			width = 3;
			cv::line(imageshow, pt11, pt12, cv::Scalar(255,255,0), width, CV_AA);
		}
		if (p4psolver.Solve(PNPSolver::METHOD::CV_ITERATIVE) == 0)
		{
			vector <double> pose=threeM2pose(end_to_base_matrix,hand_eye_M,p4psolver.RTM,i+1);
			result.push_back(d2s(pose[0]));
			result.push_back(d2s(pose[1]));
			result.push_back(d2s(pose[2]));
			result.push_back(d2s(pose[3]));
			result.push_back(d2s(pose[4]));
			result.push_back(d2s(pose[5]));
		}
		p4psolver.Points2D.clear();
		p4psolver.Points3D.clear();
		results.push_back(result);
		result.clear();
	}
	imwrite("..\\resultshow.bmp",imageshow);
}

vector<double> threeM2pose(cv::Mat end2base,cv::Mat handeye, cv::Mat b2c,int i)
{
	vector<double> pose;
	string filename1 = "matrix" + d2s((double)i)+".txt";
	const char* fileM = filename1.c_str();
	string filename2 = "vector" + d2s((double)i) + ".txt";
	const char* fileV = filename2.c_str();
	cv::Mat M=end2base*handeye*b2c;
	pose.push_back(M.at<double>(0,3));
	pose.push_back(M.at<double>(1,3));
	pose.push_back(M.at<double>(2,3));

	double thetaz = atan2(M.at<double>(1,0), M.at<double>(0,0)) / CV_PI * 180;
	double thetay = atan2(-1 * M.at<double>(2,0), sqrt(M.at<double>(2,1)*M.at<double>(2,1) + M.at<double>(2,2)*M.at<double>(2,2))) / CV_PI * 180;
	double thetax = atan2(M.at<double>(2,1), M.at<double>(2,2)) / CV_PI * 180;
	pose.push_back(thetax);
	pose.push_back(thetay);
	pose.push_back(thetaz);
	ofstream ofn(fileM);
	ofn << M.at<double>(0, 0) << "\t" << M.at<double>(0, 1) << "\t" << M.at<double>(0, 2) << "\t" << M.at<double>(0, 3) << "\t\n";
	ofn << M.at<double>(1, 0) << "\t" << M.at<double>(1, 1) << "\t" << M.at<double>(1, 2) << "\t" << M.at<double>(1, 3) << "\t\n";
	ofn << M.at<double>(2, 0) << "\t" << M.at<double>(2, 1) << "\t" << M.at<double>(2, 2) << "\t" << M.at<double>(2, 3) << "\t\n";
	ofn << 0 << "\t" << 0 << "\t" << 0 << "\t" << 1 << "\t\n";
	ofn.close();
	ofstream ofnv(fileV);
	ofnv << M.at<double>(0, 3) << "\t" << M.at<double>(1, 3) << "\t" << M.at<double>(2, 3) << "\t" << thetax << "\t" << thetay << "\t" << thetaz;
	ofnv.close();
	return pose;

}

/****************************************************************************************
**函数说明：鼠标回调函数
*****************************************************************************************/
void on_mouse(int event, int x, int y, int flags, void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号  
{
	pair<IplImage*, Point3f*> *temp2 = (pair<IplImage*, Point3f*>*)ustc;
	IplImage *srcImage = (*temp2).first;
	Point3f *pt = (*temp2).second;
	Mat org = Mat(srcImage);
	Mat img, tmp, dst;
	org.copyTo(img);
	static Point pre_pt = (-1, -1);//初始坐标  
	static Point cur_pt = (-1, -1);//实时坐标  
	char temp[16];
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆  
	{
		org.copyTo(img);//将原始图片复制到img中  
		sprintf(temp, "(%d,%d)", x, y);
		pre_pt = Point(x, y);
		putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//在窗口上显示坐标  
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//划圆  
		imshow("img", img);
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数  
	{
		img.copyTo(tmp);//将img复制到临时图像tmp上，用于显示实时坐标  
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//只是实时显示鼠标移动的坐标  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形  
	{
		img.copyTo(tmp);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_LBUTTONUP)//左键松开，将在图像上划矩形  
	{
		org.copyTo(img);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(img, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//根据初始点和结束点，将矩形画到img上  
		imshow("img", img);
		img.copyTo(tmp);
		//截取矩形包围的图像，并保存到dst中  
		int width = abs(pre_pt.x - cur_pt.x);
		int height = abs(pre_pt.y - cur_pt.y);

		dst = org(Rect(min(cur_pt.x, pre_pt.x), min(cur_pt.y, pre_pt.y), width, height));
		//记录截图的起始点，为后面恢复图片在实际图片中保存数据，z主要是为了判别是否做了截取
		(*pt).x = min(cur_pt.x, pre_pt.x);
		(*pt).y = min(cur_pt.y, pre_pt.y);
		(*pt).z = 0.1;
		namedWindow("dst");
		imshow("dst", dst);
		imwrite("..\\shot.bmp", dst);
		waitKey(1);
		destroyAllWindows();
	}
}

int pointshot(string path, vector<Point2f> points, vector<string> &result)
{
	lineinfo line1, line2, line3, line4;//定义四条线段，以下来判断它们分别属于什么形状
	model model_num=NO;
	#pragma region 构造四条线
	line1.x1=points[0].x;
	line1.y1=points[0].y;
	line1.x2=points[1].x;
	line1.y2=points[1].y;
	line1.k=(line1.y1-line1.y2)/(line1.x1-line1.x2);
	line1.b=line1.y1-line1.k*line1.x1;
	line1.dist = sqrt((line1.y1 - line1.y2)*(line1.y1 - line1.y2) + (line1.x1 - line1.x2)*(line1.x1 - line1.x2));

	line2.x1 = points[2].x;
	line2.y1 = points[2].y;
	line2.x2 = points[3].x;
	line2.y2 = points[3].y;
	line2.k = (line2.y1 - line2.y2) / (line2.x1 - line2.x2);
	line2.b = line2.y1 - line2.k*line2.x1;
	line2.dist = sqrt((line2.y1 - line2.y2)*(line2.y1 - line2.y2) + (line2.x1 - line2.x2)*(line2.x1 - line2.x2));

	line3.x1 = points[2].x;
	line3.y1 = points[2].y;
	line3.x2 = points[1].x;
	line3.y2 = points[1].y;
	line3.k = (line3.y1 - line3.y2) / (line3.x1 - line3.x2);
	line3.b = line3.y1 - line3.k*line3.x1;
	line3.dist = sqrt((line3.y1 - line3.y2)*(line3.y1 - line3.y2) + (line3.x1 - line3.x2)*(line3.x1 - line3.x2));

	line4.x1 = points[3].x;
	line4.y1 = points[3].y;
	line4.x2 = points[0].x;
	line4.y2 = points[0].y;
	line4.k = (line4.y1 - line4.y2) / (line4.x1 - line4.x2);
	line4.b = line4.y1 - line4.k*line4.x1;
	line4.dist = sqrt((line4.y1 - line4.y2)*(line4.y1 - line4.y2) + (line4.x1 - line4.x2)*(line4.x1 - line4.x2));
	#pragma endregion

	if ((atan(abs(line1.k-line2.k)/(1+line1.k*line2.k))<0.349)&&(atan(abs(line3.k-line4.k)/(1+line3.k*line4.k))<0.349))//20度
	{
		if (abs(line1.dist - line2.dist) < 25)
		{
			if (abs(line1.dist-line3.dist)<25)
			{
				model_num = SQUARE;
			}
			else if (abs(line1.dist-3*line3.dist)<25||abs(line1.dist*3-line3.dist)<25)
			{
				model_num = RECTANGLE;
			}
		}
	}
	else
	{
		if ((atan(abs(line1.k - line2.k) / (1 + line1.k*line2.k))<0.349) || (atan(abs(line3.k - line4.k) / (1 + line3.k*line4.k))<0.349))
		{
			if (abs(line1.dist - 2 * line2.dist)<25 || abs(line3.dist - 2*line4.dist)<25 
				|| abs(2 * line1.dist - line2.dist)<25 || abs(2*line3.dist -  line4.dist)<25)
			{
				model_num = TRAPEZOID;
			}
		}
	}
	vector<Point2f> new_points;
	if (model_num==SQUARE)
	{
		new_points = points;
	}
	else if (model_num==RECTANGLE)
	{
		if (line1.dist>line3.dist)
		{
			new_points.push_back(points[1]);
			new_points.push_back(points[2]);
			new_points.push_back(points[3]);
			new_points.push_back(points[0]);
		}
		else
		{
			new_points = points;
		}
	}
	else if (model_num==TRAPEZOID)
	{
		if (abs(line1.dist-2*line2.dist)<25)
		{
			new_points.push_back(points[3]);
			new_points.push_back(points[0]);
			new_points.push_back(points[1]);
			new_points.push_back(points[2]);
		}
		else if (abs(2*line1.dist-line2.dist)<25)
		{
			new_points.push_back(points[1]);
			new_points.push_back(points[2]);
			new_points.push_back(points[3]);
			new_points.push_back(points[0]);
		}
		else if (abs(line3.dist*2-line4.dist)<25)
		{
			new_points.push_back(points[2]);
			new_points.push_back(points[3]);
			new_points.push_back(points[0]);
			new_points.push_back(points[1]);
		}
		else if (abs(line3.dist-2*line4.dist)<25)
		{
			new_points = points;
		}
	}

	//定义手眼矩阵
	cv::Mat hand_eye_M = cv::Mat(4, 4, CV_64FC1);
	string handtoeyepath = "hand_to_eye.txt";
	vector<double> hand_to_eye_vec = readData(handtoeyepath);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			hand_eye_M.at<double>(i, j) = hand_to_eye_vec[i * 4 + j];
		}
	}
	//计算基座与末端矩阵
	string path_base = "end_to_base.txt";
	vector<double> end_to_base_vec = readData(path_base);
	cv::Mat end_to_base_matrix(4, 4, CV_64FC1);
	vec2matrix(end_to_base_vec, end_to_base_matrix);

	//PNP
	/*************相机物理参数***********/
	string intrinsicPath = "intrinsicM.txt";
	vector<double> intrinsicV = readData(intrinsicPath);
	//内参数
	double fx = intrinsicV[0];
	double fy = intrinsicV[1];
	double u0 = intrinsicV[2];
	double v0 = intrinsicV[3];
	//镜头畸变参数
	double k1 = intrinsicV[4];
	double k2 = intrinsicV[5];
	double p1 = intrinsicV[6];
	double p2 = intrinsicV[7];
	double k3 = intrinsicV[8];
	
	PNPSolver p4psolver;
	//初始化相机参数
	p4psolver.SetCameraMatrix(fx, fy, u0, v0);
	//设置畸变参数
	p4psolver.SetDistortionCoefficients(k1, k2, p1, p2, k3);
	switch (model_num)
	{
	case SQUARE:
		result.push_back("SQUARE:");
		p4psolver.Points3D.push_back(Point3f(5, -5, 0));
		p4psolver.Points3D.push_back(Point3f(5, 5, 0));
		p4psolver.Points3D.push_back(Point3f(-5, 5, 0));
		p4psolver.Points3D.push_back(Point3f(-5, -5, 0));
		break;
	case TRAPEZOID:
		result.push_back("TRAPEZOID:");
		p4psolver.Points3D.push_back(Point3f(5, -5, 0));
		p4psolver.Points3D.push_back(Point3f(11, 5, 0));
		p4psolver.Points3D.push_back(Point3f(-11, 5, 0));
		p4psolver.Points3D.push_back(Point3f(-5, -5, 0));
		break;
	case RECTANGLE:
		result.push_back("RECTANGLE:");
		p4psolver.Points3D.push_back(Point3f(-5, -15, 0));
		p4psolver.Points3D.push_back(Point3f(5, -15, 0));
		p4psolver.Points3D.push_back(Point3f(5, 15, 0));
		p4psolver.Points3D.push_back(Point3f(-5, 15, 0));
		break;
	case NO:
		return 0;
	default:
		break;
	}
	Mat imageshow = imread(path);
	for (size_t i = 0; i < new_points.size();i++)
	{
		p4psolver.Points2D.push_back(Point2f(new_points[i].x, new_points[i].y));
	}
	cv::line(imageshow, new_points[0], new_points[1], cv::Scalar(0, 255, 255), 3, CV_AA);
	cv::line(imageshow, new_points[1], new_points[2], cv::Scalar(0, 255, 255), 3, CV_AA);
	cv::line(imageshow, new_points[2], new_points[3], cv::Scalar(0, 255, 255), 3, CV_AA);
	cv::line(imageshow, new_points[3], new_points[0], cv::Scalar(0, 255, 255), 3, CV_AA);
	if (p4psolver.Solve(PNPSolver::METHOD::CV_ITERATIVE)==0)
	{
		vector <double> pose = threeM2pose(end_to_base_matrix, hand_eye_M, p4psolver.RTM,1);
		result.push_back(d2s(pose[0]));
		result.push_back(d2s(pose[1]));
		result.push_back(d2s(pose[2]));
		result.push_back(d2s(pose[3]));
		result.push_back(d2s(pose[4]));
		result.push_back(d2s(pose[5]));
	}
	p4psolver.Points2D.clear();
	p4psolver.Points3D.clear();
	imwrite("..\\resultshow.bmp", imageshow);
	return 1;
}






