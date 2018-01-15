#pragma once
#include "stdafx.h"
using namespace std;
using namespace cv;
class Camera
{
public:
	
	static const int        m_camNum = 6;
	VideoCapture            cap[6];
	
	thread                  threads[m_camNum];
	double                  m_imgScale = 1;
	vector<Mat>             m_camMask;
	vector<Mat>             m_camFrame;
	vector<Mat>             m_camCurrrentFrame;
	//typedef list<Mat> LISTL;
	//LISTL list0, list1, list2, list3;
	vector<list<Mat> >      m_camFrameList;


	void                    init();
	//void                    thread_task(int i);
	void                    openIPCam();   //打开ip摄像头
	void					openVideo();   //打开本地视频
	void	 			    loadMask();    //载入mask

private:

};

