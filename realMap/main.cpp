// realMap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "transform.h"
//#include "camera.h"
using namespace std;
using namespace cv;
Mat result;

int main() {
	Camera hCamera;
	Transform hTransform;
	hCamera.openIPCam();
	//hCamera.openVideo();
	hCamera.loadMask();
	vector<Mat> frameList(hCamera.m_camNum);	
	
	int result_w = 1920;
	int result_h = 1080;
	Mat resultForShow;
	Mat result;
	result.create(result_h - 120 , result_w-120, CV_8UC3);
	
	hTransform.init(hTransform.m_transformList);
	//hTransform.saveTransform();
	//hTransform.loadTransform();
	double tstart = cvGetTickCount();
	namedWindow("result", 1);
	while (true)
	{
		if (result.isContinuous())
		{
			memset(result.data, 0, result.rows*result.cols*result.channels());
		}
		for (int i = 0; i < hCamera.m_camNum; i++)
		{
			hCamera.cap[i] >> frameList[i];
			
			if (frameList[i].empty())
			{
				cout << i << " not connect!" << endl;
				continue;
			}
			//warpPerspective(frameList[0], result, hTransform.m_transform[0], result.size());
			hTransform.warpMat(frameList[i], result, hTransform.m_transformList[i], hCamera.m_maskList[i]);
		}
		cout << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;
		tstart = cvGetTickCount();
		for (int k = 0; k < hTransform.m_pointList.size(); k++)
		{
			//circle(result, hTransform.m_pointList[k], 4, Scalar(0, 0, 255), -1);
			circle(result, hTransform.m_pointListTrans[k], 4, Scalar(0, 255 - k/4*255 / 6, k / 4 * 255 / 6), -1);
		}
		result = hTransform.drawCross(result, 80);
		setMouseCallback("result", Transform::on_mouse, (void *)&hTransform);
		//resize(result, resultForShow, Size(result.cols / 1.5, result.rows / 1.5));
		imshow("result", result);
		waitKey(1);		
	}
	//result = drawCross(result);
	//imshow("result", result);
	//waitKey(0);		
}

