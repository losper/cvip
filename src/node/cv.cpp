#include "cv.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <map>
#include <iostream>

extern struct pa_plugin gp;
using namespace std;
using namespace cv;

int getMask(Mat& src, Mat& msk, Rect& rt, int thresh) {
    Mat bwImg;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    // canny 边缘检测
    //Canny(src, canny_output, thresh, thresh * 2, 3);
    cv::threshold(src, bwImg, thresh, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);
    // 寻找轮廓
    findContours(bwImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    // 寻找最大连通域
    double maxArea = 0;
    vector<cv::Point> maxContour;
    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area > maxArea)
        {
            maxArea = area;
            maxContour = contours[i];
        }
    }

    //Mat drawing = Mat::zeros(bwImg.size(), CV_8UC1);
    // 将轮廓转为矩形框
    cv::Rect maxRect = cv::boundingRect(maxContour);

    // 显示连通域
    /*cv::Mat result1, result2;

    bwImg.copyTo(result1);
    bwImg.copyTo(result2);*/

    /*for (size_t i = 0; i < contours.size(); i++)
    {
        cv::Rect r = cv::boundingRect(contours[i]);
        cv::rectangle(result1, r, cv::Scalar(255));
    }*/
    rt = cv::boundingRect(maxContour);
    bwImg(rt).copyTo(msk);
    return 0;
}
int cutImage(Mat& src, Mat& dst, Rect rt) {
    src(rt).copyTo(dst);
    return 0;
}
double calcImageStd(Mat& src, Mat& msk) {
    Mat  convr, Mean;
    meanStdDev(src, convr, Mean, msk);
    return Mean.at<double>(0);
}
double calcImageMean(Mat& src, Mat& msk) {
    Mat  convr, Mean;
    meanStdDev(src, Mean, convr, msk);
    return Mean.at<double>(0);
}
int splitImage(Mat& src, vector<std::pair<Mat, Rect>>& list, int m, int n) {
    int t = m * n;
    int height = src.rows;
    int width = src.cols;

    int ceil_height = height / m;
    int ceil_width = width / n;

    list.resize(t);
    Rect rt = {};
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::pair<Mat, Rect>& elem = list.at(i*m + j);
            rt = { j*ceil_width,  i*ceil_height, ceil_width, ceil_height };
            src(rt).copyTo(elem.first);
            elem.second = rt;
        }
    }
    return 0;
}
void getHorizontal(const char* path, int degrees) {
    Mat src = imread(path);
    if (src.rows > src.cols) {
        if (degrees) {
            cv::rotate(src, src, ROTATE_90_CLOCKWISE);
        }
        else {
            cv::rotate(src, src, ROTATE_90_COUNTERCLOCKWISE);
        }
        imwrite(path, src, { CV_IMWRITE_JPEG_QUALITY ,100 });
    }
}
int resave_impl(const char* src, const char* dst, int type, int qc) {
    Mat buf = imread(src);
    return !imwrite(dst, buf, { type,qc });
}
map<int, double> getStdev(const char* path, int rows, int cols, const char* save, double threshold) {
    Mat msk, dst, src = imread(path), res = imread(path, IMREAD_GRAYSCALE);
    Rect rt;
    vector<std::pair<Mat, Rect>> list;
    vector<std::pair<Mat, Rect>> lmsk;
    map<int, double> out;
    char buf[260];
    double value = 0;

    if (src.empty())
        return out;

    //blur(res, res, Size(3, 3));
    getMask(res, msk, rt, 0);
    cutImage(res, dst, rt);
    splitImage(dst, list, rows, cols);
    splitImage(msk, lmsk, rows, cols);
    for (size_t i = 0; i < list.size(); i++) {
        value = calcImageStd(list[i].first, lmsk[i].first);
        out.insert(std::make_pair(i, value));
        /*sprintf(buf, "%s/list%d.jpg",save, i);
        imwrite(buf, list[i].first);
        sprintf(buf, "%s/mask%d.jpg",save, i);
        imwrite(buf, lmsk[i].first);*/
        if (value > threshold) {
            list[i].second.x += rt.x;
            list[i].second.y += rt.y;
            cv::rectangle(src, list[i].second, cv::Scalar(0, 0, 255), 3);
        }
    }
    sprintf(buf, "%s/result.jpg", save);
    imwrite(buf, src, { CV_IMWRITE_JPEG_QUALITY ,70 });
    return out;
}
long countObject(Mat& src, double th) {
    long result = 0;
    for (auto iter = src.begin<uchar>();
        iter < src.end<uchar>();
        iter++) {
        if (*iter > th) {
            result++;
        };
    }
    return result;
}
map<int, long> getAbsDiff(const char* path, int rows, int cols, const char* save, double threshold, int nums) {
    Mat src = cv::imread(path, IMREAD_COLOR),
        gray = cv::imread(path, IMREAD_GRAYSCALE),
        msk,
        dst,
        hsv;
    Rect rt;
    vector<std::pair<Mat, Rect>> list;
    vector<std::pair<Mat, Rect>> lmsk;
    map<int, long> out;
    long value;
    char buf[260];

    if (src.empty())
        return out;

    getMask(gray, msk, rt, 0);
    cutImage(src, dst, rt);
    //cv::rectangle(src, rt, cv::Scalar(0, 0, 255), 3);

    cv::cvtColor(dst, hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> channels;
    split(dst, channels);
    double dw = calcImageMean(channels[2], msk) + threshold;

    splitImage(channels[2], list, rows, cols);

    for (size_t i = 0; i < list.size(); i++) {
        value = countObject(list[i].first, dw);
        out.insert(std::make_pair(i, value));
        /*sprintf(buf, "%s/list%d.jpg",save, i);
        imwrite(buf, list[i].first);
        sprintf(buf, "%s/mask%d.jpg",save, i);
        imwrite(buf, lmsk[i].first);*/
        if (value > nums) {
            list[i].second.x += rt.x;
            list[i].second.y += rt.y;
            cv::rectangle(src, list[i].second, cv::Scalar(0, 0, 255), 3);
        }
    }

    sprintf(buf, "%s/result_abs.jpg", save);
    imwrite(buf, src, { CV_IMWRITE_JPEG_QUALITY ,70 });
    return out;
}
int absdiff() {
    map<int, long> result;
    /*if (gp.is_string(ctx, 0) && gp.is_number(ctx, 1) && gp.is_number(ctx, 2) && gp.is_string(ctx, 3) && gp.is_number(ctx, 4) && gp.is_number(ctx, 5)) {
        result = getAbsDiff(gp.get_string(ctx, 0), gp.get_int(ctx, 1), gp.get_int(ctx, 2), gp.get_string(ctx, 3), gp.get_number(ctx, 4), gp.get_int(ctx, 5));
    }
    int arr_idx, obj_idx, pos = 0;
    arr_idx = gp.push_array(ctx);
    for (auto n : result)
    {
        obj_idx = gp.push_object(ctx);
        gp.push_int(ctx, n.first);
        gp.put_prop_string(ctx, obj_idx, "no");
        gp.push_number(ctx, n.second);
        gp.put_prop_string(ctx, obj_idx, "value");
        gp.put_prop_index(ctx, arr_idx, pos++);
    }*/
    return 1;
}
int stdev() {
    map<int, double> result;
    /*if (gp.is_string(ctx, 0) && gp.is_number(ctx, 1) && gp.is_number(ctx, 2) && gp.is_string(ctx, 3) && gp.is_number(ctx, 4)) {
        result = getStdev(gp.get_string(ctx, 0), gp.get_int(ctx, 1), gp.get_int(ctx, 2), gp.get_string(ctx, 3), gp.get_number(ctx, 4));
    }
    int arr_idx, obj_idx, pos = 0;
    arr_idx = gp.push_array(ctx);
    for (auto n : result)
    {
        obj_idx = gp.push_object(ctx);
        gp.push_int(ctx, n.first);
        gp.put_prop_string(ctx, obj_idx, "no");
        gp.push_number(ctx, n.second);
        gp.put_prop_string(ctx, obj_idx, "value");
        gp.put_prop_index(ctx, arr_idx, pos++);
    }*/
    return 1;
}
int horizontal() {
    /*if (gp.is_string(ctx, 0) && gp.is_number(ctx, 1)) {
        getHorizontal(gp.get_string(ctx, 0), gp.get_int(ctx, 1));
    }*/
    return 0;
}
int resave() {
    int ret = 0;
    /*if (gp.is_string(ctx, 0) && gp.is_string(ctx, 1) && gp.is_number(ctx, 2) && gp.is_number(ctx, 3)) {
        ret = resave_impl(gp.get_string(ctx, 0), gp.get_string(ctx, 1), gp.get_int(ctx, 2), gp.get_int(ctx, 3));
    }
    gp.push_int(ctx, ret);*/
    return 1;
}
int cutcap() {
    double c = 0;
    VideoCapture video_capture;
    video_capture.set(CV_CAP_PROP_FPS, 120);//帧率
    video_capture.set(CV_CAP_PROP_EXPOSURE, 1250);//曝光时间 10的-12次方秒

    video_capture.open(0);
    if (!video_capture.isOpened()) {
        std::cout << "error in opened!!!!" << std::endl;
        return 0;
    }
    c = video_capture.get(CAP_PROP_EXPOSURE);
    //video_capture.set(CV_CAP_PROP_SETTINGS,1);
    std::cout << c << "c" << std::endl;
    video_capture.set(CV_CAP_PROP_SETTINGS, 1);
    Mat frame;
    video_capture.read(frame);
    imshow("test", frame);
    waitKey();
    auto i = video_capture.get(CV_CAP_PROP_BRIGHTNESS);
    std::cout << i << std::endl;
    //c=video_capture.set(CV_CAP_PROP_FRAME_WIDTH,1024);
    //std::cout << c << "c" << std::endl;
    //c=video_capture.set(CV_CAP_PROP_FRAME_HEIGHT,768);
    //std::cout << c << "c" << std::endl;
    ////video_capture.set(CV_CAP_PROP_BRIGHTNESS,970);
    //video_capture.set(CV_CAP_PROP_CONTRAST,102);
    c = video_capture.get(CAP_PROP_FPS);
    std::cout << c << "c" << std::endl;
    //video_capture.set(CAP_PROP_AUTO_EXPOSURE, 0.25);

    //Mat frame;
    //if (!video_capture.read(frame))
    //{
    //    std::cout << "no video frame" << std::endl;
    //    return 0;
    //}
    //imshow("test", frame);
    //waitKey();
    //if (!video_capture.read(frame))
    //{
    //    std::cout << "no video frame" << std::endl;
    //    return 0;
    //}
    //imshow("test", frame);
    //waitKey();
    //video_capture.set(CV_CAP_PROP_FPS, 120);//帧率
    //video_capture.set(CV_CAP_PROP_EXPOSURE, -12.0);//曝光时间 10的-12次方秒
    //video_capture.open(0);
    //video_capture.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));//压缩格式 mjpg
    //video_capture.set(CV_CAP_PROP_FRAME_WIDTH, 1920); //画面宽
    //video_capture.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);//画面高
    return 0;
}