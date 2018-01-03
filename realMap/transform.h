#pragma once
#include "stdafx.h"
#include "camera.h"
using namespace cv;
using namespace std;

class Transform
{
public:
	//Camera m_hCamera;
	vector <Point2f> m_pointList;
	vector <Point2f> m_pointListTrans;
	vector<Mat> m_transformList;
	int m_key = 0;
	char * filename = "6transform.ini";

	void init(vector<Mat> & m_transformList);
	void saveTransform(char * fileName);
	void loadTransform();
	void warpMat(Mat & src, Mat & result, Mat transform, Mat mask);
	Mat drawCross(Mat & img, int distance = 100);
	static void on_mouse(int event, int x, int y, int flags, void* ustc);
private:

};


