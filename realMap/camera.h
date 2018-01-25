#pragma once
#include "stdafx.h"
using namespace std;
using namespace cv;
class Camera
{
public:
	
	static const int        m_camNum = 6;
	VideoCapture            cap[6];

	vector<string>          m_camPath;
	vector <Point2f>        m_pointList;
	vector <Point2f>        m_pointListTrans;
	vector<Mat>             m_transformList;
	
	double                  m_imgScale = 1;
	vector<Mat>             m_camMask;
	vector<Mat>             m_camFrame;
	vector<Mat>             m_camCurrrentFrame;
	vector<list<Mat> >      m_camFrameList;
	Mat result, transMat;



	void                    init();
	
	int m_key = 0;
	char * filename = "6transform.ini";

	void inittt(vector<Mat> & m_transformList);
	void saveTransform(char * fileName);
	void warpMat(Mat & src, Mat & result, Mat transform, Mat mask);
	Mat drawCross(Mat & img, int distance = 100);
	static void on_mouse(int event, int x, int y, int flags, void* ustc);

private:

};

