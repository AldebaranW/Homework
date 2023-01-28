#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

std::vector<Eigen::Vector3d> readPoint(const std::string &filename);
Eigen::Vector3d cam2world(Eigen::Vector4d &point, const Eigen::Matrix4d &converter,
                          const Eigen::Matrix<double, 3, 4> &cam_f);

int main() {
    Eigen::Vector3d cam_w = {2., 2., 2.};
    Eigen::Quaterniond q(-0.5, 0.5, -0.5, 0.5);
    Eigen::Matrix<double, 3, 4> cam_f;
    cam_f << 400., 0., 190., 0.,
            0., 400., 160., 0.,
            0., 0., 1., 0.;

    std::vector<Eigen::Vector3d> points = readPoint("../points.txt");

    Eigen::Matrix4d convert_mat = [&cam_w, &q]() {
        Eigen::Matrix4d converter = Eigen::Matrix4d::Zero();
        Eigen::Matrix3d rot_mat= q.matrix();
        converter.block(0, 0, 3, 3) = rot_mat.transpose();
        converter.block(0, 3, 3, 1) = -rot_mat.transpose() * cam_w;
        converter(3, 3) = 1.;
        return converter;
    }();


    cv::Mat src = cv::Mat(1000, 1500, CV_8UC3);
    for (int i = 0; i < points.size(); i++) {
        Eigen::Vector4d point_4(points[i][0], points[i][1], points[i][2], 1);
        Eigen::Vector3d res_point_3 = cam2world(point_4, convert_mat, cam_f);
        cv::Point2d res_point(res_point_3[0], -res_point_3[1] + 500);
        cv::circle(src, res_point, 1, cv::Scalar(255, 255, 255), 5);
    }

    cv::imwrite("../res.png", src);

    return 0;
}

std::vector<Eigen::Vector3d> readPoint(const std::string &filename) {
    std::ifstream in(filename);
    std::vector<double> tmp;
    std::vector<Eigen::Vector3d> res;

    if (in.is_open()) {
        while (!in.eof()) {

            double buffer;
            in >> buffer;
            tmp.push_back(buffer);
        }
    }
    else {
        std::cout << "problem" << std::endl;
    }
    in.close();

    for (int i = 1; i < tmp.size() - 1; i += 3) {
        Eigen::Vector3d point(tmp[i], tmp[i + 1], tmp[i + 2]);
        res.push_back(point);
    }

    return res;
}
Eigen::Vector3d cam2world(Eigen::Vector4d &point, const Eigen::Matrix4d &converter,
                          const Eigen::Matrix<double, 3, 4> &cam_f) {
    Eigen::Vector4d c4 = converter * point;
    Eigen::Vector3d vec = cam_f * c4;
    vec /= vec[2];
    return vec;
}

