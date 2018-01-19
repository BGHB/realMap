// realMap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "transform.h"
#include <omp.h>

using namespace std;
using namespace cv;

#if 1
void thread_task(string camPath, vector<list<Mat>> &camFrameList, int i) {
	VideoCapture cap;
	Mat img,temp;
	cap.open(camPath);
	while (true)
	{
		cap >> img;
		if (!img.empty())
		{
			camFrameList[i].push_back(img);
		}
		this_thread::sleep_for(chrono::milliseconds(1));
	}
}

int main() {
	Camera hCamera;
	Transform hTransform;
	//hCamera.openIPCam();
	vector<string> camPath;
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.119/cam/realmonitor?channel=1&subtype=0");
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.124/video1");
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.125/video1");
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.120/cam/realmonitor?channel=1&subtype=0");
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.121/cam/realmonitor?channel=1&subtype=0");
	camPath.push_back("rtsp://admin:Hk123456@192.168.10.122/sub/av_stream"); //海康 右下

	//hCamera.openVideo();

	hCamera.init();
	//hCamera.loadMask();
	for (int i = 0; i < 6; i++)
	{
		hCamera.threads[i] = thread(thread_task, camPath[i], ref(hCamera.m_camFrameList), i);
		hCamera.threads[i].detach();
	}

	int result_w = 1920;
	int result_h = 1080;
	Mat resultForShow;
	Mat result;
	result.create(result_h - 200, result_w - 160, CV_8UC3);

	hTransform.init(hTransform.m_transformList);
	//hTransform.saveTransform();
	//hTransform.loadTransform();

	double tstart = cvGetTickCount();
	//namedWindow("result", 1);
	while (true)
	{
		int flag = 0;
		for (int i = 0; i < hCamera.m_camNum; i++) {
			//cout << hCamera.m_camFrameList[i].size() << "  ";
			if (0 < hCamera.m_camFrameList[i].size()) {
				flag++;
			}
		}
		
		cout << endl;
		if (6 == flag) {
			if (result.isContinuous()) {
				memset(result.data, 0, result.rows*result.cols*result.channels());
			}
#pragma omp parallel for
			for (int i = 0; i < hCamera.m_camNum; i++)
			{
				hCamera.m_camCurrrentFrame[i] = hCamera.m_camFrameList[i].back().clone();
				hCamera.m_camFrameList[i].pop_front();
				hTransform.warpMat(hCamera.m_camCurrrentFrame[i], result, hTransform.m_transformList[i], hCamera.m_camMask[i]);
			}

			cout << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;
			tstart = cvGetTickCount();
			for (int k = 0; k < hTransform.m_pointList.size(); k++)
			{
				//circle(result, hTransform.m_pointList[k], 4, Scalar(0, 0, 255), -1);
				circle(result, hTransform.m_pointListTrans[k], 4, Scalar(0, 255 - k / 4 * 255 / 6, k / 4 * 255 / 6), -1);
			}
			result = hTransform.drawCross(result, 80);
			setMouseCallback("result", Transform::on_mouse, (void *)&hTransform);
			imshow("result", result);
			waitKey(1);
		}
		Sleep(3);
	}
	return 0;
}

#endif
