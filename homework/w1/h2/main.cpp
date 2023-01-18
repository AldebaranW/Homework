#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
void get_apple(const cv::Mat &img);
void get_apple_2(const cv::Mat & img);

int main(int argc, char** argv) {
    cv::Mat origin = cv::imread("../apple.png");
    cv::Mat img1 = origin.clone();
    cv::Mat img2 = origin.clone();
    get_apple(img1);
    get_apple_2(img2);

    return 0;
}

void get_apple(const cv::Mat &img){
    //思路1：转为HSV提取红色和橙色,二值化，形态学运算，提取轮廓
    //BGR to HSV
    cv::Mat hsv_img;
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);

    //获得红色和橙色
    cv::Mat part1, part2;
    cv::inRange(hsv_img, cv::Scalar(0, 43, 46), cv::Scalar(22, 255, 255), part1);
    cv::inRange(hsv_img, cv::Scalar(156, 43, 46), cv::Scalar(180, 255, 255), part2);

    //红橙取并
    cv::Mat ones_mat = cv::Mat::ones(cv::Size(hsv_img.cols, hsv_img.rows), CV_8UC1);
    cv::Mat res = 255 * (ones_mat - (ones_mat - part1 / 255).mul(ones_mat - part2 / 255));

    //中值滤波
    cv::Mat blured_img;
    cv::medianBlur(res, blured_img, 11);

    //形态学运算
    cv::Mat kernal_open = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(40, 40));
    cv::Mat kernal_close = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(30, 30));
    cv::morphologyEx(blured_img, res, cv::MORPH_OPEN, kernal_open);
    cv::morphologyEx(res, res, cv::MORPH_CLOSE, kernal_close);

    //轮廓提取
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierachy;
    cv::findContours(res, contours, hierachy,cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    //寻找外接矩形
    for (int i = 0; i < contours.size(); ++i)
    {
        cv::Rect rect = cv::boundingRect(cv::Mat(contours[i]));
        cv::rectangle(img, rect, cv::Scalar(255), 2);
    }
    cv::imshow("result from func1", img);
    cv::waitKey(0);
}

void get_apple_2(const cv::Mat &img){
    //思路2：考虑红色通道减绿色通道，二值化，形态学运算，提取轮廓
    //红色减绿色
    cv::Mat channels[3];
    cv::split(img, channels);
    cv::Mat red_sub_green = channels[2] - channels[1];
    cv::normalize(red_sub_green, red_sub_green, 0, 255, cv::NORM_MINMAX);

    //二值化
    cv::Mat thre;
    cv::threshold(red_sub_green, thre, 30, 255, cv::THRESH_BINARY);

    //中值滤波
    cv::Mat blur;
    cv::medianBlur(thre,blur, 9);

    //形态学运算
    cv::Mat kernal = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(35, 35));
    cv::Mat res;
    cv::morphologyEx(blur, res,cv::MORPH_CLOSE, kernal);
    cv::morphologyEx(res, res,cv::MORPH_OPEN, kernal);

    //寻找轮廓
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierachy;
    cv::findContours(res, contours, hierachy,cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    //寻找最大面积轮廓
    double max = 0;
    int maxid = 0;
    for (int i = 0; i < contours.size(); ++i)
    {
        double Area = cv::contourArea(contours[i]);
        if (Area > max){
            max = Area;
            maxid = i;
        }
    }

    //画出矩形
    cv::Rect rect = cv::boundingRect(cv::Mat(contours[maxid]));
    cv::rectangle(img, rect, cv::Scalar(255), 2);
    cv::imshow("result from func2", img);


    cv::waitKey(0);

}
