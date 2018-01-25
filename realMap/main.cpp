// realMap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "camera.h"
#include <omp.h>

using namespace std;
using namespace cv;

//查表法20181025
//读取映射矩阵 去重拼接显示
#if 1
void thread_task(string camPath, list<Mat> &camFrameList, int i) {
	VideoCapture cap;
	Mat img;
	cap.open(camPath);
	if (!cap.isOpened())
		cout << "cam " << i << " opened failed!" << endl;
	while (true)
	{
		cap >> img;
		if (!img.empty())
		{
			camFrameList.push_back(img.clone());
		}
		this_thread::sleep_for(2ms);
	}
}


int main() {
	Camera hCamera;
	hCamera.init();

	thread threads;
	for (int i = 0; i < 6; i++)
	{
		threads = thread(thread_task, hCamera.m_camPath[i], ref(hCamera.m_camFrameList[i]), i);
		threads.detach();
	}
	
	cout << "初始化..." << endl;
	
	FileStorage readfile("transMatrix.xml", FileStorage::READ);
	readfile["transMatrix"] >> hCamera.transMat;
	readfile.release();

	double tstart = cvGetTickCount();
	//namedWindow("result", 1);
	while (true)
	{
		int flag = 0;
		for (int i = 0; i < hCamera.m_camNum; i++) {
			cout << hCamera.m_camFrameList[i].size() << "  ";
			if (30 < hCamera.m_camFrameList[i].size())
			{
				hCamera.m_camFrameList[i].clear();
			}
			if (0 < hCamera.m_camFrameList[i].size()) {
				flag++;
			}
		}	
		cout << endl;
		if (6 == flag) {
			for (int k = 0; k < hCamera.m_camNum; k++)
			{
				hCamera.m_camCurrrentFrame[k] = hCamera.m_camFrameList[k].back().clone();
				hCamera.m_camFrameList[k].pop_front();
			}
			

			if (hCamera.result.isContinuous()) {
				memset(hCamera.result.data, 0, hCamera.result.rows * hCamera.result.cols * hCamera.result.channels());
			}
			for (int j = 0; j < hCamera.result.rows; j++)
			{
				uchar *resultData = hCamera.result.ptr<uchar>(j);
				double *transMatData = hCamera.transMat.ptr<double>(j);
				for (int i = 0; i < hCamera.result.cols; i++)
				{
					
					if (transMatData[4*i] < 1200000.)
					{
						uchar *srcData = hCamera.m_camCurrrentFrame[transMatData[4 * i+1]].ptr<uchar>(transMatData[4 * i + 2]);				
						resultData[3 * i] = srcData[3 * (uint)transMatData[4 * i + 3]];
						resultData[3 * i+ 1] = srcData[3 * (uint)transMatData[4 * i + 3] + 1];
						resultData[3 * i +2] = srcData[3 * (uint)transMatData[4 * i + 3] + 2];
					}
					
				}
			}

			cout << "                       耗时：" << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;
			tstart = cvGetTickCount();
			//for (int k = 0; k < hCamera.m_pointList.size(); k++)
			//{
				//circle(result, hTransform.m_pointList[k], 4, Scalar(0, 0, 255), -1);
				//circle(result, hCamera.m_pointListTrans[k], 4, Scalar(0, 255 - k / 4 * 255 / 6, k / 4 * 255 / 6), -1);
			//}
			hCamera.result = hCamera.drawCross(hCamera.result, 80);
			//setMouseCallback("hCamera.result", Camera::on_mouse, (void *)&hCamera);
			imshow("result", hCamera.result);
			waitKey(1);
		}
		else
		{
			Sleep(3);
		}
		
	}
	return 0;
}

#endif


//通过config.xml transMartix.xml
#if 0
void thread_task(string camPath, list<Mat> &camFrameList, int i) {
	VideoCapture cap;
	Mat img;
	cap.open(camPath);
	if (!cap.isOpened())
		cout << "cam " << i << " opened failed!" << endl;
	while (true)
	{
		cap >> img;
		if (!img.empty())
		{
			camFrameList.push_back(img);
		}
		this_thread::sleep_for(2ms);
	}
}


int main() {
	Camera hCamera;
	hCamera.init();

	thread threads;
	for (int i = 0; i < 6; i++)
	{
		threads = thread(thread_task, hCamera.m_camPath[i], ref(hCamera.m_camFrameList[i]), i);
		threads.detach();
	}

	int result_w = 1920;
	int result_h = 1080;
	Mat resultForShow;
	Mat result;
	result.create(result_h - 200, result_w - 160, CV_8UC3);
	Mat transMat;
	//距离，序号，列，行（dist, num, x, y）
	transMat.create(result.rows, result.cols, CV_64FC4);

	double tstart = cvGetTickCount();
	cout << "                       耗时：" << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;
	tstart = cvGetTickCount();
	FileStorage readfile("transMatrix.xml", FileStorage::READ);
	readfile["transMatrix"] >> transMat;
	readfile.release();
	//namedWindow("result", 1);
	while (true)
	{
		int flag = 0;
		for (int i = 0; i < hCamera.m_camNum; i++) {
			cout << hCamera.m_camFrameList[i].size() << "  ";
			if (0 < hCamera.m_camFrameList[i].size()) {
				flag++;
			}
		}
		cout << endl;

		if (6 == flag) {
			for (int k = 0; k < hCamera.m_camNum; k++)
			{
				hCamera.m_camCurrrentFrame[k] = hCamera.m_camFrameList[k].back().clone();
				hCamera.m_camFrameList[k].pop_front();
			}
			//int r = transMat.rows;
			//int c = transMat.cols * transMat.channels();
			//if (transMat.isContinuous()) {
			//	r = 1;
			//	c = transMat.rows * transMat.cols * transMat.channels();
			//}
			//for (int j = 0; j < r; j++)
			//{
			//	double *data = transMat.ptr<double>(j);
			//	for (int i = 0; i < c; i++)
			//	{
			//		data[i] = 1440000.;
			//	}
			//}
			//
			//
			//for (int k = 0; k < hCamera.m_camNum; k++)
			//{
			//	hCamera.m_camCurrrentFrame[k] = hCamera.m_camFrameList[k].back().clone();
			//	hCamera.m_camFrameList[k].pop_front();
			//	
			//	//hCamera.warpMat(hCamera.m_camCurrrentFrame[i], result, hCamera.m_transformList[i], hCamera.m_camMask[i]);
			//	//void Camera::warpMat(Mat & src, Mat & result, Mat transform, Mat mask)
			//	double * px0 = hCamera.m_transformList[k].ptr<double>(0);
			//	double * px1 = hCamera.m_transformList[k].ptr<double>(1);
			//	double * px2 = hCamera.m_transformList[k].ptr<double>(2);
			//	//cout << transform << endl;
			//	int transRow, transCol;
			//	for (int j = 0; j < hCamera.m_camCurrrentFrame[k].rows; j++)
			//	{
			//		uchar * srcData = hCamera.m_camCurrrentFrame[k].ptr<uchar>(j);
			//		for (int i = 0; i < hCamera.m_camCurrrentFrame[k].cols; i++)
			//		{
			//			transCol = (i * px0[0] + j * px0[1] + px0[2]) / (i * px2[0] + j * px2[1] + px2[2]);
			//			transRow = (i * px1[0] + j * px1[1] + px1[2]) / (i * px2[0] + j * px2[1] + px2[2]);
			//			//cout << resultRow<<", "<< resultCol << endl;
			//			if (transRow >= 0 && transRow < transMat.rows && transCol >= 0 && transCol < transMat.cols)
			//			{
			//				double *transMatData = transMat.ptr<double>(transRow);
			//				
			//				int dist = (i - hCamera.m_camCurrrentFrame[k].cols / 2)*(i - hCamera.m_camCurrrentFrame[k].cols / 2)
			//					+ (j - hCamera.m_camCurrrentFrame[k].rows / 2)*(j - hCamera.m_camCurrrentFrame[k].rows / 2);
			//				if (dist < transMatData[4 * transCol])
			//				{
			//					transMatData[4 * transCol] = dist;
			//					transMatData[4 * transCol + 1] = k;
			//					transMatData[4 * transCol + 2] = j;
			//					transMatData[4 * transCol + 3] = i;
			//					//cout << transMatData[4 * transCol]<< "   "<<transMatData[4 * transCol+1] <<"   "
			//					//	<<transMatData[4 * transCol+2] <<"   "<< transMatData[4 * transCol+3]  << endl;
			//				}
			//			}
			//		}
			//	}
			//}

			//FileStorage writefile("transMatrix.xml", FileStorage::WRITE);
			//writefile << "transMatrix" << transMat;
			//writefile.release();

			if (result.isContinuous()) {
				memset(result.data, 0, result.rows*result.cols*result.channels());
			}
			for (int j = 0; j < result.rows; j++)
			{
				uchar *resultData = result.ptr<uchar>(j);
				double *transMatData = transMat.ptr<double>(j);
				for (int i = 0; i < result.cols; i++)
				{

					if (transMatData[4 * i] < 1200000.)
					{

						uchar *srcData = hCamera.m_camCurrrentFrame[transMatData[4 * i + 1]].ptr<uchar>(transMatData[4 * i + 2]);
						if (j == 336 && i > 1237)
						{
							cout << transMatData[4 * i + 3] << endl;
							cout << 3 * (uint)transMatData[4 * i + 3] << endl;
							cout << (int)srcData[3 * (uint)transMatData[4 * i + 3]] << endl;
						}

						resultData[3 * i] = srcData[3 * (uint)transMatData[4 * i + 3]];
						resultData[3 * i + 1] = srcData[3 * (uint)transMatData[4 * i + 3] + 1];
						resultData[3 * i + 2] = srcData[3 * (uint)transMatData[4 * i + 3] + 2];
					}

				}
			}

			cout << "                       耗时：" << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;
			tstart = cvGetTickCount();
			//for (int k = 0; k < hCamera.m_pointList.size(); k++)
			//{
			//circle(result, hTransform.m_pointList[k], 4, Scalar(0, 0, 255), -1);
			//circle(result, hCamera.m_pointListTrans[k], 4, Scalar(0, 255 - k / 4 * 255 / 6, k / 4 * 255 / 6), -1);
			//}
			result = hCamera.drawCross(result, 80);
			//setMouseCallback("result", Camera::on_mouse, (void *)&hCamera);
			imshow("result", result);
			waitKey(1);
		}
		else
		{
			Sleep(3);
		}

	}
	return 0;
}
}

#endif

//config版本，备份，20180123
#if 0
void thread_task(string camPath, list<Mat> &camFrameList, int i) {
	VideoCapture cap;
	Mat img;
	cap.open(camPath);
	if (!cap.isOpened())
		cout << "cam " << i << " opened failed!" << endl;
	while (true)
	{
		cap >> img;
		if (!img.empty())
		{
			camFrameList.push_back(img.clone());
		}
		this_thread::sleep_for(2ms);
	}
}

int main() {
	Camera hCamera;
	hCamera.init();

	thread threads;
	for (int i = 0; i < 6; i++)
	{
		threads = thread(thread_task, hCamera.m_camPath[i], ref(hCamera.m_camFrameList[i]), i);
		threads.detach();
	}

	int result_w = 1920;
	int result_h = 1080;
	Mat resultForShow;
	Mat result;
	result.create(result_h - 200, result_w - 160, CV_8UC3);

	double tstart = cvGetTickCount();
	cout << "                       耗时：" << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;
	tstart = cvGetTickCount();
	//namedWindow("result", 1);
	while (true)
	{
		int flag = 0;
		for (int i = 0; i < hCamera.m_camNum; i++) {
			cout << hCamera.m_camFrameList[i].size() << "  ";
			if (0 < hCamera.m_camFrameList[i].size()) {
				flag++;
			}
		}
		cout << endl;

		if (6 == flag) {
			if (result.isContinuous()) {
				memset(result.data, 0, result.rows*result.cols*result.channels());
			}
			for (int i = 0; i < hCamera.m_camNum; i++)
			{
				hCamera.m_camCurrrentFrame[i] = hCamera.m_camFrameList[i].back().clone();
				hCamera.m_camFrameList[i].pop_front();
				hCamera.warpMat(hCamera.m_camCurrrentFrame[i], result, hCamera.m_transformList[i], hCamera.m_camMask[i]);
			}
			//
			cout << "                       耗时：" << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;
			tstart = cvGetTickCount();
			for (int k = 0; k < hCamera.m_pointList.size(); k++)
			{
				//circle(result, hTransform.m_pointList[k], 4, Scalar(0, 0, 255), -1);
				circle(result, hCamera.m_pointListTrans[k], 4, Scalar(0, 255 - k / 4 * 255 / 6, k / 4 * 255 / 6), -1);
			}
			result = hCamera.drawCross(result, 80);
			setMouseCallback("result", Camera::on_mouse, (void *)&hCamera);
			imshow("result", result);
			waitKey(1);
		}
		else
		{
			Sleep(3);
		}
	}
	return 0;
}

#endif

//write config file
#if 0
#include <windows.h>
int main() {
	vector<string>			m_camPath;
	vector <Point2f>        m_pointList;
	vector <Point2f>        m_pointListTrans;
	vector<Mat>             m_transformList;
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.119/cam/realmonitor?channel=1&subtype=0");
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.124/video1");
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.125/video1");
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.120/cam/realmonitor?channel=1&subtype=0");
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.121/cam/realmonitor?channel=1&subtype=0");
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.122/sub/av_stream");

	FileStorage readfile("confignew.xml", FileStorage::READ);
	readfile["pointList"] >> m_pointList;
	readfile["pointListTrans"] >> m_pointListTrans;
	readfile["transformList"] >> m_transformList;
	readfile.release();

	FileStorage writefile("config.xml", FileStorage::WRITE);
	writefile << "camPath" << camPath;
	writefile << "pointList" << m_pointList;
	writefile << "pointListTrans" << m_pointListTrans;
	writefile << "transformList" << m_transformList;
	writefile.release();
	cout << "complete" << endl;
	system("pause");
	return 0;
}
#endif // 1
