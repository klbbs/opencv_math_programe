#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
using namespace cv;

//各个颜色HSV区间
//红1区间
auto Mired1 = Scalar(0, 43, 221);
auto Mared1 = Scalar(10, 255, 255);
//我也不知道为什么红色有俩区间,所以这是红2区间
auto Mired2 = Scalar(156, 43, 46);
auto Mared2 = Scalar(230, 255, 255);
//橙色区间
auto Miorg = Scalar(11,43, 46);
auto Maorg = Scalar(25, 255, 255);
//绿色区间
auto Migre = Scalar(35, 43, 46);
auto Magre = Scalar(77, 255, 255);
//青色区间,虽然我感觉青色要么像蓝色要么像绿色
auto MiQin = Scalar(35, 43, 46);
auto MaQin = Scalar(99, 255, 255);
//蓝色区间
auto Miblu = Scalar(100, 43, 46);
auto Mablu = Scalar(124, 255, 255);
//紫色区间
auto Miplu = Scalar(125, 43, 46);
auto Maplu = Scalar(155, 255, 255);

string calRang(Scalar& meanCor)
{
    //造了一个像素点的图像，颜色就是那个平均值
    Mat colorMat(1, 1, CV_8UC3, meanCor);
    for(int i = 0; i < 3; i ++)cout << meanCor[i] << ' ';
    cout << endl;
    Mat mask;
    inRange(colorMat, Mired1, Mared1, mask);
    // for(int i = 0; i < 3; i ++)cout << mask << ' ';

    // return "DEBUG";
    if(mask.at<uchar>(0, 0) == 255)return "红色";
    inRange(colorMat, Mired2, Mared2, mask);
    if(mask.at<uchar>(0, 0) == 255)return "红色";
    inRange(colorMat, Miorg, Maorg, mask);
    if(mask.at<uchar>(0,0) == 255)return "橙色";
    inRange(colorMat, Migre, Magre, mask);
    if(mask.at<uchar>(0,0) == 255)return "绿色";
    inRange(colorMat, MiQin, MaQin, mask);
    if(mask.at<uchar>(0,0) == 255)return "青色";
    inRange(colorMat, Miblu, Mablu, mask);
    if(mask.at<uchar>(0,0) == 255)return "蓝色";
    inRange(colorMat, Miplu, Maplu, mask);
    if(mask.at<uchar>(0,0) == 255)return "紫色";
    return "UnKown!";

    // cv::Mat colorMat(1, 1, CV_8UC3, meanCor);
    // cv::Mat mask;

    // for (const auto& [lower, upper, colorName] : colorRanges) 
    // {
    //     cv::inRange(colorMat, lower, upper, mask);
    //     if (mask.at<uchar>(0, 0) == 255) {
    //         return colorName;  // 如果匹配到范围，就返回颜色名
    //     }
    // }

    return "Unknown!";  // 如果没有匹配到任何颜色范围
}

void color(Mat orig, Mat gre)
{
    Mat prfix;
    Canny(gre, prfix, 100, 255);
    //存轮廓点，到时候就处理这些轮廓
    vector<vector<Point>>prfix_ps;
    findContours(prfix, prfix_ps, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    //找到最大的面积作为要处理的图像
    double maxAre = -1;
    int idx = -1; //记录在容器里的下标
    for(int i = 0; i < prfix_ps.size(); i ++)
    {
        double area = contourArea(prfix_ps[i]);
        if( area > maxAre)
        {
            idx = i;
            maxAre = area;
        }
    }
    Mat mask = Mat::zeros(orig.size(), CV_8U);//造个膜mask
    drawContours(mask, prfix_ps, idx, cv::Scalar(255), cv::FILLED);//填充为白色
    imshow("mask", mask);
    Scalar meanCor = mean(orig, mask);//原图像计算颜色平均值
    cout << calRang(meanCor) << endl;
}