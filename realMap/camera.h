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
	vector<Point2f>         m_pointList;
	vector<Point2f>         m_pointListTrans;
	vector<Mat>             m_transformList;
	
	vector<VideoCapture>    m_capList;
	vector<Point2i>         m_frameSizeList;

 	const static int        imgHeight = 1080;
	const static int        imgWidth = 1920;
	double                  m_imgScale = 1;
	
	int m_key = 0;
	char* filename = "sdf";

	vector<Mat>             m_camMask;
	vector<Mat>             m_camFrame;
	vector<Mat>             m_camCurrrentFrame;
	vector<list<Mat> >      m_camFrameList;
	Mat result, transMat;



	void                    init();
	void                    init_generate();
	vector<Point2f>         getPointListTrans(vector<Point2f> pointList, vector <Mat> transformList);
	vector<Mat>             getTransformList(vector <Point2f> pointList, vector <Point2f> pointListTrans);
	Mat 			        getTransMat(vector<Mat> transformList);
	void                    saveTransformList();
	
	
	void inittt(vector<Mat> & m_transformList);
	
	void warpMat(Mat & src, Mat & result, Mat transform, Mat mask);
	Mat drawCross(Mat & img, int distance = 100);
	static void on_mouse(int event, int x, int y, int flags, void* ustc);

private:

};

