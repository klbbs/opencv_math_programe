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
auto Mired2 = Scalar(156, 43, 46);
auto Mared2 = Scalar(230, 255, 255);
//绿色区间
auto Migre = Scalar(35, 43, 46);
auto Magre = Scalar(77, 255, 255);
auto MiQin = Scalar(35, 43, 46);
auto MaQin = Scalar(99, 255, 255);

//数字模板
static vector<Mat> templates;
string dir = "../template/digit_";
void initTmp() 
{
    if (!templates.empty()) return; // 防止重复加载
    for (int i = 0; i < 10; i ++) {
        string filename = dir + to_string(i) + ".png";
        Mat templ = imread(filename, 0);
        threshold(templ, templ, 127, 255, THRESH_BINARY_INV);
        vector<vector<Point>>tmprfx;
        findContours(templ,tmprfx, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        Rect tmp = boundingRect(tmprfx[0]);
        templ = templ(tmp);
        //imshow("debug",templ);
        templates.push_back(templ);
    }
    cout << "load 完成" << endl;
}

string calRang(Scalar& meanCor)
{
    Mat colorMat(1, 1, CV_8UC3, meanCor);
    cvtColor(colorMat,colorMat,COLOR_BGR2HSV);
    for(int i = 0; i < 3; i ++)cout << meanCor[i] << ' ';
    cout << endl;
    Mat mask;
    inRange(colorMat, Mired1, Mared1, mask);
    if(mask.at<uchar>(0, 0) == 255)return "红色";
    inRange(colorMat, Mired2, Mared2, mask);
    if(mask.at<uchar>(0, 0) == 255)return "红色";
    inRange(colorMat, Migre, Magre, mask);
    if(mask.at<uchar>(0,0) == 255)return "青色/绿色";
    inRange(colorMat, MiQin, MaQin, mask);
    if(mask.at<uchar>(0,0) == 255)return "青色/绿色";

    return "Unknown!";
}

void color(Mat orig, Mat bin)
{
    Mat kernel = getStructuringElement(MORPH_RECT, Size(10, 10));
    dilate(bin, bin, kernel);
    vector<vector<Point>>prfix_ps;
    findContours(bin, prfix_ps, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    double maxAre = -1;
    int idx = -1;
    for(int i = 0; i < prfix_ps.size(); i ++)
    {
        double area = contourArea(prfix_ps[i]);
        if( area > maxAre)
        {
            idx = i;
            maxAre = area;
        }
    }
    Rect roi = boundingRect(prfix_ps[idx]);
    orig = orig(roi);
    bin = bin(roi);
    imshow("bin",bin);
    GaussianBlur(orig,orig,Size(21,21),0,0);
    Mat ker_org = getStructuringElement(MORPH_RECT, Size(45,45));
    dilate(orig,orig,ker_org);
    imshow("colorDebug", orig);
    //Mat mask = Mat::zeros(orig.size(), CV_8U);
    //drawContours(mask, prfix_ps, idx, Scalar(255), FILLED);
    //imshow("mask",mask);
    Scalar meanCor = mean(orig);
    cout << calRang(meanCor) << endl;
}

void digit(Mat orig)
{
    Mat ker_dil = getStructuringElement(MORPH_RECT, Size(80,80));
    Mat ker_ero = getStructuringElement(MORPH_RECT, Size(30,30));
    dilate(orig, orig, ker_dil);
    //imshow("dil",orig);
    vector<vector<Point>>trg;
    vector<Vec4i>his;
    findContours(orig,trg,his,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
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
    Rect tmp;
    if( tmpidx != -1)
    tmp = boundingRect(trg[tmpidx]);
    else cout << "发生错误!error!" << endl;
    erode(orig,orig,ker_ero);
    Mat roi = orig(tmp);
    //imshow("roi",roi);
    double score = -1;
    int idx = -1;
    double maxScore = -1, minScore = -1;
    for (int i = 0; i < templates.size(); i++) {
        if (roi.empty() || templates[i].empty()) continue;
        Mat resizedRoi;
        resize(roi, resizedRoi, templates[i].size());
        Mat res;
        matchTemplate(resizedRoi, templates[i], res, TM_CCOEFF_NORMED);
        minMaxLoc(res, &minScore, &maxScore);
        if (score < maxScore) {
            score = maxScore;
            idx = i;
        }
    }
    if (idx != -1)
        cout << "识别的数字是：" << idx << endl;
}
