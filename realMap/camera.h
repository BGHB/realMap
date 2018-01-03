#pragma once
#include "stdafx.h"
using namespace std;
using namespace cv;
class Camera
{
public:
	
	static const int        m_camNum = 6;
	VideoCapture            cap[6];
	vector<Mat>             m_maskList;
	

	void                    openIPCam();   //打开ip摄像头
	void					openVideo();   //打开本地视频
	void	 			    loadMask();    //载入mask

private:

};

