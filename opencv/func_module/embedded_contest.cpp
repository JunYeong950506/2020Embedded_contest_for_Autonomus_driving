#include <iostream>
#include "opencv2/opencv.hpp"
#include "func.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
using namespace cv;

/*
1.ROI ����
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
	-�ʿ���� ���� ��󳻱� (���͸�)
	-��ǥ���� ���ϱ�
	-�ҽ��� ����
8.HSV���͸�(Ư�� �� ����)
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
			printf("1. ROI ����\n");
			Roi();
			break;
		case 2:
			printf("2. Grayscale \n");
			convert_grayscale();
			break;
		case 3:
			printf("3. Binarization(����ȭ)\n");
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
			printf("8. HSV���͸� \n");
			HSV_check();
			break;
		case 0:
			printf("�����մϴ� \n");
			return 0;
		default:
			printf("--�Է¿���--\n");
			break;
		}
	}

	return 0;
}


