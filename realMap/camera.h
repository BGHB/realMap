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
	

	void                    openIPCam();   //��ip����ͷ
	void					openVideo();   //�򿪱�����Ƶ
	void	 			    loadMask();    //����mask

private:

};

