#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <cmath>
using namespace std;
using namespace cv;
CvCapture* cap=cvCaptureFromCAM(CV_CAP_ANY);
Mat src,dst;
vector<vector<Point> > contours;
bool epilepse=false;
vector<Vec4i> hierarchy;
int off=0;
int main(int argc,char* argv[]){		
	namedWindow("Vidia");
	namedWindow("nVidia");
	while(waitKey(10) < 1){
		IplImage* img=cvQueryFrame(cap);
		src=cvarrToMat(img);
		if(epilepse)
			threshold(src,src,0,255,THRESH_BINARY);
		else
			threshold(src,src,10,255,THRESH_BINARY);//Its magic, you know
		inRange(src, Scalar(0,0,0), Scalar(255,0,255), dst);
		bitwise_not(dst,dst);
		double tlM=255,trM=255,blM=255,brM=255;
		Point w,br(0,255),tr(0,0),bl(255,255),tl(255,0);
		findContours( dst, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
		for(int x=0;x<contours.size();x++){
			for(int y=0;y<contours[x].size();y++){
				w=contours[x][y];
				int magnitude;
				double ctlM=sqrt(pow((tl.x-w.x),2)+pow(tl.y-w.y)),ctrM=sqrt(pow((tr.x-w.x),2)+pow(tr.y-w.y)),cblM=sqrt(pow((bl.x-w.x),2)+pow(bl.y-w.y)),cbrM=sqrt(pow((br.x-w.x),2)+pow(br.y-w.y));
				//distance bettwen two points = sqrt(pow(x1-x2,2)+pow(y2-y1,2))
				
			}	
		}
		circle(src,br,5,Scalar(0,0,255),5);
		imshow("Vidia",dst);	
		imshow("nVidia",src);
	}
	destroyAllWindows();
}
