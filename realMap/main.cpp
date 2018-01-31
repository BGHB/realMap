// realMap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "camera.h"
#include <omp.h>

using namespace std;
using namespace cv;

//读取配置文件，计算映射矩阵，通过查表法计算拼接结果
#if 1
void thread_task(VideoCapture &cap, list<Mat> &camFrameList) {
	Mat img;
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
	list<Mat> saveList;
	VideoWriter write("C:\\Users\\handewei\\Desktop\\Vid1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(1760, 880));

	thread threads;
	for (int i = 0; i < hCamera.m_camPath.size(); i++)
	{
		threads = thread(thread_task, ref(hCamera.m_capList[i]), ref(hCamera.m_camFrameList[i]));
		threads.detach();
	}	
	double tstart = cvGetTickCount();
	hCamera.transMat = hCamera.getTransMat(hCamera.m_transformList);
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
		if (hCamera.m_camPath.size() == flag) {
			tstart = cvGetTickCount();
			for (int k = 0; k < hCamera.m_camNum; k++)
			{
				hCamera.m_camCurrrentFrame[k] = hCamera.m_camFrameList[k].back();
				hCamera.m_camFrameList[k].pop_front();
			}
			
#pragma omp parallel for
			for (int j = 0; j < hCamera.result.rows; j++)
			{
				uchar *resultData = hCamera.result.ptr<uchar>(j);
				int *transMatData = hCamera.transMat.ptr<int>(j);
				
				for (int i = 0; i < hCamera.result.cols; i++)
				{					
					if (transMatData[4*i] < 1310720)
					{
						uchar *srcData = hCamera.m_camCurrrentFrame[transMatData[4 * i+1]].ptr<uchar>(transMatData[4 * i + 2]);	
						uint index = 3 * (uint)transMatData[4 * i + 3];
						resultData[3 * i] = srcData[index];
						resultData[3 * i+ 1] = srcData[index + 1];
						resultData[3 * i +2] = srcData[index + 2];
					}					
				}
			}
			//hCamera.result = hCamera.drawCross(hCamera.result, 80);
			imshow("result", hCamera.result);
			saveList.push_back(hCamera.result.clone());
			if ('q' == waitKey(1))
			{
				break;
			}
			cout << "                               耗时：" << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;	
		}
		else
		{
			Sleep(3);
		}
	}
	while (!saveList.empty())
	{

		write << saveList.front();
		saveList.pop_front();

	}
	write.release();
	return 0;
}

#endif


//生成config.xml
#if 0
void thread_task(VideoCapture &cap, list<Mat> &camFrameList) {
	Mat img;
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
	hCamera.init_generate();
	
	thread threads;
	for (int i = 0; i < hCamera.m_camPath.size(); i++)
	{
		threads = thread(thread_task, ref(hCamera.m_capList[i]), ref(hCamera.m_camFrameList[i]));
		threads.detach();
	}

	double tstart = cvGetTickCount();
	hCamera.transMat = hCamera.getTransMat(hCamera.m_transformList);
	//namedWindow("result", 1);
	while (true)
	{
		int flag = 0;
		for (int i = 0; i < hCamera.m_camNum; i++) {
			cout << hCamera.m_camFrameList[i].size() << "  ";
			//if (30 < hCamera.m_camFrameList[i].size()){
			//	hCamera.m_camFrameList[i].clear();
			//}
			if (0 < hCamera.m_camFrameList[i].size()) {
				flag++;
			}
		}
		cout << endl;

		if (6 == flag) {
			for (int k = 0; k < hCamera.m_camNum; k++)
			{
				hCamera.m_camCurrrentFrame[k] = hCamera.m_camFrameList[k].back().clone();
				//hCamera.m_camFrameList[k].pop_front();	
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
					if (transMatData[4 * i] < 1200000.)
					{
						uchar *srcData = hCamera.m_camCurrrentFrame[transMatData[4 * i + 1]].ptr<uchar>(transMatData[4 * i + 2]);
						//uchar *srcData = hCamera.m_camCurrrentFrame[0].ptr<uchar>(transMatData[4 * i + 2]);

						resultData[3 * i] = srcData[3 * (uint)transMatData[4 * i + 3]];
						resultData[3 * i + 1] = srcData[3 * (uint)transMatData[4 * i + 3] + 1];
						resultData[3 * i + 2] = srcData[3 * (uint)transMatData[4 * i + 3] + 2];
					}
				}
				//cout <<"("<< j << endl;
			}

			cout << "                       耗时：" << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;
			tstart = cvGetTickCount();
			for (int k = 0; k < hCamera.m_pointList.size(); k++)
			{
				//circle(result, hTransform.m_pointList[k], 4, Scalar(0, 0, 255), -1);
				circle(hCamera.result, hCamera.m_pointListTrans[k], 4, Scalar(0, 255 - k / 4 * 255 / 6, k / 4 * 255 / 6), -1);
			}
			hCamera.result = hCamera.drawCross(hCamera.result, 80);
			setMouseCallback("result", Camera::on_mouse, (void *)&hCamera);
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
