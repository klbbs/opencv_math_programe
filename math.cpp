#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;
//各个颜色HSV区间
//红1区间
auto Mired1 = Scalar(0, 43, 221);
auto Mared1 = Scalar(10, 255, 255);
//我也不知道为什么红色有俩区间,所以这是红2区间
auto Mired2 = Scalar(156, 43, 46);
auto Mared2 = Scalar(180, 255, 255);
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
 
int main(){
    VideoCapture video = VideoCapture("../test.mp4");//这里三读取视频作为测试了，为电脑不知道为什么不能在虚拟机上调用摄像头，所以用视频代替
    //VideoCapture video;//如果你像调用摄像头的话，把上面那句话注释了，这句话解开注释
    //接上面打开摄像头，然后把while里面的1改为video.isopen()
    //video.open(0);//记得把这句话也解开注释
    double area = 1000;
    while(1){
        Mat frame;//这个是图片存储的一种格式,矩阵
        video.read(frame);
        /*把视频或者你摄像头获得的赋值到图片,因为这个程序主要是对图片进行处理判断,但是好像
         会有些小问题,后面会找机会优化的,这只是初版的程序*/
        GaussianBlur(frame,frame,Size(3,3),0,0);//高斯模糊处理噪点
        Mat im2,mask;
        cvtColor(frame,im2,COLOR_BGR2HSV);//原图像转换HSV图像
        inRange(im2,Miblu,Mablu,mask);//我也不知道应该叫啥这个mask,总之就是提取了这个范围内颜色的东西到mask里
        vector<vector<Point>>Contain;//存储找到的点
        findContours(mask,Contain,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);//调用这个让它帮忙把那个图像里的物体提取出来成点并且记录到刚才那个contain里面
        for(int i=0;i<Contain.size();i++){
            Rect box = boundingRect(Contain[i]);
            double needarea = box.area();
            //下面有个范围判断,是防止有噪点,导致乱识别
            if(needarea > area){
                rectangle(frame,box,Scalar(0,255,0),5);
                Point center(box.x + box.width/2,box.y + box.height/2);
                cout << center << endl;
            }
        }
        //这里是简单的设置窗口大小
        namedWindow("test2",WINDOW_NORMAL);
        namedWindow("test",WINDOW_NORMAL);
        resizeWindow("test",800,600);
        resizeWindow("test2",800,600);
        imshow("test2",mask);
        imshow("test",frame);
        waitKey(60);//额可以说三帧率？
    }
    return 0;
}


