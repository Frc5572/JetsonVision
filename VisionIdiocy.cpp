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
using namespace std;
using namespace cv;
CvCapture* cap=cvCaptureFromCAM(CV_CAP_ANY);
Mat src,dst;
vector<vector<Point> > contours;
bool epilepse=false;
vector<Vec4i> hierarchy;
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
		findContours( dst, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
		for(int x=0;x<contours.size();x++){
			for(int y=0;y<contours[x].size();y++){
							
			}	
		}
		imshow("Vidia",dst);	
		imshow("nVidia",src);
	}
	destroyAllWindows();
}
