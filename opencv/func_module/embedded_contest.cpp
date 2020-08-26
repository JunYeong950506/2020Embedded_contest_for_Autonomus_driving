#include <iostream>
#include "opencv2/opencv.hpp"
#include "func.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
using namespace cv;

/*
1.ROI 설정
2.Grayscale
3.Binarization
4.Adaptive Binarization
5.Mopology
6.Canny edge
	-smoothing
	-finding gradient
	-non-maximum suppression
	-double thresholding
	-edge tracking
7.Hough transform
	-필요없는 직선 골라내기 (필터링)
	-대표직선 구하기
	-소실점 추출
8.HSV필터링(특정 색 추출)
*/

int main()
{
	int num;
	while (1)
	{
		print_menu();

		scanf("%d", &num);

		switch (num) {
		case 1:
			printf("1. ROI 설정\n");
			Roi();
			break;
		case 2:
			printf("2. Grayscale \n");
			convert_grayscale();
			break;
		case 3:
			printf("3. Binarization(이진화)\n");
			Binarization();
			break;
		case 4:
			printf("4. Mopology \n");
			Adaptive_Binarization();
				break;
		case 5:
			printf("5. Mopology \n");
			Mopology();
			break;
		case 6:
			printf("6. Canny edge \n");
			Canny_Edge();
			break;
		case 7:
			printf("7. Hough transform \n");
			Hough_LineDetect();
			break;
		case 8:
			printf("8. HSV필터링 \n");
			HSV_check();
			break;
		case 0:
			printf("종료합니다 \n");
			return 0;
		default:
			printf("--입력오류--\n");
			break;
		}
	}

	return 0;
}


