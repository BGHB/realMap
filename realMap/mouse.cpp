#include "stdafx.h"
#include <opencv2/opencv.hpp>  
using namespace cv;
using namespace std;
//Mat src, dst;
//void on_mouse(int event, int x, int y, int flags, void* ustc)
//{
//	static Point pre_pt = { -1,-1 };
//	static Point cur_pt = { -1,-1 };
//	//CvFont font;
//	//cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);
//	char temp[16];
//
//	if (event == CV_EVENT_LBUTTONDOWN)
//	{
//		dst = src;
//		sprintf(temp, "(%d,%d)", x, y);
//		pre_pt = cvPoint(x, y);
//		//cvPutText(src, temp, pre_pt, &font, cvScalar(0, 0, 0, 255));
//		circle(src, pre_pt, 3, cvScalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
//		imshow("src", src);
//		src = dst;
//	}
//	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))
//	{
//		dst = src;
//		sprintf(temp, "(%d,%d)", x, y);
//		cur_pt = Point(x, y);
//		//cvPutText(src, temp, cur_pt, &font, cvScalar(0, 0, 0, 255));
//		imshow("src", src);
//	}
//	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
//	{
//		dst = src;
//		sprintf(temp, "(%d,%d)", x, y);
//		cur_pt = cvPoint(x, y);
//		//cvPutText(src, temp, cur_pt, &font, cvScalar(0, 0, 0, 255));
//		rectangle(src, pre_pt, cur_pt, cvScalar(0, 255, 0, 0), 1, 8, 0);
//		imshow("src", src);
//	}
//	else if (event == CV_EVENT_LBUTTONUP)
//	{
//		sprintf(temp, "(%d,%d)", x, y);
//		cur_pt = cvPoint(x, y);
//		//cvPutText(src, temp, cur_pt, &font, cvScalar(0, 0, 0, 255));
//		circle(src, cur_pt, 3, cvScalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
//		rectangle(src, pre_pt, cur_pt, cvScalar(0, 255, 0, 0), 1, 8, 0);
//		imshow("src", src);
//		src = dst;
//	}
//}
//int mmain()
//{
//	src = imread("37.jpg", 1);
//	dst = src.clone();
//	namedWindow("src", 1);
//	setMouseCallback("src", on_mouse, 0);
//
//	imshow("src", src);
//	waitKey(0);
//	destroyAllWindows();
//	return 0;
//}