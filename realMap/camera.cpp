#include "stdafx.h"
#include "camera.h"
#include <io.h>
using namespace cv;
using namespace std;

void Camera::init()
{
	typedef list<Mat> LISTL;
	LISTL list;
	Mat mask;
	char maskPath[128];
	for (int i = 0; i < m_camNum; i++)
	{
		sprintf(maskPath, "%s%d%s", "mask//", i, "_mask.bmp");
		mask = imread(maskPath, 0);
		m_camMask.push_back(mask.clone());
		m_camFrame.push_back(mask.clone());
		m_camFrameList.push_back(list);
		m_camCurrrentFrame.push_back(mask.clone());
	}

	result.create(imgHeight - 200, imgWidth - 160, CV_8UC3);
	//距离，序号，列，行（dist, num, x, y）
	transMat.create(result.rows, result.cols, CV_64FC4);        

	//加载配置文件,相机/视频路径、转换前的点、转换后的点、准换矩阵
	FileStorage readFile("config.xml", FileStorage::READ);
	if (readFile.isOpened()) {
		readFile["camPath"] >> m_camPath;
		readFile["pointList"] >> m_pointList;
		readFile["transformList"] >> m_transformList;
		readFile.release();
		m_pointListTrans = getPointListTrans(m_pointList, m_transformList);
	}
	
	//打开摄像头并保存句柄和视频Size
	for (int i = 0; i < m_camPath.size(); i++)
	{
		VideoCapture cap;
		cap.open(m_camPath[i]);
		Point2i frameSize;
		if (cap.isOpened()) {
			cout << "camera " << i << " is opened!" << endl;
			frameSize.x = cap.get(CAP_PROP_FRAME_WIDTH);
			frameSize.y = cap.get(CAP_PROP_FRAME_HEIGHT);
			m_frameSizeList.push_back(frameSize);
			m_capList.push_back(cap);
		}else {
			cout << "cam " << i << " opened failed!" << endl;
		}
	}
}

void Camera::init_generate()
{
	typedef list<Mat> LISTL;
	LISTL list;
	Mat mask;
	char maskPath[128];
	//加载配置文件,相机/视频路径、转换前的点、转换后的点、准换矩阵
	FileStorage readFile("config.xml", FileStorage::READ);
	if (readFile.isOpened()){
		readFile["camPath"] >> m_camPath;
		readFile["pointList"] >> m_pointList;
		readFile["transformList"] >> m_transformList;
		readFile.release();
		m_pointListTrans = getPointListTrans(m_pointList, m_transformList);
	}
	else
	{
		FileStorage readFile("cameraPath.xml", FileStorage::READ);
		if (!readFile.isOpened()){
			cout << "config file load failed!" << endl;
			readFile.release();
			system("pause");
		}
		readFile["camPath"] >> m_camPath;
		readFile.release();
		Point2f point;
		for (int i = 0; i < m_camPath.size(); i++)
		{
			m_pointList.push_back(Point2f(480, 270));
			point.x = i * imgWidth / m_camPath.size() + m_pointList.back().x / m_camPath.size();
			point.y = i * imgHeight / m_camPath.size() + m_pointList.back().y / m_camPath.size();
			m_pointListTrans.push_back(point);

			m_pointList.push_back(Point2f(960, 270));
			point.x = i * imgWidth / m_camPath.size() + m_pointList.back().x / m_camPath.size();
			point.y = i * imgHeight / m_camPath.size() + m_pointList.back().y / m_camPath.size();
			m_pointListTrans.push_back(point);

			m_pointList.push_back(Point2f(480, 540));
			point.x = i * imgWidth / m_camPath.size() + m_pointList.back().x / m_camPath.size();
			point.y = i * imgHeight / m_camPath.size() + m_pointList.back().y / m_camPath.size();
			m_pointListTrans.push_back(point);

			m_pointList.push_back(Point2f(960, 540));
			point.x = i * imgWidth / m_camPath.size() + m_pointList.back().x / m_camPath.size();
			point.y = i * imgHeight / m_camPath.size() + m_pointList.back().y / m_camPath.size();
			m_pointListTrans.push_back(point);
		}
		m_transformList = getTransformList(m_pointList, m_pointListTrans);
		FileStorage writeFile("config.xml", FileStorage::WRITE);
		if (writeFile.isOpened()) {
			writeFile << "camPath"<< m_camPath;
			writeFile << "pointList" <<  m_pointList;
			writeFile << "transformList" << m_transformList;
			writeFile.release();
		}
	}
	//打开摄像头并保存句柄和视频Size
	for (int i = 0; i < m_camPath.size(); i++)
	{
		VideoCapture cap;
		cap.open(m_camPath[i]);
		Point2i frameSize;
		if (cap.isOpened()) {
			cout << "camera " << i << " is opened!" << endl;
			frameSize.x = cap.get(CAP_PROP_FRAME_WIDTH);
			frameSize.y = cap.get(CAP_PROP_FRAME_HEIGHT);
			m_frameSizeList.push_back(frameSize);
			m_capList.push_back(cap);
		}
		else{
			cout << "cam " << i << " opened failed!" << endl;
		}
	}
	
	for (int i = 0; i < m_camPath.size(); i++)
	{
		sprintf(maskPath, "%s%d%s", "mask//", i, "_mask.bmp");
		mask = imread(maskPath, 0);
		m_camMask.push_back(mask.clone());
		m_camFrame.push_back(mask.clone());
		m_camFrameList.push_back(list);
		m_camCurrrentFrame.push_back(mask.clone());
	}

	result.create(imgHeight - 200, imgWidth - 160, CV_8UC3);
	//距离，序号，列，行（dist, num, x, y）
	transMat.create(result.rows, result.cols, CV_64FC4);
}

vector<Point2f> Camera::getPointListTrans(vector<Point2f> pointList, vector<Mat> transformList) {
	vector<Point2f> pointListTrans;
	Point2f point;
	for (int i = 0; i < pointList.size(); i++)
	{
		double * px0 = transformList[i / 4].ptr<double>(0);
		double * px1 = transformList[i / 4].ptr<double>(1);
		double * px2 = transformList[i / 4].ptr<double>(2);
		point.x = (pointList[i].x * px0[0] + pointList[i].y * px0[1] + px0[2]) / (pointList[i].x * px2[0] + pointList[i].y * px2[1] + px2[2]);
		point.y = (pointList[i].x * px1[0] + pointList[i].y * px1[1] + px1[2]) / (pointList[i].x * px2[0] + pointList[i].y * px2[1] + px2[2]);
		pointListTrans.push_back(point);
	}
	return pointListTrans;
}

vector<Mat> Camera::getTransformList(vector <Point2f> pointList, vector <Point2f> pointListTrans)
{
	Mat transform;
	vector<Mat> transformList;
	transform.create(3, 3, CV_64FC1);
	vector<Point2f>::iterator iter = m_pointList.begin();
	vector<Point2f>::iterator iter1 = m_pointListTrans.begin();
	for (; iter != m_pointList.end(); iter += 4, iter1 += 4)
	{
		vector<Point2f> corners(4);
		corners[0] = *iter;     	    // left-top
		corners[1] = *(iter + 1); 	    // left-bottom
		corners[2] = *(iter + 2); 	    // right-top
		corners[3] = *(iter + 3); 	    // right-bottom
		vector<Point2f> corners_trans(4);
		corners_trans[0] = *iter1;
		corners_trans[1] = *(iter1 + 1);
		corners_trans[2] = *(iter1 + 2);
		corners_trans[3] = *(iter1 + 3);
		
		transform = getPerspectiveTransform(corners, corners_trans);
		transformList.push_back(transform.clone());
	}
	return transformList;
}

Mat Camera::getTransMat(vector<Mat> transformList)
{
	Mat temp;
	temp.create(transMat.rows, transMat.cols, CV_64FC4);
	int r = 1;
	int c = temp.rows * temp.cols * temp.channels();
	for (int j = 0; j < r; j++)
	{
		double *data = temp.ptr<double>(j);
		for (int i = 0; i < c; i++)
		{
			data[i] = 1440000.;
		}
	}
	
	for (int k = 0; k < transformList.size(); k++)
	{
		double * px0 = m_transformList[k].ptr<double>(0);
		double * px1 = m_transformList[k].ptr<double>(1);
		double * px2 = m_transformList[k].ptr<double>(2);
		int transRow, transCol;
		for (int j = 0; j < m_frameSizeList[k].y; j++)
		{
			for (int i = 0; i < m_frameSizeList[k].x; i++)
			{
				transCol = (i * px0[0] + j * px0[1] + px0[2]) / (i * px2[0] + j * px2[1] + px2[2]);
				transRow = (i * px1[0] + j * px1[1] + px1[2]) / (i * px2[0] + j * px2[1] + px2[2]);
				//cout << resultRow<<", "<< resultCol << endl;
				if (transRow >= 0 && transRow < temp.rows && transCol >= 0 && transCol < temp.cols)
				{
					double *data = temp.ptr<double>(transRow);
					int dist = (i - (m_frameSizeList[k].x >> 1))*(i - (m_frameSizeList[k].x >> 1)) + (j - (m_frameSizeList[k].y >> 1))*(j - (m_frameSizeList[k].y >> 1));
					if (dist < data[4 * transCol])
					{
						data[4 * transCol] = dist;
						data[4 * transCol + 1] = k;
						data[4 * transCol + 2] = j;
						data[4 * transCol + 3] = i;
					}
				}
			}
		}
	}
	return temp;
}



//自动初始化
void Camera::inittt(vector<Mat> & m_transformList)
{
	Mat transform;
	transform.create(3, 3, CV_64FC1);
	for (int i = 0; i < Camera::m_camNum; i++) {
		m_transformList.push_back(transform.clone());
		for (int i = 0; i < 4; i++)
		{
			m_pointList.push_back(Point2f(0, 0));
			m_pointListTrans.push_back(Point(0, 0));
		}
	}

	//判断transform文件是否存在
	if ((_access(filename, 0) == -1))  //文件不存在
	{
		m_pointList = { Point2f(385, 1003), Point2f(1576, 927), Point2f(851, 261), Point2f(225, 228), Point2f(315, 619), Point2f(1049, 64), Point2f(1191, 250), Point2f(441, 805),
			Point2f(617, 322), Point2f(947, 247), Point2f(226, 581), Point2f(557, 846), Point2f(480, 200), Point2f(888, 151), Point2f(133, 662), Point2f(357, 843),
			Point2f(1006, 130), Point2f(1029, 551), Point2f(163, 577), Point2f(162, 151), Point2f(1617, 872), Point2f(225, 542), Point2f(363, 232), Point2f(1322, 172) };
		for (int i = 0; i < m_pointList.size(); i++)
		{
			m_pointListTrans[i].x = i / 4 * imgWidth / Camera::m_camNum + m_pointList[i].x / Camera::m_camNum;
			m_pointListTrans[i].y = i / 4 * imgHeight / Camera::m_camNum + m_pointList[i].y / Camera::m_camNum;
		}

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
					writeFile << data[c] << " ";
				}
				writeFile << "\n";
			}
		}
		writeFile.close();
	}
}

void Camera::saveTransformList()
{
	vector<Point2f>::iterator iter = m_pointList.begin();
	vector<Point2f>::iterator iter1 = m_pointListTrans.begin();
	vector<Mat>::iterator iterM = m_transformList.begin();
	for (; iter != m_pointList.end();)
	{
		vector<Point2f> corners(4);
		corners[0] = *iter;     	    // left-top
		corners[1] = *(iter + 1); 	    // left-bottom
		corners[2] = *(iter + 2); 	    // right-top
		corners[3] = *(iter + 3); 	    // right-bottom
		vector<Point2f> corners_trans(4);
		corners_trans[0] = *iter1;
		corners_trans[1] = *(iter1 + 1);
		corners_trans[2] = *(iter1 + 2);
		corners_trans[3] = *(iter1 + 3);
		
		*iterM = getPerspectiveTransform(corners, corners_trans);
		iter += 4, iter1 += 4, iterM++;
	}
	transMat = getTransMat(m_transformList);
	FileStorage writeFile("config.xml", FileStorage::WRITE);
	writeFile << "camPath" << Camera::m_camPath;
	writeFile << "pointList" << m_pointList;
	writeFile << "transformList" << m_transformList;
	writeFile.release();
}


//鼠标操作
void Camera::on_mouse(int event, int x, int y, int flags, void* ustc)
{
	Camera *p_this = (Camera *)ustc;
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
		p_this->saveTransformList();
	}
}


//画纵横线
Mat Camera::drawCross(Mat & img, int distance) {
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
