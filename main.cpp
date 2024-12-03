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
        Mat gre,binary;
        cvtColor(frame,gre,6);
        threshold(gre, binary, 100, 255, THRESH_BINARY);
        imshow("bin",binary);
        //数字
        digit(binary);
        //颜色
        color(frame,binary);
        waitKey(3);
    }
    return 0;
}
