#include <X11/Xlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define SECOND 1000000
#define OUT "out"
#define IN "in"
#define ON "1"
#define OFF "0"

bool x11Active = false;

void probe(){
	x11Active = XOpenDisplay(NULL) ? true : false;
}

void exportPin(std::string pin){
	system(("echo "+pin+" > /sys/class/gpio/export").c_str());
}

void unexportPin(std::string pin){
	system(("echo "+pin+" > /sys/class/gpio/unexport").c_str());
}

void setPinValue(std::string pin, std::string val){
	system(("echo "+val+" > /sys/class/gpio/gpio" + pin + "/value").c_str());
}

void setPinDirection(std::string pin, std::string direction){
	system(("echo "+direction+" > /sys/class/gpio/gpio" + pin + "/direction").c_str());
}


int (&toBits(unsigned int i))[8]{
	int k[8]={0,0,0,0,0,0,0,0};
	if(i > 255){
		for(int i = 0; i < 8; i++){
			k[i] = 1;
		}
	}else{
		for(int i = 0; i < 8; i++){
			k[i] = i & 1;
			i>>=1;
		}
	}
	return k;
}

std::string asInt(int *k){
	std::stringstream ss;
	ss << k;
	std::string str = ss.str();
}

void setAnalogOutput0(unsigned int k){
	int *bits[8] = {toBits(k)};
	setPinDirection("222",OUT);
	setPinValue("222", asInt(bits[0]));
	setPinDirection("219",OUT);
	setPinValue("219", asInt(bits[1]));
	setPinDirection("216",OUT);
	setPinValue("216", asInt(bits[2]));
	setPinDirection("226",OUT);
	setPinValue("226", asInt(bits[3]));
	setPinDirection("81",OUT);
	setPinValue("81", asInt(bits[4]));
	setPinDirection("84",OUT);
	setPinValue("84", asInt(bits[5]));
	setPinDirection("82",OUT);
	setPinValue("82", asInt(bits[6]));
	setPinDirection("160",OUT);
	setPinValue("160", asInt(bits[7]));
}

void setAnalogOutput1(unsigned int k){
	int *bits[8] = {toBits(k)};
	setPinDirection("57",OUT);
	setPinValue("57", asInt(bits[0]));
	setPinDirection("58",OUT);
	setPinValue("58", asInt(bits[1]));
	setPinDirection("221",OUT);
	setPinValue("221", asInt(bits[2]));
	setPinDirection("220",OUT);
	setPinValue("220", asInt(bits[3]));
	setPinDirection("223",OUT);
	setPinValue("223", asInt(bits[4]));
	setPinDirection("166",OUT);
	setPinValue("166", asInt(bits[5]));
	setPinDirection("165",OUT);
	setPinValue("165", asInt(bits[6]));
	setPinDirection("164",OUT);
	setPinValue("164", asInt(bits[7]));
}

void exportPins(){
	exportPin("222");
	exportPin("219");
	exportPin("216");
	exportPin("226");
	exportPin("81");
	exportPin("84");
	exportPin("82");
	exportPin("160");

	exportPin("57");
	exportPin("58");
	exportPin("221");
	exportPin("220");
	exportPin("223");
	exportPin("166");
	exportPin("165");
	exportPin("164");
}

void unexportPins(){
	unexportPin("222");
	unexportPin("219");
	unexportPin("216");
	unexportPin("226");
	unexportPin("81");
	unexportPin("84");
	unexportPin("82");
	unexportPin("160");

	unexportPin("57");
	unexportPin("58");
	unexportPin("221");
	unexportPin("220");
	unexportPin("223");
	unexportPin("166");
	unexportPin("165");
	unexportPin("164");
}


using namespace cv;
using namespace std;
/*
CvCapture* capture;
Mat src, src_gray, thresh;

int red_l = 0, red_h = 255;
int green_l = 0, green_h = 255;
int blue_l = 0, blue_h = 255;

bool init = false;

bool getScreenInfo(Point *tl, Point *br){
	if(!init){
		capture = cvCaptureFromCAM(CV_CAP_ANY);
		init = true;
	}
	IplImage* frame = cvQueryFrame(capture);
	src = cvarrToMat(frame);
	inRange(src, Scalar(red_l, green_l, blue_l), Scalar(red_h, green_h, blue_h), thresh);
	//cvtColor( thresh, src_gray, CV_BGR2GRAY );
	src_gray = thresh.clone();
	blur( src_gray, src_gray, Size(3,3) );
	vector<vector<Point> > contours;
  	vector<Vec4i> hierarchy;
	findContours( src_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	vector<vector<Point> > contours_poly( contours.size() );
	int area;
	for( int i = 0; i < contours.size(); i++ ){
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
       		area = area > boundingRect( Mat(contours_poly[i]) ).area() ? area : boundingRect( Mat(contours_poly[i]) ).area();
		rectangle(src, boundingRect( Mat(contours_poly[i]) ).tl(), boundingRect( Mat(contours_poly[i]) ).br(), Scalar(255,255,255));
	}
	for( int i = 0; i < contours.size(); i++ ){
		if(boundingRect( Mat(contours_poly[i]) ).area() == area){
			*tl = boundingRect( Mat(contours_poly[i]) ).tl();
			*br = boundingRect( Mat(contours_poly[i]) ).br();
			return true;
		}
	}
	return false;
}*/

int main(){
	/*
	probe();
	namedWindow("test");
	Point tl, br;
	while(1){
		if(getScreenInfo(&tl, &br)){
			//rectangle(src, tl, br, Scalar(255,255,255));
		}
		imshow("test", src);
		if(waitKey(10) > 0) break;
	}*/
	exportPins();
	usleep(1000000);
	unexportPins();
	return 0;
}
