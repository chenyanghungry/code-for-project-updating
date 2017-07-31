#include "commonfunc.h"

#include "PNPSolver.h"
#include "convert.h"

/*--------------------------------------------------------------
**����˵����������������ƽ�е��߶��У�����һ���߶������˵㵽��һ���߶ε���С����
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
**����˵����������ֱ�ߵĽ���
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
**�������ܣ�ʶ���ı���
**
**����˵����ͨ��֮ǰ�����ƽ���ߣ���Ҫͨ���Ҵ��������߶���ƽ���߷ֱ��ཻ
**********************************************************************/
model quad_recog(ntuple_list n_tuple, vector<lineinfo> lines, vector<Point2f>& pixel, model model_num)
{
	lineinfo line1,line2;
	double angle;
	int lline = 0, rline = 0;
	vector<lineinfo> lines_cross;
	#pragma region �ж��ı���������ģ��
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
			//angle�Ƕȷǳ�Ӱ��������Ϊ�������ڲ�ͬ�Ƕ��£���ֱ�Ǳ仯�ܴ󣬴˴�1.13��65��
			if (angle > 1.13)//1��3ģ��ֻҪҪ�����80�Ȳ�࣬����Ҫ��Ҫ��ֱ��������Ϊ�Ƕ����⣬����80�ȶ��ﲻ��������Ҫ���ǵ��ཻ������
			{
				//�ཻ�����Ҫ��һ����ƽ����һ���غϣ���һ������һ��һ���غ�
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
	else if (model_num == TRAPEZOID)//����ƽ�е�����ȵ��߶������ε����������߶�
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

	/*���˳���д���һ��ȷ���������ж�����������ϵ�����������ǵĻ����Խ����Ӱ���Ǻܴ��*/
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
			cout << "�ı�����ȡ����" << endl;
		return NO;
	}
	else
	{
		return NO;
	}
}

/***************************************************************************
**����˵������LSD�����߰���С�������򣬶���ÿ���ߵ�������Ҳ���Ӵ�С����
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
**�������ܣ��ϲ�ͬһ��ֱ���ϵ��߶�
**********************************************************************/
void mergeline(ntuple_list ntl,ntuple_list n_tuple_merge)
{
	lineinfo line1, line2;
	double slope_t;//������б�ʵĽǶ�
	int *p = new int[ntl->size];//Ϊÿ���߶����ñ�־������Ѿ������������ߣ��Ͳ��ټ���
	double line_dist;//����ϲ����ߵĳ��ȣ���ֹ��������
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
			if (slope_t<0.05)//�غ������ǶȲ����2.86��
			{
				if (linetoline_dist(line1,line2)<5)//������֮��ľ���С��5����
				{
					//�����������߶˵����
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
**����˵����Ѱ��ģ�͵��ı���
****************************************************************************/
void find_quad(ntuple_list n_tuple_merge, cv::Mat temp,vector<pair<vector<Point2f>, model> >& pixels)
{
	model model_num=NO;//ģ��ö��
	lineinfo line1, line2;
	vector<lineinfo> lines1;//��������ƽ���߶�
	vector<Point2f> pixel;//����ÿ����ȡ���ı��ε��ĸ�������˳�����Ժ������ֱ��ƥ����Ӧ�� ��
	Point2f pt1,pt2;//���ڼ�������ģ���Ƿ����Ѿ�������
	//ģ���ظ���־
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
		
			//�˴�ƽ�еľ���û�кϲ�ʱ��ô�ϸ񣬴��ڹ��յ�Ӱ�쵼�²����ر�ƽ��
			if (atan(abs((line1.k - line2.k) / (1 + line1.k*line2.k))) < 0.087)//С��5��
			{
				//�������˴���ͼƬ�ߴ�Ӱ��ϴ������ƽ����ƥ�䣬������Ҳֻ��1���������Ĳ����ʱû�г���
				//��֪��ģ���ܹ���Ӱ��ߴ�᲻��ܴ�
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
**����˵����QT��������·����Ĵ���������֮ǰ��C++����������Ҫ����
**
**����˵����path������ͼƬ��·��
			resultshow:������Ҫ��ʾ���ı���Ϣ����results�ظ�����ʵ����ɾ��
			pts��ͼƬ����ʼ�����꣬Ϊ�����ͼ��������
			results��������Ҫ��QT����ʾ������
**************************************************************************/
void procfunc(string path,string showpath, string& resultshow, Point2f pts, vector<vector<string>>& results)
{
	resultshow = "λ����ʾ��\n\n";
	vector<string> result;
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

	vector<pair<vector<Point2f>, model>> pixels;
	find_quad(n_tuple_merge, src, pixels);
	free_ntuple_list(n_tuple_merge);

	//PNP
	/*************����������***********/
	//�ڲ���
	double fx = 1501.3345;
	double fy = 1500.9675;
	double u0 = 499.9127;
	double v0 = 508.6514;
	//��ͷ�������
	double k1 = -0.1746   ;
	double k2 = 0.1028;
	double p1 = 0;
	double p2 = 0;
	double k3 = 0.1379;

	vector<Point3f> Point3D;
	PNPSolver p4psolver;
	//��ʼ���������
	p4psolver.SetCameraMatrix(fx, fy, u0, v0);
	//���û������
	p4psolver.SetDistortionCoefficients(k1, k2, p1, p2, k3);
	
	Mat imageshow;
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
			resultshow += "SQUARE:\n\t";
			result.push_back("SQUARE:");
			p4psolver.Points3D.push_back(Point3f(5, -5, 0));
			p4psolver.Points3D.push_back(Point3f(5, 5, 0));
			p4psolver.Points3D.push_back(Point3f(-5, 5, 0));
			p4psolver.Points3D.push_back(Point3f(-5, -5, 0));
			break;
		case TRAPEZOID:
			resultshow += "TRAPEZOID:\n\t";
			result.push_back("TRAPEZOID:");
			p4psolver.Points3D.push_back(Point3f(5, -5, 0));
			p4psolver.Points3D.push_back(Point3f(11, 5, 0));
			p4psolver.Points3D.push_back(Point3f(-11, 5, 0));
			p4psolver.Points3D.push_back(Point3f(-5, -5, 0));
			break;
		case RECTANGLE:
			resultshow += "RECTANGLE:\n\t";
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
			resultshow += d2s(p4psolver.Position_OwInC.x);
			resultshow += "\t";
			resultshow += d2s(p4psolver.Position_OwInC.y);
			resultshow += "\t";
			resultshow += d2s(p4psolver.Position_OwInC.z);
			resultshow += "\n";
			
			result.push_back(d2s(p4psolver.Position_OwInC.x));
			result.push_back(d2s(p4psolver.Position_OwInC.y));
			result.push_back(d2s(p4psolver.Position_OwInC.z));
			result.push_back(d2s(p4psolver.Theta_C2W.x));
			result.push_back(d2s(p4psolver.Theta_C2W.y));
			result.push_back(d2s(p4psolver.Theta_C2W.z));
			//std::cout << "test2:CV_ITERATIVE����:	Ŀ��������λ�á�" << "���ƽ��=" << p4psolver.Position_OwInC << endl;
		}
		p4psolver.Points2D.clear();
		p4psolver.Points3D.clear();
		results.push_back(result);
		result.clear();
	}
	imwrite("..\\resultshow.bmp",imageshow);
}

/****************************************************************************************
**����˵�������ص�����
*****************************************************************************************/
void on_mouse(int event, int x, int y, int flags, void *ustc)//event����¼����ţ�x,y������꣬flags��ק�ͼ��̲����Ĵ���  
{
	pair<IplImage*, Point3f*> *temp2 = (pair<IplImage*, Point3f*>*)ustc;
	IplImage *srcImage = (*temp2).first;
	Point3f *pt = (*temp2).second;
	Mat org = Mat(srcImage);
	Mat img, tmp, dst;
	org.copyTo(img);
	static Point pre_pt = (-1, -1);//��ʼ����  
	static Point cur_pt = (-1, -1);//ʵʱ����  
	char temp[16];
	if (event == CV_EVENT_LBUTTONDOWN)//������£���ȡ��ʼ���꣬����ͼ���ϸõ㴦��Բ  
	{
		org.copyTo(img);//��ԭʼͼƬ���Ƶ�img��  
		sprintf(temp, "(%d,%d)", x, y);
		pre_pt = Point(x, y);
		putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//�ڴ�������ʾ����  
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//��Բ  
		imshow("img", img);
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//���û�а��µ����������ƶ��Ĵ�����  
	{
		img.copyTo(tmp);//��img���Ƶ���ʱͼ��tmp�ϣ�������ʾʵʱ����  
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//ֻ��ʵʱ��ʾ����ƶ�������  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//�������ʱ������ƶ�������ͼ���ϻ�����  
	{
		img.copyTo(tmp);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//����ʱͼ����ʵʱ��ʾ����϶�ʱ�γɵľ���  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_LBUTTONUP)//����ɿ�������ͼ���ϻ�����  
	{
		org.copyTo(img);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(img, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//���ݳ�ʼ��ͽ����㣬�����λ���img��  
		imshow("img", img);
		img.copyTo(tmp);
		//��ȡ���ΰ�Χ��ͼ�񣬲����浽dst��  
		int width = abs(pre_pt.x - cur_pt.x);
		int height = abs(pre_pt.y - cur_pt.y);

		dst = org(Rect(min(cur_pt.x, pre_pt.x), min(cur_pt.y, pre_pt.y), width, height));
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
	lineinfo line1, line2, line3, line4;//���������߶Σ��������ж����Ƿֱ�����ʲô��״
	model model_num=NO;
#pragma region ����������

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

	if ((atan(abs(line1.k-line2.k)/(1+line1.k*line2.k))<0.349)&&(atan(abs(line3.k-line4.k)/(1+line3.k*line4.k))<0.349))//20��
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

	//PNP
	/*************����������***********/
	//�ڲ���
	double fx = 1501.3345;
	double fy = 1500.9675;
	double u0 = 499.9127;
	double v0 = 508.6514;
	//��ͷ�������
	double k1 = -0.1746;
	double k2 = 0.1028;
	double p1 = 0;
	double p2 = 0;
	double k3 = 0.1379;
	
	
	PNPSolver p4psolver;
	//��ʼ���������
	p4psolver.SetCameraMatrix(fx, fy, u0, v0);
	//���û������
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
		result.push_back(d2s(p4psolver.Position_OwInC.x));
		result.push_back(d2s(p4psolver.Position_OwInC.y));
		result.push_back(d2s(p4psolver.Position_OwInC.z));
		result.push_back(d2s(p4psolver.Theta_C2W.x));
		result.push_back(d2s(p4psolver.Theta_C2W.y));
		result.push_back(d2s(p4psolver.Theta_C2W.z));
		//std::cout << "test2:CV_ITERATIVE����:	Ŀ��������λ�á�" << "���ƽ��=" << p4psolver.Position_OwInC << endl;
	}
	p4psolver.Points2D.clear();
	p4psolver.Points3D.clear();
	imwrite("..\\resultshow.bmp", imageshow);
	
}






