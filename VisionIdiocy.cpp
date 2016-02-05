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
#include <math.h>
#include <string.h>
#define ALLOW_OFF
using namespace std;
using namespace cv;
CvCapture* cap=cvCaptureFromCAM(CV_CAP_ANY);
Mat src,dst;
Point ignore[5];
int it=0;
vector<vector<Point> > contours;
bool epilepse=false;
vector<Vec4i> hierarchy;
int off=0;
//Scalar(BLUE,GRREEN,RED)
int offsret=ALLOW_OFF;
void smooth(Mat* s, int size){
  Mat element = getStructuringElement( MORPH_RECT,
                                       Size( 2*size + 1, 2*size+1 ),
                                       Point( size, size ) );
  erode( *s, *s, element );
  
  dilate(*s, *s, element );
}

RNG rng(256);

int main(int argc,char* argv[]){		
	namedWindow("Vidia");
	namedWindow("nVidia");
	double avg = 0;
	double amnt = 0;
	Mat element = getStructuringElement( MORPH_RECT,
                                       Size( 7, 7 ),
                                       Point( 3, 3 ) );
	
	while(true){
		IplImage* img=cvQueryFrame(cap);
		src=cvarrToMat(img);
		if(epilepse)
			threshold(src,src,0,255,THRESH_BINARY);
		else
			threshold(src,src,10,255,THRESH_BINARY);//Its magic, you know
		inRange(src, Scalar(0,0,0), Scalar(255,0,255), dst);
		bitwise_not(dst,dst);
		smooth(&dst, 5);
		int width = src.size().width;
		int height = src.size().height;
		int widt=width*width+height*height;
		double tlM=widt,trM=widt,blM=widt,brM=widt;                                                            
		Point w,br(width, height),tr(width,0),bl(0, height),tl(0,0);         
		Point xbr(width, height),xtr(width,0),xbl(0, height),xtl(0,0);
		dilate(dst,dst,element);
		findContours( dst, contours, hierarchy,CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE );
		circle(src,bl,5,Scalar(255,0,0),5);
		circle(src,tr,5,Scalar(0,255,0),5);
		circle(src,tl,5,Scalar(0,0,255),5);
		circle(src,br,5,Scalar(255,255,255),5);
		dst = Mat::zeros( dst.size(), CV_8UC3 );
		//*
		contour:
		for(int x=0;x<contours.size();x++){
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       			drawContours( dst, contours, x, color, 2, 8, hierarchy, 0, Point() );
			for(int y=0;y<contours[x].size();y++){
				w=contours[x][y];
				bool ber=0;
				for(int z=0;z<ALLOW_OFF;z++){
					Point q=ignore[z];
					if(q.x==w.x&&q.y==w.y)
						ber=1;
				}
				if(ber)
					continue;
				circle(dst,w,2,Scalar(128,128,128),2);
				int magnitude;
				double ctlM=(pow(w.x-tl.x,2)+pow(w.y-tl.y,2));
				double ctrM=(pow(w.x-tr.x,2)+pow(w.y-tr.y,2));
				double cbrM=(pow(w.x-br.x,2)+pow(w.y-br.y,2));
				double cblM=(pow(w.x-bl.x,2)+pow(w.y-bl.y,2));
				//distance bettwen two points = sqrt(pow(x1-x2,2)+pow(y2-y1,2))
				if(ctlM<tlM){
					tlM=ctlM;
					xtl=w;
				}if(ctrM<trM){
					trM=ctrM;
					xtr=w;
				}if(cblM<blM){
					blM=cblM;
					xbl=w;
				}if(cbrM<brM){
					brM=cbrM;
					xbr=w;
				}
			}	
		}//*/
		circle(src,xbl,5,Scalar(255,0,0),5);
		circle(src,xtr,5,Scalar(0,255,0),5);
		circle(src,xtl,5,Scalar(0,0,255),5);
		circle(src,xbr,5,Scalar(255,255,255),5);
		int widthb=xbr.x-xbl.x;
		int widtht=xtr.x-xtl.x;
		if((widthb>widtht)&&((widthb-widtht)>offsret)){
			ignore[it]=bl;			
			it++;
			goto contour;
		}		
		for(int i=0;i<ALLOW_OFF;i++)
			ignore[i]=Point(0,0);
		int m = waitKey(10);
		if(m == 32){
			avg += (xbr.x - xbl.x) < 0 ? -(xbr.x - xbl.x) : (xbr.x - xbl.x);
			amnt++;
		}else if(m != -1){
			break;
		}
		double fina = avg / amnt;
		std::cout << fina << std::endl;
		imshow("Vidia",dst);	
		imshow("nVidia",src);
	}
	destroyAllWindows();
}
