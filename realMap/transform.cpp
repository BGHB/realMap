#include "stdafx.h"
#include "transform.h"
#include  <io.h>

#define FRAMEHEIGHT 1080
#define FRAMEWIDTH 1920

//自动初始化
void Transform::init(vector<Mat> & m_transformList)
{
	Mat transform;
	transform.create(3, 3, CV_64FC1);
	for (int i = 0; i < Camera::m_camNum; i++) {
		m_transformList.push_back(transform.clone());
		for (int i = 0;i < 4; i++)
		{
			m_pointList.push_back(Point2f(0, 0));
			m_pointListTrans.push_back(Point(0, 0));
		}
	}
		
	//判断transform文件是否存在
	if ((_access(filename, 0) == -1))  //文件不存在
	{
		m_pointList = {Point2f(385, 1003), Point2f(1576, 927), Point2f(851, 261), Point2f(225, 228), Point2f(315, 619), Point2f(1049, 64), Point2f(1191, 250), Point2f(441, 805), 
			Point2f(617, 322), Point2f(947, 247), Point2f(226, 581), Point2f(557, 846), Point2f(480, 200), Point2f(888, 151), Point2f(133, 662), Point2f(357, 843), 
			Point2f(1006, 130), Point2f(1029, 551), Point2f(163, 577), Point2f(162, 151), Point2f(1617, 872), Point2f(225, 542), Point2f(363, 232), Point2f(1322, 172)};
		for (int i = 0; i < m_pointList.size(); i++)
		{
			m_pointListTrans[i].x = i / 4 * FRAMEWIDTH / Camera::m_camNum + m_pointList[i].x / Camera::m_camNum;
			m_pointListTrans[i].y = i / 4 * FRAMEHEIGHT / Camera::m_camNum + m_pointList[i].y / Camera::m_camNum;
		}
		
		for (int i = 0; i < Camera::m_camNum; i++)
		{
			//m_pointList[i * 4] = Point2f(FRAMEWIDTH / 4, FRAMEHEIGHT / 4);  // left-top
			//m_pointList[i * 4 + 1] = Point2f(FRAMEWIDTH / 4, FRAMEHEIGHT / 2);  // left-bottom
			//m_pointList[i * 4 + 2] = Point2f(FRAMEWIDTH / 2, FRAMEHEIGHT / 4);  // right-top
			//m_pointList[i * 4 + 3] = Point2f(FRAMEWIDTH / 2, FRAMEHEIGHT / 2);  // right-bottom
			//m_pointListTrans[i * 4] = Point2f((i * FRAMEWIDTH + FRAMEWIDTH / 4) / Camera::m_camNum, (i * FRAMEHEIGHT + FRAMEHEIGHT / 4) / Camera::m_camNum);
			//m_pointListTrans[i * 4 + 1] = Point2f((i * FRAMEWIDTH + FRAMEWIDTH / 4) / Camera::m_camNum, (i * FRAMEHEIGHT + FRAMEHEIGHT / 2) / Camera::m_camNum);
			//m_pointListTrans[i * 4 + 2] = Point2f((i * FRAMEWIDTH + FRAMEWIDTH / 2) / Camera::m_camNum, (i * FRAMEHEIGHT + FRAMEHEIGHT / 4) / Camera::m_camNum);
			//m_pointListTrans[i * 4 + 3] = Point2f((i * FRAMEWIDTH + FRAMEWIDTH / 2) / Camera::m_camNum, (i * FRAMEHEIGHT + FRAMEHEIGHT / 2) / Camera::m_camNum);

			vector<Point2f> corners(4);
			corners[0] = m_pointList[i * 4];     	    // left-top
			corners[1] = m_pointList[i * 4 + 1]; 	    // left-bottom
			corners[2] = m_pointList[i * 4 + 2]; 	    // right-top
			corners[3] = m_pointList[i * 4 + 3]; 	    // right-bottom
			vector<Point2f> corners_trans(4);
			corners_trans[0] = m_pointListTrans[i * 4];
			corners_trans[1] = m_pointListTrans[i * 4 + 1];
			corners_trans[2] = m_pointListTrans[i * 4 + 2];
			corners_trans[3] = m_pointListTrans[i * 4 + 3];

			m_transformList[i] = getPerspectiveTransform(corners, corners_trans);
		}

		ofstream writeFile(filename);
		for (int i = 0; i < Camera::m_camNum * 4; i++)
		{
			writeFile << m_pointList[i].x << " ";
			writeFile << m_pointList[i].y << " ";
		}
		writeFile << "\n";
		for (int i = 0; i < Camera::m_camNum * 4; i++)
		{
			writeFile << m_pointListTrans[i].x << " ";
			writeFile << m_pointListTrans[i].y << " ";
		}
		writeFile << "\n";
		
		for (int i = 0; i < Camera::m_camNum; i++) 
		{
			for (int r = 0; r < m_transformList[0].rows; r++)
			{
				double * data = m_transformList[i].ptr<double>(r);
				for (int c = 0; c < m_transformList[0].cols; c++)
				{
					writeFile << data[c]<<" ";
				}
				writeFile << "\n";
			}
		}
		writeFile.close();
	}
	loadTransform();	
}
void Transform::loadTransform()
{
	ifstream readFile;
	readFile.open(filename);  //读
	for (int i = 0; i < Camera::m_camNum * 4; i++)
	{
		readFile >> m_pointList[i].x;
		readFile >> m_pointList[i].y;
	}
	for (int i = 0; i < Camera::m_camNum * 4; i++)
	{
		readFile >> m_pointListTrans[i].x;
		readFile >> m_pointListTrans[i].y;
	}

	for (int i = 0; i < Camera::m_camNum; i++)
	{
		for (int r = 0; r < m_transformList[0].rows; r++)
		{
			double * data = m_transformList[i].ptr<double>(r);
			for (int c = 0; c < m_transformList[0].cols; c++)
			{
				readFile >> data[c];
			}
		}
		//cout << m_transform[i] << endl;
	}
	readFile.close();
}

void Transform::saveTransform(char * fileName)
{
	ofstream writeFile(filename);
	for (int i = 0; i < Camera::m_camNum * 4; i++)
	{
		writeFile << m_pointList[i].x << " ";
		writeFile << m_pointList[i].y << " ";
	}
	writeFile << "\n";
	for (int i = 0; i < Camera::m_camNum * 4; i++)
	{
		writeFile << m_pointListTrans[i].x << " ";
		writeFile << m_pointListTrans[i].y << " ";
	}
	writeFile << "\n";
	for (int i = 0; i < Camera::m_camNum; i++)
	{
		vector<Point2f> corners(4);
		corners[0] = m_pointList[i * 4];     	    // left-top
		corners[1] = m_pointList[i * 4 + 1]; 	    // left-bottom
		corners[2] = m_pointList[i * 4 + 2]; 	    // right-top
		corners[3] = m_pointList[i * 4 + 3]; 	    // right-bottom
		vector<Point2f> corners_trans(4);
		corners_trans[0] = m_pointListTrans[i * 4];
		corners_trans[1] = m_pointListTrans[i * 4 + 1];
		corners_trans[2] = m_pointListTrans[i * 4 + 2];
		corners_trans[3] = m_pointListTrans[i * 4 + 3];

		m_transformList[i] = getPerspectiveTransform(corners, corners_trans);
		cout << m_transformList[i] << endl;
		for (int r = 0; r < m_transformList[0].rows; r++)
		{
			double * data = m_transformList[i].ptr<double>(r);
			for (int c = 0; c < m_transformList[0].cols; c++)
			{
				writeFile << data[c]<<" ";
			}
			writeFile <<"\n";
		}
	}
	writeFile.close();
}


void Transform::warpMat(Mat & src, Mat & result, Mat transform, Mat mask)
{
	double * px0 = transform.ptr<double>(0);
	double * px1 = transform.ptr<double>(1);
	double * px2 = transform.ptr<double>(2);
	//cout << transform << endl;
	int resultRow, resultCol;
	for (int j = 0; j < src.rows; j++)
	{
		uchar * srcData = src.ptr<uchar>(j);
		uchar * maskData = mask.ptr<uchar>(j);
		for (int i = 0; i < src.cols; i++)
		{
			if (!maskData[i])
				continue;
			
			resultCol = (i * px0[0] + j * px0[1] + px0[2]) / (i * px2[0] + j * px2[1] + px2[2]);
			resultRow = (i * px1[0] + j * px1[1] + px1[2]) / (i * px2[0] + j * px2[1] + px2[2]);
			//cout << resultRow<<", "<< resultCol << endl;
			if (resultRow >= 0 && resultRow < result.rows && resultCol >= 0 && resultCol < result.cols)
			{
				uchar *resultData = result.ptr<uchar>(resultRow);
				if (resultData[3 * resultCol])
				{
					continue;
				}
				resultData[3 * resultCol] = srcData[3 * i];
				resultData[3 * resultCol +  1] = srcData[3 * i + 1];
				resultData[3 * resultCol + 2] = srcData[3 * i + 2];
			}
			i += 4;
		}
		j += 4;
	}
}

//鼠标操作
void Transform::on_mouse(int event, int x, int y, int flags, void* ustc)
{
	Transform *p_this = (Transform *)ustc;
	static Point pre_pt = { -1,-1 };
	static Point cur_pt = { -1,-1 };

	if (event == CV_EVENT_LBUTTONDOWN)
	{
		pre_pt = cvPoint(x, y);
		int dist = INT_MAX;
		for (int i = 0; i < p_this->m_pointListTrans.size(); i++)
		{
			int temp = (pre_pt.x - p_this->m_pointListTrans[i].x) * (pre_pt.x - p_this->m_pointListTrans[i].x) +
				(pre_pt.y - p_this->m_pointListTrans[i].y) * (pre_pt.y - p_this->m_pointListTrans[i].y);
			if (temp < dist)
			{
				p_this->m_key = i;
				dist = temp;
			}
		}
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		cur_pt = cvPoint(x, y);
		p_this->m_pointListTrans[p_this->m_key].x = cur_pt.x;
		p_this->m_pointListTrans[p_this->m_key].y = cur_pt.y;
		p_this->saveTransform(p_this->filename);
	}
}



//画纵横线
Mat Transform::drawCross(Mat & img, int distance) {
	for (int j = 0; j < img.rows; j++)
	{
		if (j % distance == (distance - 1))
		{
			uchar *data = img.ptr<uchar>(j);
			for (int i = 0; i < img.cols; i++)
			{
				data[3 * i + 2] = 255;
			}
		}
		else
		{
			for (int i = 0; i < img.cols; i++)
			{
				if (i % distance == (distance - 1))
				{
					uchar *data = img.ptr<uchar>(j);
					data[3 * i + 2] = 255;
				}
			}
		}
	}
	return img;
}

////鼠标操作
//void Transform::on_mouse(int event, int x, int y, int flags, void* ustc)
//{
//	static Point pre_pt = { -1,-1 };
//	static Point cur_pt = { -1,-1 };
//	//CvFont font;
//	//cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);
//	if (event == CV_EVENT_LBUTTONDOWN)
//	{
//		//dst = src;
//		//sprintf(temp, "(%d,%d)", x, y);
//		pre_pt = cvPoint(x, y);
//		//cvPutText(src, temp, pre_pt, &font, cvScalar(0, 0, 0, 255));
//		//circle(result, pre_pt, 3, cvScalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
//		//circle(result, pre_pt, 3, Scalar(0, 0, 255), -1);
//
//		imshow("result", result);
//		//src = dst;
//	}
//
//	else if (event == CV_EVENT_LBUTTONUP)
//	{
//		//sprintf(temp, "(%d,%d)", x, y);
//		cur_pt = cvPoint(x, y);
//		//cvPutText(src, temp, cur_pt, &font, cvScalar(0, 0, 0, 255));
//		//circle(src, cur_pt, 3, cvScalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
//		circle(result, cur_pt, 3, Scalar(0, 255, 255), -1);
//		//rectangle(src, pre_pt, cur_pt, cvScalar(0, 255, 0, 0), 1, 8, 0);
//		imshow("result", result);
//		//src = dst;
//	}
//}

