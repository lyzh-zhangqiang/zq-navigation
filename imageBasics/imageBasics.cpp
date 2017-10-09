#include <iostream>
#include <chrono>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>
#include <time.h>  
#include <fcntl.h>
#include <sys/types.h>
using namespace std;
using namespace cv;

#define BMP_STATUS 0
/*
 输入：参数所在偏移
 输出：无
 返回：参数
 */
uint8_t zqpara_read(int off)
{
	uint8_t para;
	char filename[] ={"navi_para1.zqpara"};
	int fd;
	if ((fd = open(filename, O_RDWR |O_DSYNC)) == -1)
  	 {
	   printf("Error opening file：navi_para.zqpara\n");		   
  	 }
	lseek(fd, off, SEEK_SET);
	read(fd, &para,1);
	close(fd);
	return para;
}
/*
 输入：参数所在偏移
 输出：参数
 返回：无
 */
void zqpara_write(int off,int* para)
{
	char filename[] ={"navi_para1.zqpara"};
	int fd;
	if ((fd = open(filename, O_RDWR |O_DSYNC)) == -1)
  	 {
	   printf("Error opening file：navi_para.zqpara\n");		   
  	 }
	lseek(fd, off, SEEK_SET);
	write(fd, para,1);
	close(fd);	
}
static void draw_reck(int x,int y,float th,Mat image)
{
    Point points[1][4];  
    points[0][0] = Point( x, -y+800 );
    points[0][1] = Point( x+10*cos(th+2.5+1.57),-y-10*sin(th+2.5+1.57)+800 );
    points[0][2] = Point( x+12*cos(th+1.57),-y-12*sin(th+1.57)+800 );
    points[0][3] = Point( x+10*cos(th-2.5+1.57),-y-10*sin(th-2.5+1.57) +800);
    
    const Point* pt[1] = { points[0] }; 
    int npt[1] = {4};
    
    polylines( image, pt, npt, 1, 1, Scalar(0,0,0)) ; 
    //fillPoly( image, pt, npt, 1, Scalar(250,0,0), 8); 
}
int main ( int argc, char** argv )
{
    // 读取argv[1]指定的图像
    Mat image;    
    int para_bmp;
    while(1)
    {
      while(!zqpara_read(BMP_STATUS))
      {
	printf("waiting for transmit complete\n");
	usleep(300000);
      }
      printf("I get a frame!\n");
      para_bmp=0;
      zqpara_write(BMP_STATUS,&para_bmp);      
      image = imread ( argv[1] ,0); //cv::imread函数读取指定路径下的图像      
      // 判断图像文件是否正确读取
      if( image.data == nullptr ) //数据不存在,可能是文件不存在
	continue;
      
      draw_reck(400,400,45,image);
      
      //cv::namedWindow("image", CV_WINDOW_NORMAL);
      //cv::setWindowProperty("image", CV_WND_PROP_FULLSCREEN，CV_WINDOW_FULLSCREEN);
      
      imshow ( "image", image );      // 用cv::imshow显示图像    
      waitKey ( 200 );                  // 等待200ms
    }   
     
    return 0;
}
