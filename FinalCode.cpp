#include <X11/Xlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#define WDEBUG true
#define SECOND 1000000
#define OUT "out"
#define IN "in"
#define ON "1"
#define OFF "0"
#define SPEED 1000000

bool x11Active = false;

void probe(){
	x11Active = XOpenDisplay(NULL) ? true : false;
}

void exportPin(std::string pin){
	system(("echo "+pin+" > /sys/class/gpio/export").c_str());
	usleep(SECOND/8);
}

void unexportPin(std::string pin){
	system(("echo "+pin+" > /sys/class/gpio/unexport").c_str());
}

void setPinValue(std::string pin, std::string val){
	system(("echo "+val+" > /sys/class/gpio/gpio" + pin + "/active_low").c_str());
	system(("echo "+val+" > /sys/class/gpio/gpio" + pin + "/value").c_str());
	
}

void setPinDirection(std::string pin, std::string direction){
	system(("echo "+direction+" > /sys/class/gpio/gpio" + pin + "/direction").c_str());
}

std::string asInt(int k){
	std::stringstream ss;
	ss << k;
	std::string str = ss.str();
	return str;
}

int getBits(unsigned int *a, int div){
	int res=((int)*a)&1;
	std::cout<<*a<<"\t";
	*a>>=1;
	if(*a > 1024) *a = 0;
	if(WDEBUG)
	std::cout << res << std::endl;
	return res;
}

void setAnalogOutput0(unsigned int i){
	if(i<=255){
		setPinDirection("222",OUT);
		setPinValue("222", asInt(getBits(&i,1)));
		setPinDirection("219",OUT);
		setPinValue("219", asInt(getBits(&i,2)));
		setPinDirection("216",OUT);
		setPinValue("216", asInt(getBits(&i,3)));
		setPinDirection("226",OUT);
		setPinValue("226", asInt(getBits(&i,4)));
		setPinDirection("81",OUT);
		setPinValue("81", asInt(getBits(&i,5)));
		setPinDirection("84",OUT);
		setPinValue("84", asInt(getBits(&i,6)));
		setPinDirection("82",OUT);
		setPinValue("82", asInt(getBits(&i,7)));
		setPinDirection("160",OUT);
		setPinValue("160", asInt(getBits(&i,8)));
	}
}

void setAnalogOutput1(unsigned int i){
	if(i<=255){
		if(WDEBUG){
			std::cout << "bits" << std::endl;
		}
		setPinDirection("57",OUT);
		setPinValue("57", asInt(getBits(&i,1)));
		setPinDirection("221",OUT);
		setPinValue("221", asInt(getBits(&i,2)));
		setPinDirection("220",OUT);
		setPinValue("220", asInt(getBits(&i,3)));
		setPinDirection("223",OUT);
		setPinValue("223", asInt(getBits(&i,4)));
		setPinDirection("225",OUT);
		setPinValue("225", asInt(getBits(&i,5)));
		setPinDirection("166",OUT);
		setPinValue("166", asInt(getBits(&i,6)));
		setPinDirection("165",OUT);
		setPinValue("165", asInt(getBits(&i,7)));
		setPinDirection("164",OUT);
		setPinValue("164", asInt(getBits(&i,8)));
		usleep(SECOND);
		setPinValue("57", "0");
		setPinValue("221", "0");
		setPinValue("220", "0");
		setPinValue("223", "0");
		setPinValue("225", "0");
		setPinValue("166", "0");
		setPinValue("165", "0");
		setPinValue("164", "0");
	}
}

void exportPins(){
	if(WDEBUG)
	std::cout << "222" << std::endl;
	exportPin("222");
	if(WDEBUG)
	std::cout << "219" << std::endl;
	exportPin("219");
	if(WDEBUG)
	std::cout << "216" << std::endl;
	exportPin("216");
	if(WDEBUG)
	std::cout << "226" << std::endl;
	exportPin("226");
	if(WDEBUG)
	std::cout << "81" << std::endl;
	exportPin("81");
	if(WDEBUG)
	std::cout << "160" << std::endl;
	exportPin("160");

	if(WDEBUG)
	std::cout << "57" << std::endl;
	exportPin("57");
	if(WDEBUG)
	std::cout << "221" << std::endl;
	exportPin("221");
	if(WDEBUG)
	std::cout << "230" << std::endl;
	exportPin("220");
	if(WDEBUG)
	std::cout << "223" << std::endl;
	exportPin("223");
	if(WDEBUG)
	std::cout << "166" << std::endl;
	exportPin("166");
	if(WDEBUG)
	std::cout << "165" << std::endl;
	exportPin("165");
	if(WDEBUG)
	std::cout << "164" << std::endl;
	exportPin("164");

	
	if(WDEBUG)
	std::cout << "161" << std::endl;
	exportPin("161");
	if(WDEBUG)
	std::cout << "162" << std::endl;
	exportPin("162");
	if(WDEBUG)
	std::cout << "163" << std::endl;
	exportPin("163");
	if(WDEBUG)
	std::cout << "225" << std::endl;
	exportPin("225");
}

void unexportPins(){
	unexportPin("222");
	unexportPin("219");
	unexportPin("216");
	unexportPin("226");
	unexportPin("81");
	unexportPin("160");
	
	unexportPin("225");
	unexportPin("163");
	unexportPin("162");
	unexportPin("161");

	unexportPin("57");
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
	int m = 0;
	while((int)m > 0|| (int)m < 255){
		cin >> m;
		if((int)m == -1) break;
		cout << m << endl;
		
		setAnalogOutput1((unsigned int)m);
	}
	usleep(SECOND);
	unexportPins();

	return 0;
}
