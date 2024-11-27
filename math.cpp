#include <opencv2/opencv.hpp>
#include "include/reg.hpp"
#include <vector>

using namespace std;
using namespace cv;

int main(){
    VideoCapture video = VideoCapture("../test4.avi");
    initTmp();
    int last = 0;
    while(1){
        Mat frame;
        video.read(frame);
        GaussianBlur(frame,frame,Size(5,5),0,0);
        Mat im2,binary;
        cvtColor(frame,im2,6);
        threshold(im2, binary, 175, 255, THRESH_BINARY);
        imshow("bin",binary);
        //数字
        last = digit(binary, last);
        //颜色
        color(frame,im2);
        namedWindow("test",WINDOW_NORMAL);
        resizeWindow("test",800,600);
        //imshow("test",frame);
        waitKey(60);
    }
    return 0;
}
