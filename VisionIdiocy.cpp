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
#include <vector>
#include <string.h>
#define ALLOW_OFF 25
using namespace std;
using namespace cv;
CvCapture* cap=cvCaptureFromCAM(CV_CAP_ANY);
Mat src,dst;
int it=0;
vector<vector<Point> > contours;
bool epilepse=false;
vector<Vec4i> hierarchy;
int off=0;
//Scalar(BLUE,GRREEN,RED)
int offsret=ALLOW_OFF;
void removeSmall(Mat* s, int size){
	Mat element = getStructuringElement( MORPH_RECT,
                                       Size( 2*size + 1, 2*size+1 ),
                                       Point( size, size ) );
	erode( *s, *s, element );
	dilate(*s, *s, element );
}

RNG rng(256);

std::string toString(int i){
	std::stringstream ss;
	ss << i;
	return ss.str();
}

int dist(Point a, Point b){
	return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
}

Point mid(Point a, Point b){
	return Point((a.x+b.x)/2,(a.y+b.y)/2);
}

Point lerp(Point a, Point b, double c){
	if(c > 1)
		return b;
	if(c < 0)
		return a;
	return Point(a.x*(1-c)+b.x*c,a.y*(1-c)+b.y*c);
}

float sign (Point p1, Point p2, Point p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle (Point pt, Point v1, Point v2, Point v3)
{
    bool b1, b2, b3;

    b1 = sign(pt, v1, v2) < 0;
    b2 = sign(pt, v2, v3) < 0;
    b3 = sign(pt, v3, v1) < 0;

    return ((b1 == b2) && (b2 == b3));
}

bool PointInQuad (Point pt, Point v1, Point v2, Point v3, Point v4)
{
    return PointInTriangle(pt,v1,v2,v3) || PointInTriangle(pt,v1,v3,v4);
}

int main(int argc,char* argv[]){		
	namedWindow("Vidia");
	namedWindow("nVidia");
	double avg = 0;
	double amnt = 0;
	Mat element = getStructuringElement( MORPH_RECT,
                                       Size( 7, 7 ),
                                       Point( 3, 3 ) );
	int itl,itr,ibl,ibr;
	int ixtl,ixtr,ixbl,ixbr;
	while(true){
		IplImage* img=cvQueryFrame(cap);
		src=cvarrToMat(img);
		if(epilepse)
			threshold(src,src,0,255,THRESH_BINARY);
		else
			threshold(src,src,25,255,THRESH_BINARY);//Its magic, you know
		inRange(src, Scalar(0,0,0), Scalar(255,0,255), dst);
		inRange(src, Scalar(0,0,0), Scalar(255,0,255), src);
		bitwise_not(src,src);
		bitwise_not(dst,dst);
		//removeSmall(&dst, 5);
		int width = src.size().width;
		int height = src.size().height;
		int widt=width*width+height*height;
		double tlM=widt,trM=widt,blM=widt,brM=widt;                                                            
		Point w,br(width, height),tr(width,0),bl(0, height),tl(0,0);         
		Point xbr(width, height),xtr(width,0),xbl(0, height),xtl(0,0);
		dilate(dst,dst,element);
		findContours( dst, contours, hierarchy,CV_RETR_LIST, CV_CHAIN_APPROX_TC89_L1 );
		circle(src,bl,5,Scalar(255,0,0),5);
		circle(src,tr,5,Scalar(0,255,0),5);
		circle(src,tl,5,Scalar(0,0,255),5);
		circle(src,br,5,Scalar(255,255,255),5);
		dst = Mat::zeros( dst.size(), CV_8UC3 );
		//*
		for(int x=0;x<contours.size();x++){
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       			drawContours( dst, contours, x, color, 2, 8, hierarchy, 0, Point() );
			Point lbr(-1,-1),lbl(-1,-1),ltr(-1,-1),ltl(-1,-1);
			double lx = 0, ly = 0;
			double ax = 0, ay = 0;
			for(int y=0;y<contours[x].size();y++){
				w=contours[x][y];
				lx += w.x;
				ly += w.y;
				ax++;
				ay++;
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
					ltl = w;
					itl=y;
					ixtl=x;
				}if(ctrM<trM){
					trM=ctrM;
					xtr=w;
					ltr = w;
					itr=y;
					ixtr=x;
				}if(cblM<blM){
					blM=cblM;
					xbl=w;
					lbl = w;
					ibl=y;
					ixbl=x;
				}if(cbrM<brM){
					brM=cbrM;
					xbr=w;
					lbr = w;
					ibr=y;
					ixbr=x;
				}
			}
			Point middle = Point((lbr.x+lbl.x+ltr.x+ltl.x)/4,(lbr.y+lbl.y+ltr.y+ltl.y)/4);
			circle(src,middle,2,Scalar(128,128,128),2);
			int size = 2;
			putText(src, toString(dist(lbr,lbl)), mid(lbr,lbl), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));
			putText(src, toString(dist(lbl,ltl)), mid(lbl,ltl), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));
			putText(src, toString(dist(ltl,ltr)), mid(ltr,ltr), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));
			putText(src, toString(dist(ltr,lbr)), mid(ltr,lbr), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));
			Point _tl(ltl.x, ltl.y), _tr(contours[x][0].x,contours[x][0].y), _bl(contours[x][0].x,contours[x][0].y), _br(contours[x][0].x,contours[x][0].y);
			int _ltl = 0;int _ltr = 0;int _lbl = 0;int _lbr = 0;
			for(int y=0;y<contours[x].size();y++){
				w=contours[x][y];
				if(w.x > middle.x){
					if(w.y < middle.y){
						if(dist(middle,w) > _ltr){
							_ltr = dist(middle,w);
							_tr = w;
						}
					}
					if(w.y > middle.y){
						if(dist(middle,w) > _lbr){
							_lbr = dist(middle,w);
							_br = w;
						}
					}
				}
				if(w.x < middle.x){
					if(w.y < middle.y){
						if(dist(middle,w) > _ltl){
							_ltl = dist(middle,w);
							_tl = w;
						}
					}
					if(w.y > middle.y){
						if(dist(middle,w) > _lbl){
							_lbl = dist(middle,w);
							_bl = w;
						}
					}
				}
			}
			int play = (_br.x-_bl.x)/41*3;
			int tape = (_br.x-_bl.x)/41*4;
			Point addTL(_tl.x-play, _tl.y-play),addTR(_tr.x+play, _tr.y-play),addBL(_bl.x-play, _bl.y+play),addBR(_br.x+play, _br.y+play);
			Point subTL (_tl.x+play+tape,_tl.y-play),subTR (_tr.x-play-tape,_tr.y-play),subBL (_bl.x+play+tape,_bl.y-play-tape),subBR (_br.x-play-tape,_br.y-tape-play);
			bool positive = true;
			for(int y=0;y<contours[x].size();y++){
				Point w = contours[x][y];
				if(!PointInQuad(w,addTL,addTR,addBR,addBL) || PointInQuad(w,subTL,subTR,subBR,subBL)){
					positive = false;
					break;
				}
			}
			if(positive){
				line(src, addTL, subTL, color);
			line(src, subTL, subBL, color);
			line(src, subBL, subBR, color);
			line(src, subBR, subTR, color);
			line(src, subTR, addTR, color);
			line(src, addTR, addBR, color);
			line(src, addBR, addBL, color);
			line(src, addBL, addTL, color);
			}
		}//*/
		circle(src,xbl,5,Scalar(255,0,0),5);
		circle(src,xtr,5,Scalar(0,255,0),5);
		circle(src,xtl,5,Scalar(0,0,255),5);
		circle(src,xbr,5,Scalar(255,255,255),5);
		int widthb=xbr.x-xbl.x;
		int widtht=xtr.x-xtl.x;
		int heightl=xbl.y-xtl.y;
		int heightr=xbr.y-xtr.y;
		int totalWidth = widthb > widtht ? widtht : widthb;	
		int totalHeight = heightl > heightr ? heightr : heightl;
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
