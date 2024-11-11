#include <opencv2/opencv.hpp>
#include "include/reg.hpp"
#include <vector>

using namespace std;
using namespace cv;

int main(){
    VideoCapture video = VideoCapture("../test3.mp4");//这里三读取视频作为测试了，为电脑不知道为什么不能在虚拟机上调用摄像头，所以用视频代替
    //VideoCapture video;//如果你像调用摄像头的话，把上面那句话注释了，这句话解开注释
    //接上面打开摄像头，然后把while里面的1改为video.isopen()
    //video.open(0);//记得把这句话也解开注释
    double area = 1000;
    while(1){
        Mat frame;//这个是图片存储的一种格式,矩阵
        video.read(frame);
        /*把视频或者你摄像头获得的赋值到图片,因为这个程序主要是对图片进行处理判断,但是好像
         会有些小问题,后面会找机会优化的,这只是初版的程序*/
        GaussianBlur(frame,frame,Size(5,5),0,0);//高斯模糊处理噪点
        Mat im2;
        cvtColor(frame,im2,6);//原图像转换灰度图像
        color(frame, im2);
        //这里是简单的设置窗口大小
        namedWindow("test",WINDOW_NORMAL);
        resizeWindow("test",800,600);
        imshow("test",frame);
        waitKey(60);//额可以说是帧率？
    }
    return 0;
}


