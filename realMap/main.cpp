// realMap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "camera.h"
#include <omp.h>

using namespace std;
using namespace cv;

//读取配置文件，计算映射矩阵，通过查表法计算拼接结果
#if 0
void thread_task(VideoCapture &cap, list<Mat> &camFrameList) {
	Mat img, gray, result;
	while (true)
	{
		cap >> img;
		if (!img.empty())
		{
			camFrameList.push_back(img.clone());
		}
		this_thread::sleep_for(2ms);
	}
}

int main() {
	Camera hCamera;
	hCamera.init();
	list<Mat> saveList;
	//VideoWriter write("C:\\Users\\handewei\\Desktop\\Vid1we.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(1760, 880));

	thread threads;
	for (int i = 0; i < hCamera.m_camPath.size(); i++)
	{
		threads = thread(thread_task, ref(hCamera.m_capList[i]), ref(hCamera.m_camFrameList[i]));
		threads.detach();
	}	
	double tstart = cvGetTickCount();
	hCamera.transMat = hCamera.getTransMat(hCamera.m_transformList);
	//滤波
	for (int j = 0; j < hCamera.transMat.rows; j++)
	{
		int *transMatData = hCamera.transMat.ptr<int>(j);
		for (int i =0; i < hCamera.transMat.cols - 2; i++)
		{
			//根据相邻像素把中间像素补上
			if (transMatData[4 * (i + 1)] > 1310720 && transMatData[4 * i] < 1310720 && transMatData[4 * (i + 2)] < 1310720)
			{
				transMatData[4 * (i + 1)] = (transMatData[4 * i]);
				transMatData[4 * (i + 1) + 1] = (transMatData[4 * i + 1]);
				transMatData[4 * (i + 1) + 2] = (transMatData[4 * i + 2]);
				transMatData[4 * (i + 1) + 3] = (transMatData[4 * i + 3]);
			}
		}
	}

	while (true)
	{
		int flag = 0;
		for (int i = 0; i < hCamera.m_camNum; i++) {
			cout << hCamera.m_camFrameList[i].size() << "  ";
			if (100 < hCamera.m_camFrameList[i].size()) {
				hCamera.m_camFrameList[i].clear();
			}
			if (0 < hCamera.m_camFrameList[i].size()) {
				flag++;
			}
		}	
		cout << endl;
		if (hCamera.m_camPath.size() == flag) {
			tstart = cvGetTickCount();
			for (int k = 0; k < hCamera.m_camNum; k++)
			{
				hCamera.m_camCurrrentFrame[k] = hCamera.m_camFrameList[k].back();
				hCamera.m_camFrameList[k].pop_front();
			}
			
#pragma omp parallel for
			for (int j = 0; j < hCamera.result.rows; j++)
			{
				uchar *resultData = hCamera.result.ptr<uchar>(j);
				int *transMatData = hCamera.transMat.ptr<int>(j);
				
				for (int i = 0; i < hCamera.result.cols; i++)
				{					
					if (transMatData[4*i] < 1310720)
					{
						uchar *srcData = hCamera.m_camCurrrentFrame[transMatData[4 * i + 1]].ptr<uchar>(transMatData[4 * i + 2]);	
						uint index = 3 * (uint)transMatData[4 * i + 3];
						resultData[3 * i] = srcData[index];
						resultData[3 * i+ 1] = srcData[index + 1];
						resultData[3 * i +2] = srcData[index + 2];
					}					
				}
			}

			//hCamera.result = hCamera.drawCross(hCamera.result, 80);
			imshow("result", hCamera.result);
			//saveList.push_back(hCamera.result.clone());
			if ('q' == waitKey(1))
				break;
			cout << "                               耗时：" << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;	
		}
		else
		{
			Sleep(3);
		}
	}
	//while (!saveList.empty())
	//{
	//	write << saveList.front();
	//	saveList.pop_front();
	//}
	//write.release();
	return 0;
}

#endif


//生成config.xml
#if 0
void thread_task(VideoCapture &cap, list<Mat> &camFrameList) {
	Mat img;
	while (true)
	{
		cap >> img;
		if (!img.empty())
		{
			camFrameList.push_back(img);	
		}
		this_thread::sleep_for(2ms);
	}
}


int main() {
	Camera hCamera;
	hCamera.init_generate();
	
	thread threads;
	for (int i = 0; i < hCamera.m_camPath.size(); i++)
	{
		threads = thread(thread_task, ref(hCamera.m_capList[i]), ref(hCamera.m_camFrameList[i]));
		threads.detach();
	}

	double tstart = cvGetTickCount();
	hCamera.transMat = hCamera.getTransMat(hCamera.m_transformList);
	//namedWindow("result", 1);
	while (true)
	{
		int flag = 0;
		for (int i = 0; i < hCamera.m_camNum; i++) {
			cout << hCamera.m_camFrameList[i].size() << "  ";
			//if (30 < hCamera.m_camFrameList[i].size()){
			//	hCamera.m_camFrameList[i].clear();
			//}
			if (0 < hCamera.m_camFrameList[i].size()) {
				flag++;
			}
		}
		cout << endl;

		if (6 == flag) {
			for (int k = 0; k < hCamera.m_camNum; k++)
			{
				hCamera.m_camCurrrentFrame[k] = hCamera.m_camFrameList[k].back();
				//hCamera.m_camFrameList[k].pop_front();	
			}
			if (hCamera.result.isContinuous()) {
				memset(hCamera.result.data, 0, hCamera.result.rows * hCamera.result.cols * hCamera.result.channels());
			}

			for (int j = 0; j < hCamera.result.rows; j++)
			{
				uchar *resultData = hCamera.result.ptr<uchar>(j);
				int *transMatData = hCamera.transMat.ptr<int>(j);
				for (int i = 0; i < hCamera.result.cols; i++)
				{
					if (transMatData[4 * i] < 1310720)
					{
						uchar *srcData = hCamera.m_camCurrrentFrame[transMatData[4 * i + 1]].ptr<uchar>(transMatData[4 * i + 2]);

						resultData[3 * i] = srcData[3 * (uint)transMatData[4 * i + 3]];
						resultData[3 * i + 1] = srcData[3 * (uint)transMatData[4 * i + 3] + 1];
						resultData[3 * i + 2] = srcData[3 * (uint)transMatData[4 * i + 3] + 2];
					}
				}
			}

			cout << "                       耗时：" << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;
			tstart = cvGetTickCount();
			for (int k = 0; k < hCamera.m_pointList.size(); k++)
			{
				circle(hCamera.result, hCamera.m_pointListTrans[k], 4, Scalar(0, 255 - k / 4 * 255 / 6, k / 4 * 255 / 6), -1);
			}
			hCamera.result = hCamera.drawCross(hCamera.result, 80);
			setMouseCallback("result", Camera::on_mouse, (void *)&hCamera);
			imshow("result", hCamera.result);
			waitKey(1);
		}
		else
		{
			Sleep(3);
		}
	}
	return 0;
}
#endif

//config版本，备份，20180123
#if 0
void thread_task(string camPath, list<Mat> &camFrameList, int i) {
	VideoCapture cap;
	Mat img;
	cap.open(camPath);
	if (!cap.isOpened())
		cout << "cam " << i << " opened failed!" << endl;
	while (true)
	{
		cap >> img;
		if (!img.empty())
		{
			camFrameList.push_back(img.clone());
		}
		this_thread::sleep_for(2ms);
	}
}

int main() {
	Camera hCamera;
	hCamera.init();

	thread threads;
	for (int i = 0; i < 6; i++)
	{
		threads = thread(thread_task, hCamera.m_camPath[i], ref(hCamera.m_camFrameList[i]), i);
		threads.detach();
	}

	int result_w = 1920;
	int result_h = 1080;
	Mat resultForShow;
	Mat result;
	result.create(result_h - 200, result_w - 160, CV_8UC3);

	double tstart = cvGetTickCount();
	cout << "                       耗时：" << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;
	tstart = cvGetTickCount();
	//namedWindow("result", 1);
	while (true)
	{
		int flag = 0;
		for (int i = 0; i < hCamera.m_camNum; i++) {
			cout << hCamera.m_camFrameList[i].size() << "  ";
			if (0 < hCamera.m_camFrameList[i].size()) {
				flag++;
			}
		}
		cout << endl;

		if (6 == flag) {
			if (result.isContinuous()) {
				memset(result.data, 0, result.rows*result.cols*result.channels());
			}
			for (int i = 0; i < hCamera.m_camNum; i++)
			{
				hCamera.m_camCurrrentFrame[i] = hCamera.m_camFrameList[i].back().clone();
				hCamera.m_camFrameList[i].pop_front();
				hCamera.warpMat(hCamera.m_camCurrrentFrame[i], result, hCamera.m_transformList[i], hCamera.m_camMask[i]);
			}
			//
			cout << "                       耗时：" << (cvGetTickCount() - tstart) / cvGetTickFrequency() / 1000 << "ms" << endl;
			tstart = cvGetTickCount();
			for (int k = 0; k < hCamera.m_pointList.size(); k++)
			{
				//circle(result, hTransform.m_pointList[k], 4, Scalar(0, 0, 255), -1);
				circle(result, hCamera.m_pointListTrans[k], 4, Scalar(0, 255 - k / 4 * 255 / 6, k / 4 * 255 / 6), -1);
			}
			result = hCamera.drawCross(result, 80);
			setMouseCallback("result", Camera::on_mouse, (void *)&hCamera);
			imshow("result", result);
			waitKey(1);
		}
		else
		{
			Sleep(3);
		}
	}
	return 0;
}

#endif

//保存6路视频
#if 0
void thread_task(VideoCapture &cap, list<Mat> &camFrameList) {
	Mat img;
	while (true)
	{
		cap >> img;
		if (!img.empty())
		{
			camFrameList.push_back(img.clone());
		}
		this_thread::sleep_for(2ms);
		if (camFrameList.size() > 350)
		{
			break;
		}
	}
}


int main() {
	Camera hCamera;
	typedef list<Mat> LISTL;
	LISTL list;
	//加载配置文件,相机/视频路径、转换前的点、转换后的点、准换矩阵
	FileStorage readFile("cameraPath.xml", FileStorage::READ);
	if (readFile.isOpened()) {
		readFile["camPath"] >> hCamera.m_camPath;
		readFile.release();
	}
	
	//打开摄像头并保存句柄和视频Size
	for (int i = 0; i < hCamera.m_camPath.size(); i++)
	{
		VideoCapture cap;
		cap.open(hCamera.m_camPath[i]);
		Point2i frameSize;
		if (cap.isOpened()) {
			cout << "camera " << i << " is opened!" << endl;
			frameSize.x = cap.get(CAP_PROP_FRAME_WIDTH);
			frameSize.y = cap.get(CAP_PROP_FRAME_HEIGHT);
			cout << i <<"    "<<cap.get(CAP_PROP_FPS) << endl;
			hCamera.m_frameSizeList.push_back(frameSize);
			hCamera.m_capList.push_back(cap);
			hCamera.m_camFrameList.push_back(list);
		}
		else {
			cout << "camera " << i << " is opened failed!" << endl;
		}
	}
	
	VideoWriter writer0("videone0.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(hCamera.m_frameSizeList[0].x, hCamera.m_frameSizeList[0].y));
	VideoWriter writer1("videone1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(hCamera.m_frameSizeList[1].x, hCamera.m_frameSizeList[1].y));
	VideoWriter writer2("videone2.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(hCamera.m_frameSizeList[2].x, hCamera.m_frameSizeList[2].y));
	VideoWriter writer3("videone3.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(hCamera.m_frameSizeList[3].x, hCamera.m_frameSizeList[3].y));
	VideoWriter writer4("videone4.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(hCamera.m_frameSizeList[4].x, hCamera.m_frameSizeList[4].y));
	VideoWriter writer5("videone5.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(hCamera.m_frameSizeList[5].x, hCamera.m_frameSizeList[5].y));

	thread threads;
	for (int i = 0; i < hCamera.m_camPath.size(); i++)
	{
		threads = thread(thread_task, ref(hCamera.m_capList[i]), ref(hCamera.m_camFrameList[i]));
		threads.detach();
	}

	while (true)
	{
		for (int i = 0; i < hCamera.m_camNum; i++) {
			cout << hCamera.m_camFrameList[i].size() << "  ";
			if (30 < hCamera.m_camFrameList[i].size()){
				hCamera.m_camFrameList[i].clear();
			}	
		}
		cout << endl;

		if (hCamera.m_camFrameList[0].size() && hCamera.m_camFrameList[1].size()
			&& hCamera.m_camFrameList[2].size() && hCamera.m_camFrameList[3].size()
			&& hCamera.m_camFrameList[4].size() && hCamera.m_camFrameList[5].size())
		{
			break;
		}
		Sleep(20);
	}
	while (true)
	{
		cout << "save ";
		for (int i = 0; i < hCamera.m_camNum; i++) {
			cout << hCamera.m_camFrameList[i].size() << "  ";
		}
		cout << endl;
		if (hCamera.m_camFrameList[0].size() && hCamera.m_camFrameList[1].size()
			&& hCamera.m_camFrameList[2].size() && hCamera.m_camFrameList[3].size()
			&& hCamera.m_camFrameList[4].size() && hCamera.m_camFrameList[5].size())
		{
			//imshow("win0", hCamera.m_camFrameList[0].front());
			//imshow("win0", hCamera.m_camFrameList[1].front());
			//imshow("win0", hCamera.m_camFrameList[2].front());
			//imshow("win0", hCamera.m_camFrameList[3].front());
			imshow("win0", hCamera.m_camFrameList[4].front());
			//imshow("win0", hCamera.m_camFrameList[5].front());
			if ('q' == waitKey(1))
			{
				break;
			}
			writer0 << hCamera.m_camFrameList[0].front();
			writer1 << hCamera.m_camFrameList[1].front();
			writer2 << hCamera.m_camFrameList[2].front();
			writer3 << hCamera.m_camFrameList[3].front();
			writer4 << hCamera.m_camFrameList[4].front();
			writer5 << hCamera.m_camFrameList[5].front();

			hCamera.m_camFrameList[0].pop_front();
			hCamera.m_camFrameList[1].pop_front();
			hCamera.m_camFrameList[2].pop_front();
			hCamera.m_camFrameList[3].pop_front();
			hCamera.m_camFrameList[4].pop_front();
			hCamera.m_camFrameList[5].pop_front();

		}		
		Sleep(3);
	}
	return 0;
}
#endif


//目标跟踪
#if 1

Rect2d getFrameRect(Mat frame, Rect2d srcRoi, double scale = 2)
{
	Rect2d resultRoi;
	if (scale < 1) return srcRoi;

	double widthEnlarge = srcRoi.width * (scale - 1) / 2;
	double heightEnlarge = srcRoi.height * (scale - 1) / 2;
	//x
	if (srcRoi.x - widthEnlarge < 0){
		resultRoi.x = 0;
	}else{	
		resultRoi.x = srcRoi.x - widthEnlarge;
	}
	//y
	if (srcRoi.y - heightEnlarge < 0) {
		resultRoi.y = 0;
	}
	else {
		resultRoi.y = srcRoi.y - heightEnlarge;
	}
	//w
	if (srcRoi.x + srcRoi.width  + heightEnlarge > frame.cols) {
		resultRoi.width = frame.cols - resultRoi.x;
	}
	else {
		resultRoi.width = srcRoi.width + widthEnlarge;
	}
	//h
	if (srcRoi.y + srcRoi.height + heightEnlarge > frame.cols) {
		resultRoi.height = frame.cols - resultRoi.y;
	}
	else {
		resultRoi.height = srcRoi.height + heightEnlarge;
	}
	
	return resultRoi;
}
Rect2d getNewRoi(Rect2d frameRect, Rect2d roi) {
	Rect2d newRoi;
	newRoi.x = roi.x - frameRect.x;
	newRoi.y = roi.y - frameRect.y;
	newRoi.width = roi.width;
	newRoi.height = roi.height;
	return newRoi;
}
Rect2d reRoi(Rect2d frameRect, Rect2d newRoi) {
	Rect2d roi;
	roi.x = newRoi.x + frameRect.x;
	roi.y = newRoi.y + frameRect.y;
	roi.width = newRoi.width;
	roi.height = newRoi.height;
	return roi;
}

Rect2d  changeRect( Rect2d &srcRect) {
	Rect2d dstRect;
	dstRect.x = srcRect.x;
	dstRect.y = srcRect.y;
	dstRect.width = srcRect.width;
	dstRect.height = srcRect.height;
	return dstRect;
}

int main()
{
	VideoCapture cap;
	cap.open("video\\Vid1we.avi");
	Mat frame;
	vector<Point2i> objectPath;
	
	if (!cap.isOpened())
	{
		printf("can not open camera \n");
		return -1;
	}
	namedWindow("output", WINDOW_AUTOSIZE);

	//Ptr<Tracker> tracker = Tracker::create("KCF");
	//TLD速度超慢
	//Ptr<Tracker> tracker = Tracker::create("TLD");
	//Ptr<Tracker> tracker = Tracker::create("MEDIANFLOW");

	Ptr<Tracker> trackerKCF = TrackerKCF::create();
	Ptr<Tracker> trackerTLD = TrackerTLD::create();

	int count = 0;
	while (true)
	{
		cap >> frame;
		if (frame.empty()) {
			cout << "not found input video!";
			return 0;
		} 
		imshow("output", frame);
		char ch = waitKey(1);
		if ('b' == ch)
		{
			break;
		}
	}

	Rect2d roi = selectROI("output", frame);
	if (roi.width == 0 || roi.height == 0)
	{
		return -1;
	}
	
	//跟踪
	trackerKCF->init(frame, roi);
	trackerTLD->init(frame, roi);
	int trackeNum = 0;
	objectPath.push_back(Point(roi.x + roi.width / 2, roi.y + roi.height / 2));
	int frame_cn = 1;
	Rect2d oldRoi;
	oldRoi = changeRect(roi);
	cout << frame_cn << " 帧       " << oldRoi << "           " << roi << endl;
	while (frame_cn++)
	{
		cap >> frame;
		if (frame.empty()) break;

		//cvtColor(frame, resultHSV, CV_BGR2HSV);
		//split(resultHSV, HSV);
		//equalizeHist(HSV[2], HSV[2]);
		//merge(HSV, resultHSV);
		//cvtColor(resultHSV, frame, CV_HSV2BGR);

		//Rect2d frameRect = getFrameRect(frame, roi);
		//rectangle(frame, frameRect, Scalar(0, 255, 0,0.5), -5);
		//Mat newFeame = frame(frameRect);
		//Rect2d newRoi = getNewRoi(frameRect, roi);
		//bool isfound = tracker->update(newFeame, newRoi);
		//roi = reRoi(frameRect, newRoi);
		// update the tracking result
		
		
		switch (trackeNum)
		{
		case 0:
			trackerTLD->update(frame, roi);
			if (roi.x + roi.width < oldRoi.x || roi.x > oldRoi.x + oldRoi.width
				|| roi.y + roi.height < oldRoi.y || roi.y > oldRoi.y + oldRoi.height)
			{
				cout << "TLD tracker filed!   "<< frame_cn << endl;
				trackeNum = 1;
				roi = changeRect(oldRoi);
				trackerKCF->init(frame, oldRoi);
			}else
			{
				oldRoi = changeRect(roi);
			}
			break;
		case 1:
			bool isfound = trackerKCF->update(frame, roi);
			if (!isfound)
			{
				cout << "KCF tracker filed!   " << frame_cn << endl;
				trackeNum = 0;
				roi = changeRect(oldRoi);
				trackerTLD->init(frame, oldRoi);
			}else
			{
				oldRoi = changeRect(roi);
			}
			break;
		}
	
		cout << frame_cn<<" 帧       "<< oldRoi << "           " << roi << endl;
		Point2i center;
		center = objectPath.back();
		if (center.x != oldRoi.x + oldRoi.width / 2 && center.y != oldRoi.y + oldRoi.height / 2)
		{
			objectPath.push_back(Point(oldRoi.x + oldRoi.width / 2, oldRoi.y + oldRoi.height / 2));
		}

		for (int i = 0; i < objectPath.size() - 1; i++)
		{
			//line(frame, objectPath[i], objectPath[i+1], Scalar(0, 0, 255));
			circle(frame, objectPath[i], 2, Scalar(0, 0, 255), -1);
		}
		rectangle(frame, roi, Scalar(0, 255, 0), 2);
		imshow("output", frame);
		waitKey(0);
	}

	//cap.release();
	return 0;
}
#endif