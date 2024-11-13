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

//数字模板
static vector<Mat> templates;
string dir = "../template/";
void initTmp() 
{
    if (!templates.empty()) return; // 防止重复加载

    for (int i = 0; i < 10; i ++) {
        string filename = dir + to_string(i) + ".png";
        Mat templ = imread(filename, 0);
        vector<vector<Point>>tmprfx;
        Mat cny;
        Canny(templ,cny,50,200);
        findContours(cny,tmprfx, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        waitKey(100);
        Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        dilate(cny, cny, kernel);
        //fillPoly(templ,tmprfx,Scalar(255,0,0));
        double area = -1;
        int idx = -1;
        for(int i = 0; i < tmprfx.size(); i ++)
        {
            if(area < contourArea(tmprfx[i]))
            {
                area = contourArea(tmprfx[i]);
                idx = i;
            }
            Rect tmp = boundingRect(tmprfx[i]);
            cout << tmp.height << ' ' << tmp.width << endl;
            templ = templ(tmp);
            // imshow("de",templ);
            // waitKey(100);
        }
        threshold(templ,templ,20,255,THRESH_BINARY_INV);
        imshow("debug",templ);
        templates.push_back(templ);
    }
    cout << "load 完成" << endl;
}

string calRang(Scalar& meanCor)
{
    //造了一个像素点的图像，颜色就是那个平均值
    Mat colorMat(1, 1, CV_8UC3, meanCor);
    cvtColor(colorMat,colorMat,COLOR_BGR2HSV);
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
    Canny(gre, prfix, 150, 250);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(prfix, prfix, kernel);
    //存轮廓点，到时候就处理这些轮廓
    vector<vector<Point>>prfix_ps;
    findContours(prfix, prfix_ps, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
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
    drawContours(mask, prfix_ps, idx, Scalar(255), FILLED);//填充为白色
    imshow("mask", mask);
    Scalar meanCor = mean(orig, mask);//原图像计算颜色平均值
    cout << calRang(meanCor) << endl;
}

void digit(Mat orig)
{
    Canny(orig,orig,50,200);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(orig, orig, kernel);
    imshow("debu",orig);
    vector<vector<Point>>trg;
    findContours(orig,trg,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    double maxAre = -1;
    int tmpidx = -1;
    for(int i = 0; i < trg.size(); i ++)
    {
        if(maxAre < contourArea(trg[i]))
        {
            maxAre = contourArea(trg[i]);
            tmpidx = i;
        }
    }
    cout << trg.size() << endl;
    Rect tmp;
    if( tmpidx != -1)
    tmp = boundingRect(trg[tmpidx]);
    else cout << "发生错误!error!" << endl;
    //drawContours(orig,trg,tmpidx,FILLED);//不知道为什么没有填充
    fillPoly(orig,trg,Scalar(255,0,0));
    Mat roi = orig(tmp);
    double Score = -1;
    int idx = -1;
    double maxScore = -1, minScore = -1;
    for(int i = 0; i < templates.size(); i ++)
    {
        resize(roi,roi,templates[i].size());
        imshow("roi",roi);
        imshow("tmplate",templates[i]);
        Mat res;
        matchTemplate(roi,templates[i],res,TM_CCOEFF_NORMED);
        minMaxLoc(res,&minScore,&maxScore);
        if(Score < maxScore)
        {
            Score = maxScore;
            idx = i;
        }
    }
    cout << "这个数字是：" << idx << endl;
}