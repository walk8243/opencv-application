#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include "pch.h"
#include "opencv.h"

bool is_same_image(const std::string refFilepath, const std::string targetFilepath, const bool debugFlag) throw(std::runtime_error)
{
    cv::Mat refImage;
    refImage = cv::imread(refFilepath, cv::IMREAD_UNCHANGED);
    if (refImage.empty()) {
        throw std::runtime_error("画像ファイルを見つけられないか開けません(" + refFilepath + ")");
    }
    cv::Mat refMatGray;
    cvtColor(refImage, refMatGray, cv::COLOR_RGB2GRAY);
    cv::Mat refMatThreshold;
    threshold(refMatGray, refMatThreshold, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::Mat targetImage;
    targetImage = cv::imread(targetFilepath, cv::IMREAD_UNCHANGED);
    if (targetImage.empty()) {
        throw std::runtime_error("画像ファイルを見つけられないか開けません(" + targetFilepath + ")");
    }
    cv::Mat targetMatGray;
    cvtColor(targetImage, targetMatGray, cv::COLOR_RGB2GRAY);
    cv::Mat targetMatThreshold;
    threshold(targetMatGray, targetMatThreshold, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::Mat matDiff;
    absdiff(targetMatThreshold, refMatThreshold, matDiff);
    cv::Mat matErode;
    erode(matDiff, matErode, cv::Mat(), cv::Point(-1, -1), 1);
    cv::Mat matDilate;
    dilate(matErode, matDilate, cv::Mat(), cv::Point(-1, -1), 1);

    std::vector<std::vector<cv::Point>> vctContours;
    findContours(matDilate, vctContours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    if (debugFlag)
    {
        int i = 0;
        for (auto contour = vctContours.begin(); contour != vctContours.end(); contour++) {
            drawContours(targetImage, vctContours, i, CV_RGB(0, 0, 255), 2);
            i++;
        }

        imwrite("compare-result.jpg", targetImage);
        imshow("diff", targetImage);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    return vctContours.size() == 0;
}
