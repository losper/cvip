#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/flann.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/ml.hpp"
#include <vector>


#include <string>
#include <vector>

#include "cvip.h"

using namespace cv;
using namespace std;

vector<unsigned char> buff;
std::string tmp_img;

stPos cvipMatch(const char* src, const char* dst) {
	stPos stp = { 0 };
	cv::Mat image = imread(dst, cv::IMREAD_COLOR);//imdecode(buff, cv::IMREAD_COLOR);
	cv::Mat templateImage = imread(src, cv::IMREAD_COLOR); 

	if (!templateImage.data || !image.data) {
		return stp;
	}

	int result_cols = image.cols - templateImage.cols + 1;
	int result_rows = image.rows - templateImage.rows + 1;

	if (result_cols <0 || result_rows<0) {
		return stp;
	}

	templateImage.at<Vec3b>(0, 0)[0] = ~templateImage.at<Vec3b>(0, 0)[0];
	image.at<Vec3b>(0, 0)[0] = ~image.at<Vec3b>(0, 0)[0];

	cv::Mat result = cv::Mat(result_cols, result_rows, CV_32FC1);
	cv::matchTemplate(image, templateImage, result, CV_TM_CCOEFF_NORMED);

	double minVal, maxVal;
	cv::Point minLoc, maxLoc, matchLoc;
	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	matchLoc = minLoc;

	rectangle(image, maxLoc, Point(maxLoc.x + templateImage.cols, maxLoc.y + templateImage.rows), Scalar(255, 255, 0), 2, 8);

	if (maxVal > 0.9)
	{
		cv::rectangle(image, cv::Rect(matchLoc, cv::Size(templateImage.cols, templateImage.rows)), Scalar(0, 0, 255), 2, 8, 0);
		stp.status = 1;
		stp.x = maxLoc.x + (templateImage.cols / 2);
		stp.y = maxLoc.y + (templateImage.rows / 2);
	}
	stp.val = maxVal;
	return stp;
}
int cvipEncode(const char* src, const char* dst, int cvfmt) {
	//Mat image = imdecode(buff, IMREAD_COLOR);
	cv::Mat image = imread(src, cv::IMREAD_COLOR);//imdecode(buff, cv::IMREAD_COLOR);
	vector<int> param = vector<int>(2);
	param[0] = cvfmt;
	param[1] = 9;
	if (image.data) {
		imwrite(dst, image, param);
		return 0;
	}
	else {
		return -1;
	}
}

int cvipCut(const char* src, const char* dst, int cvfmt, int x, int y, int w, int h) {
    cv::Mat image = imread(src, cv::IMREAD_COLOR);//imdecode(buff, cv::IMREAD_COLOR);
    if (!image.data) {
        return -2;
    }
    cv::Rect rc = { x,y,w,h };
    cv::Mat tmp = image(rc);
    vector<int> param = vector<int>(2);
    param[0] = cvfmt;
    param[1] = 9;

    if (tmp.data) {
        imwrite(dst, tmp, param);
        return 0;
    }
    else {
        return -1;
    }
}