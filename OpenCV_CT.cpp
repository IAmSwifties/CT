#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	string input_path = argv[1];
	string output_path = argv[2];

	Mat img = imread(input_path, 0);
	if (img.empty()) 
	{
		cout << "Couldn't read the img: " << input_path << endl;
	}
	imshow("1", img);
	imwrite("原圖.png", img);

	Mat img2;
	threshold(img, img2, 225, 255, THRESH_BINARY); //二值化
	imshow("2", img2);
	imwrite("二值化.png", img2);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(img2, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point()); //找輪廓
	
	int k = 0, maxArea = 0, area = 0;
	for (int i = 0; i < contours.size(); i++) 
	{
		area = contourArea(contours[i]);
		if (area > maxArea) 
		{
			k = i;
			maxArea = area;
		}
	}

	Mat img4 = Mat::zeros(img.size(), CV_8UC1);
	drawContours(img4, contours, k, Scalar(255, 255, 255), -1, 8, hierarchy); //畫輪廓
	imshow("4", img4);

	Mat img5;
	bitwise_and(img, img4, img5);
	imshow("5", img5);
	imwrite("移除頭骨外側組織.png", img5);
	
	Mat img6 = img5.clone();
	RotatedRect anEllipse = fitEllipse(contours[k]);
	ellipse(img6, anEllipse, Scalar(255), 2, CV_8UC1);
	imshow("6", img6);
	
	int angle = 0;
	Mat img7;
	if (anEllipse.angle > 90)
		angle = -(180 - anEllipse.angle);
	else
		angle = anEllipse.angle;
	Mat rotatedMartix = getRotationMatrix2D(anEllipse.center, angle, 1.0);
	warpAffine(img5, img7, rotatedMartix, img5.size());
	imshow("7", img7);

	imwrite(output_path, img7);
	waitKey(0);
	return 0;
}