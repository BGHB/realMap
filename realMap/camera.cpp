#include "stdafx.h"
#include "camera.h"
using namespace cv;
using namespace std;

void Camera::openIPCam() {
	int i = 0;
	cap[i++].open("rtsp://admin:Hk123456@192.168.10.119/cam/realmonitor?channel=1&subtype=0"); 
	cap[i++].open("rtsp://admin:Hk123456@192.168.10.124/video1");
	cap[i++].open("rtsp://admin:Hk123456@192.168.10.125/video1");
	//i++;
	cap[i++].open("rtsp://admin:Hk123456@192.168.10.120/cam/realmonitor?channel=1&subtype=0");
	
	cap[i++].open("rtsp://admin:Hk123456@192.168.10.121/cam/realmonitor?channel=1&subtype=0");
	cap[i++].open("rtsp://admin:Hk123456@192.168.10.122/sub/av_stream"); //海康 右下
    //cap[i++].open("rtsp://admin:Hk123456@192.168.10.123/video1");   //楼梯   暂时不用
	//cap[i++].open("rtsp://admin:Hk123456@192.168.10.126/video1");   // 三楼楼梯
}

void Camera::openVideo() {
	char Videopath[m_camNum][32];
	for (int i = 0; i < m_camNum; i++)
	{
		sprintf(Videopath[i], "%s%d%s", "video//", i, ".mp4");
		cap[i].open(Videopath[i]);
	}
}

void Camera::loadMask()
{
	Mat mask;
	char maskPath[m_camNum][32];
	for (int i = 0; i < m_camNum; i++)
	{
		sprintf(maskPath[i], "%s%d%s", "mask//", i, "_mask.bmp");
		mask = imread(maskPath[i],0);
		m_maskList.push_back(mask.clone());
	}
}

