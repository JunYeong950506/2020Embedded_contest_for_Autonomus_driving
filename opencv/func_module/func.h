#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

bool mouse_is_pressing = false;
Mat src_hsv;
int start_x, start_y, end_x, end_y;
int lower_hue = 40, upper_hue = 80;
int step = 0;


void print_menu(); // �޴���� �Լ�
void swap(int* v1, int* v2); //call by value ��� �����Լ� �������⾲��
void mouse_callback(int event, int x, int y, int flags, void* userdata); // Roi������ 
void Roi(); // ������ �������� ����� ������ �κ� ����ó��
void convert_grayscale();//�޾ƿ� RGB������ �׷��̽����Ϸ� 
void Binarization();// �޾ƿ� ������ ����ȭó���ϴ� �Լ�
void Adaptive_Binarization(); //������ ����ȭ �����Լ�
void trackbar_for_adaptiveThreshold(int pos, void* userdata);//������ 2��ȭ�� Ȯ���ϱ� ���� Ʈ���� ���� �Լ�
void Mopology();// ħ��,��â����� mopology��� ����
void Canny_Edge();// ĳ�Ͼ˰����� ���� �ܰ��� ����
void HSV_check();//HSV ���͸��� ���� Ư�� ���� ����
void Hough_LineDetect();//������ȯ�� ���� ���ε��ü�
void on_hue_changed(int, void*);//HSV ���͸� Ʈ���ٱ����� ���� �����Լ�

void print_menu()
{
	printf("\n\n===============MENU================\n");
	printf("    1. Roi ���� \n");
	printf("    2. Grayscale \n");
	printf("    3. Binarization \n");
	printf("    4. Adaptive Binarization \n");
	printf("    5. Mopology \n");
	printf("    6. Canny edge \n");
	printf("    7. Hough Transform \n");
	printf("    8. HSV Filttering \n");
	printf("    0. ���� \n");
	printf("===================================\n\n");
}

/////////////////////////////ROI�Լ�////////////////////////////////


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
	//���� ĸ�� �ʱ�ȭ
	//VideoCapture cap(1);
	VideoCapture cap("1.mp4");
	if (!cap.isOpened()) {
		cerr << "���� - ī�޶� �� �� �����ϴ�.\n";
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


/////////////////////////////Grayscale�Լ�////////////////////////////////
void convert_grayscale()
{
	//���� ĸ�� �ʱ�ȭ
	//VideoCapture cap(1);
	VideoCapture cap("2.mp4");
	if (!cap.isOpened()) {
		cerr << "���� - ī�޶� �� �� �����ϴ�.\n";
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
		cvtColor(frame, gray, COLOR_BGR2GRAY);	//�׷��̽�����
		resize(frame, frame, Size(640, 320), 0, 0, INTER_LINEAR);
		resize(gray, gray, Size(640, 320), 0, 0, INTER_LINEAR);

		imshow("frame", frame);
		imshow("gray", gray);
		//printf("frame : w : %d ,h : %d\n", frame.size().width, frame.size().height);
		//printf("gray : w : %d ,h : %d\n", gray.size().width, gray.size().height);
		//hconcat�� �������� �������� ���߿� ����
		//hconcat(frame, gray, result);
		//imshow("Grayscale", result);
		
		if (waitKey(25) >= 0)
			break;
	}
	destroyAllWindows();
}

/////////////////////////////Binarization�Լ�////////////////////////////////
void Binarization()
{
	//���� ĸ�� �ʱ�ȭ
	//VideoCapture cap(0);
	VideoCapture cap("3.mp4");
	if (!cap.isOpened()) {
		cerr << "���� - ī�޶� �� �� �����ϴ�.\n";
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
		cvtColor(frame, gray, COLOR_BGR2GRAY);	//�׷��̽�����
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

/////////////////////////////Adaptive_binaryzation�Լ�////////////////////////////////
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

/////////////////////////////Mopology�Լ�////////////////////////////////
void Mopology()
{
	//���� ĸ�� �ʱ�ȭ
	VideoCapture cap(0);
	//VideoCapture cap("test_2.mp4");
	if (!cap.isOpened()) {
		cerr << "���� - ī�޶� �� �� �����ϴ�.\n";
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
		cvtColor(frame, gray, COLOR_BGR2GRAY);	//�׷��̽�����
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


/////////////////////////////Canny edge ���� �Լ�////////////////////////////////

void Canny_Edge()
{
	//���� ĸ�� �ʱ�ȭ
	
	//VideoCapture cap(1);
	VideoCapture cap("3.mp4");
	if (!cap.isOpened()) {
		cerr << "���� - ī�޶� �� �� �����ϴ�.\n";
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
		imshow("C: ĸ�� , V: ��ȭ ,ESC : ����", img_result);


		if (waitKey(25) >= 0)
			break;
	}
	destroyAllWindows();
}

/////////////////////////////hough��ȯ �Լ�////////////////////////////////

void Hough_LineDetect()
{
	Mat grayImg;
	Mat cannyImg;
	Mat houghImg;
	Mat concat;
	//���� �Կ����� ���� �ڵ�
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
		int threshold = 70; // r,�� ��鿡�� ��� ��� �������� ������ �� �������� �Ǵ������� ���� �ּҰ�
		HoughLinesP(cannyImg, lines, 1, CV_PI / 180, 30,30,3);
		//HoughLinesP(cannyImg, lines, 1, CV_PI / 180, threshold);
		cvtColor(cannyImg, houghImg, COLOR_GRAY2BGR);


		//for (size_t i = 0; i < lines.size(); i++)
		for (int i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			line(houghImg, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2,LINE_AA);//�׷��� �̹���/������/����/�÷�/����/����Ʈ��
			//line(originImg, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, LINE_AA);//�׷��� �̹���/������/����/�÷�/����/����Ʈ��
		}
		


		hconcat(originImg, houghImg, concat);
		imshow("Original_Img / Hough_LineDetect_Img", concat);

		if (waitKey(delay) == 27)
			break;
	}
	waitKey();
	destroyAllWindows();

	waitKey(0);
	////���� �Կ����� ���� �ڵ�
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
	//	Canny(frame, edge, 50, 150);//ĳ�ϰ��� 
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



/////////////////////////////HSV_check �Լ�////////////////////////////////
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

