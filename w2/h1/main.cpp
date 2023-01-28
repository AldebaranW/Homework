#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

int main() {
    cv::Mat origin = cv::imread("../1634287874477.png");

    cv::Point2f src[4];
    src[0] = cv::Point2i (740.0, 408.0);
    src[1] = cv::Point2i (750.0, 485.0);
    src[2] = cv::Point2i (960.0, 411.0);
    src[3] = cv::Point2i (970.0, 485.0);

    cv::Point2f dst[4];
    dst[0] = cv::Point2i (0.0, 0.0);
    dst[1] = cv::Point2i (0.0, 140.0);
    dst[2] = cv::Point2i (440.0, 0.0);
    dst[3] = cv::Point2i (440.0, 140.0);

    cv::Mat pers_mat = cv::getPerspectiveTransform(src, dst);

    cv::Mat res;
    cv::warpPerspective(origin, res, pers_mat, cv::Size(440, 140));

    cv::imshow("res", res);
    cv::waitKey(0);
    return 0;
}
