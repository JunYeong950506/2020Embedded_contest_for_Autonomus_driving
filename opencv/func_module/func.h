#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

bool mouse_is_pressing = false;
Mat src_hsv;
int start_x, start_y, end_x, end_y;
int lower_hue = 40, upper_hue = 80;
int step = 0;


void print_menu(); // 메뉴출력 함수
void swap(int* v1, int* v2); //call by value 기반 스왑함수 여기저기쓰임
void mouse_callback(int event, int x, int y, int flags, void* userdata); // Roi영역을 
void Roi(); // 선택한 영역만을 남기고 나머지 부분 음영처리
void convert_grayscale();//받아온 RGB영상을 그레이스케일로 
void Binarization();// 받아온 영상을 이진화처리하는 함수
void Adaptive_Binarization(); //적응형 이진화 구현함수
void trackbar_for_adaptiveThreshold(int pos, void* userdata);//적응형 2진화를 확인하기 위한 트랙바 구현 함수
void Mopology();// 침식,팽창방식의 mopology기능 구현
void Canny_Edge();// 캐니알고리즘을 통한 외곽선 검출
void HSV_check();//HSV 필터링을 통한 특정 색상 검출
void Hough_LineDetect();//허프변환을 통한 라인디택션
void on_hue_changed(int, void*);//HSV 필터링 트랙바구현을 위한 보조함수

void print_menu()
{
	printf("\n\n===============MENU================\n");
	printf("    1. Roi 설정 \n");
	printf("    2. Grayscale \n");
	printf("    3. Binarization \n");
	printf("    4. Adaptive Binarization \n");
	printf("    5. Mopology \n");
	printf("    6. Canny edge \n");
	printf("    7. Hough Transform \n");
	printf("    8. HSV Filttering \n");
	printf("    0. 종료 \n");
	printf("===================================\n\n");
}

/////////////////////////////ROI함수////////////////////////////////


void swap(int* v1, int* v2) {
	int temp = *v1;
	*v1 = *v2;
	*v2 = temp;
}


void mouse_callback(int event, int x, int y, int flags, void* userdata)
{
	Mat frame;
	Mat img_result = frame.clone();

	if (event == EVENT_LBUTTONDOWN) {
		step = 1;
		mouse_is_pressing = true;
		start_x = x;
		start_y = y;
	}
	else if (event == EVENT_MOUSEMOVE) {
		if (mouse_is_pressing) {
			end_x = x;
			end_y = y;
			step = 2;
		}
	}
	else if (event == EVENT_LBUTTONUP) {
		mouse_is_pressing = false;
		end_x = x;
		end_y = y;
		step = 3;
	}
}

void Roi()
{
	Mat frame;
	//비디오 캡쳐 초기화
	//VideoCapture cap(1);
	VideoCapture cap("1.mp4");
	if (!cap.isOpened()) {
		cerr << "에러 - 카메라를 열 수 없습니다.\n";
		return ;
	}

	namedWindow("ROI", 1);
	setMouseCallback("ROI", mouse_callback);

	while (true)
	{
		cap.read(frame);
		if (frame.empty()) {
			cerr << "cam load error.\n";
			break;
		}

		switch (step)
		{
		case 1:
			circle(frame, Point(start_x, start_y), 10, Scalar(0, 255, 0), -1);
			break;
		case 2:
			rectangle(frame, Point(start_x, start_y), Point(end_x, end_y), Scalar(0, 255, 0), 3);
			break;
		case 3:
			if (start_x > end_x) {
				swap(&start_x, &end_x);
				swap(&start_y, &end_y);
			}

			Mat ROI(frame, Rect(start_x, start_y, end_x - start_x, end_y - start_y));

			cvtColor(ROI, ROI, COLOR_BGR2GRAY);
			Canny(ROI, ROI, 150, 50);
			cvtColor(ROI, ROI, COLOR_GRAY2BGR);

			ROI.copyTo(frame(Rect(start_x, start_y, end_x - start_x, end_y - start_y)));
			break;
		}
		
		imshow("ROI", frame);

		if (waitKey(25) >= 0)
			break;
	}
	destroyAllWindows();
	
}


/////////////////////////////Grayscale함수////////////////////////////////
void convert_grayscale()
{
	//비디오 캡쳐 초기화
	//VideoCapture cap(1);
	VideoCapture cap("2.mp4");
	if (!cap.isOpened()) {
		cerr << "에러 - 카메라를 열 수 없습니다.\n";
		return;
	}
	//Mat size_t(1280, 480);
	
	while (true)
	{
		Mat frame;
		Mat gray;
		Mat result;
		/*int width = frame.cols;
		int heigth = frame.rows;
		resize(frame, frame, Size(width*0.8, heigth*0.8));
		resize(gray, gray, Size(width*0.8, heigth*0.8));
*/
		cap >>frame;
		if (frame.empty()) {
			cerr << "cam load error.\n";
			break;
		}
		//namedWindow("Grayscale");
		cvtColor(frame, gray, COLOR_BGR2GRAY);	//그레이스케일
		resize(frame, frame, Size(640, 320), 0, 0, INTER_LINEAR);
		resize(gray, gray, Size(640, 320), 0, 0, INTER_LINEAR);

		imshow("frame", frame);
		imshow("gray", gray);
		//printf("frame : w : %d ,h : %d\n", frame.size().width, frame.size().height);
		//printf("gray : w : %d ,h : %d\n", gray.size().width, gray.size().height);
		//hconcat이 정상동작을 하지않음 나중에 수정
		//hconcat(frame, gray, result);
		//imshow("Grayscale", result);
		
		if (waitKey(25) >= 0)
			break;
	}
	destroyAllWindows();
}

/////////////////////////////Binarization함수////////////////////////////////
void Binarization()
{
	//비디오 캡쳐 초기화
	//VideoCapture cap(0);
	VideoCapture cap("3.mp4");
	if (!cap.isOpened()) {
		cerr << "에러 - 카메라를 열 수 없습니다.\n";
		return;
	}
	while (true)
	{
		Mat frame,gray;
		Mat binarization;
		Mat concat;
		int width = frame.cols;
		int height = frame.rows;

		cap >> frame;
		if (frame.empty()) {
			cerr << "cam load error.\n";
			break;
		}
		cvtColor(frame, gray, COLOR_BGR2GRAY);	//그레이스케일
		threshold(gray, binarization, 127, 255, THRESH_BINARY);
		
		resize(frame, frame, Size(640, 320), 0, 0, INTER_LINEAR);
		resize(binarization, binarization, Size(640, 320), 0, 0, INTER_LINEAR);


		imshow("Frame", frame);
		imshow("Binarization", binarization);
		//hconcat(frame, binarization, concat);
		//imshow("Binarization", concat);

		if (waitKey(25) >= 0)
			break;
	}
	destroyAllWindows();
}

/////////////////////////////Adaptive_binaryzation함수////////////////////////////////
void Adaptive_Binarization()
{
	Mat src = imread("ggam.jpg", IMREAD_GRAYSCALE);
	
	if (src.empty()){
		cerr << "Image load failed!" << endl;
		return;
	}
	imshow("Original", src);

	namedWindow("Adaptive_Binarization");
	createTrackbar("pos", "Adaptive_Binarization", 0, 120, trackbar_for_adaptiveThreshold, (void*)&src);
	setTrackbarPos("pos", "Adaptive_Binarization", 11);

	waitKey();
	destroyAllWindows();
	return;
}


void trackbar_for_adaptiveThreshold(int pos, void* userdata)
{
	Mat src = *(Mat*)userdata;
	int bsize = pos;
	if (bsize % 2 == 0) bsize--;
	if (bsize < 3)bsize = 3;
	Mat dst;
	adaptiveThreshold(src, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, bsize, 5);
	imshow("Adaptive_Binarization", dst);
}

/////////////////////////////Mopology함수////////////////////////////////
void Mopology()
{
	//비디오 캡쳐 초기화
	VideoCapture cap(0);
	//VideoCapture cap("test_2.mp4");
	if (!cap.isOpened()) {
		cerr << "에러 - 카메라를 열 수 없습니다.\n";
		return;
	}
	while (true)
	{
		Mat frame, gray;
		Mat binarization;
		Mat erode_img, dilate_img;
		Mat concat1, concat2;
		cap >> frame;
		if (frame.empty()) {
			cerr << "cam load error.\n";
			break;
		}
		cvtColor(frame, gray, COLOR_BGR2GRAY);	//그레이스케일
		erode(gray, erode_img, Mat());
		dilate(gray, dilate_img, Mat());

		imshow("Frame", frame);
		imshow("Erode", erode_img);
		imshow("dilate", dilate_img);
		/*hconcat(frame, erode_img,concat1);
		hconcat(frame, dilate_img, concat2);
		imshow("Erode", concat1);
		imshow("Dilate", concat2);*/
		if (waitKey(25) >= 0)
			break;
	}
	destroyAllWindows();
}


/////////////////////////////Canny edge 검출 함수////////////////////////////////

void Canny_Edge()
{
	//비디오 캡쳐 초기화
	
	//VideoCapture cap(1);
	VideoCapture cap("3.mp4");
	if (!cap.isOpened()) {
		cerr << "에러 - 카메라를 열 수 없습니다.\n";
		return;
	}
	while (1)
	{
		Mat frame;
		Mat binarization;
		Mat erode_img, dilate_img;

		cap >> frame;
		if (frame.empty()) {
			cerr << "cam load error.\n";
			break;
		}
		Mat img_canny;
		Mat img_result;
		Canny(frame, img_canny, 50, 150);
		cvtColor(img_canny, img_canny, COLOR_GRAY2BGR);
		
		resize(frame, frame, Size(640, 320), 0, 0, INTER_LINEAR);
		resize(img_canny, img_canny, Size(640, 320), 0, 0, INTER_LINEAR);

		hconcat(frame, img_canny, img_result);
		imshow("C: 캡쳐 , V: 녹화 ,ESC : 종료", img_result);


		if (waitKey(25) >= 0)
			break;
	}
	destroyAllWindows();
}

/////////////////////////////hough변환 함수////////////////////////////////

void Hough_LineDetect()
{
	Mat grayImg;
	Mat cannyImg;
	Mat houghImg;
	Mat concat;
	//비디오 촬영분을 위한 코드
	//VideoCapture cap(1);
	VideoCapture cap("test_2.mp4");
	if (!cap.isOpened()) {
		cerr << "Camera open failed!" << endl;
		return;
	}

	int w = cvRound(cap.get(CAP_PROP_FRAME_WIDTH));
	int h = cvRound(cap.get(CAP_PROP_FRAME_HEIGHT));
	double fps = cap.get(CAP_PROP_FPS);
	int fourcc = VideoWriter::fourcc('D', 'I', 'V', 'X');
	int delay = cvRound(100 / fps);


	Mat originImg;
	while (true) {
		cap >> originImg;
		if (originImg.empty())
			break;

		cvtColor(originImg, grayImg, COLOR_BGR2GRAY);
		//Canny(grayImg, cannyImg, 125, 250);
		Canny(grayImg, cannyImg, 30, 90,3);

		vector<Vec4i> lines;
		int threshold = 70; // r,θ 평면에서 몇개의 곡선이 한점에서 만났을 때 직선으로 판단할지에 대한 최소값
		HoughLinesP(cannyImg, lines, 1, CV_PI / 180, 30,30,3);
		//HoughLinesP(cannyImg, lines, 1, CV_PI / 180, threshold);
		cvtColor(cannyImg, houghImg, COLOR_GRAY2BGR);


		//for (size_t i = 0; i < lines.size(); i++)
		for (int i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			line(houghImg, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2,LINE_AA);//그려질 이미지/시작점/끝점/컬러/굵기/디폴트값
			//line(originImg, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, LINE_AA);//그려질 이미지/시작점/끝점/컬러/굵기/디폴트값
		}
		


		hconcat(originImg, houghImg, concat);
		imshow("Original_Img / Hough_LineDetect_Img", concat);

		if (waitKey(delay) == 27)
			break;
	}
	waitKey();
	destroyAllWindows();

	waitKey(0);
	////비디오 촬영분을 위한 코드
	//VideoCapture cap(1);
	//if (!cap.isOpened()) {
	//	cerr << "Camera open failed!" << endl;
	//	return ;
	//}
	//int w = cvRound(cap.get(CAP_PROP_FRAME_WIDTH));
	//int h = cvRound(cap.get(CAP_PROP_FRAME_HEIGHT));
	//double fps = cap.get(CAP_PROP_FPS);
	//int fourcc = VideoWriter::fourcc('D', 'I', 'V', 'X');
	//int delay = cvRound(1000 / fps);
	//Mat frame;
	//while (true) {
	//	cap >> frame;
	//	if (frame.empty())
	//		break;
	//	//imshow("frame", frame);
	//	Mat edge;
	//	Canny(frame, edge, 50, 150);//캐니검출 
	//	vector<Vec2f>lines;
	//	HoughLinesP(edge, lines, 1, CV_PI / 100, 30);
	//	Mat dst;
	//	cvtColor(edge, dst, COLOR_GRAY2BGR);
	//	for (size_t i = 0; i < lines.size(); i++) {
	//		float r = lines[i][0], t = lines[i][1];
	//		double cos_t = cos(t), sin_t = sin(t);
	//		double x0 = r * cos_t, y0 = r * sin_t;
	//		double alpha = 1000;
	//		Point pt1(cvRound(x0 + alpha * (-sin_t)), cvRound(y0 + alpha * cos_t));
	//		Point pt2(cvRound(x0 - alpha * (-sin_t)), cvRound(y0 + alpha * cos_t));
	//		line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
	//	}
	//	imshow("frame", frame);
	//	imshow("dst", dst);
	//	if (waitKey(delay) == 27)
	//		break;
	//}
	//waitKey();
	//destroyAllWindows();

}



/////////////////////////////HSV_check 함수////////////////////////////////
void HSV_check()
{
	Mat src;

	src = imread("smarties.png", IMREAD_COLOR);
	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return ;
	}

	cvtColor(src, src_hsv, COLOR_BGR2HSV);
	imshow("src", src);

	namedWindow("mask");
	createTrackbar("Lower hue", "mask", &lower_hue, 179, on_hue_changed);
	createTrackbar("Upper hue", "mask", &upper_hue, 179, on_hue_changed);
	on_hue_changed(0, 0);

	waitKey(0);
	destroyAllWindows();

}
void on_hue_changed(int, void*)
{
	Mat mask;
	Scalar lowerb(lower_hue, 100, 0);
	Scalar upperb(upper_hue, 255, 255);
	inRange(src_hsv, lowerb, upperb, mask);

	imshow("mask", mask);
}

