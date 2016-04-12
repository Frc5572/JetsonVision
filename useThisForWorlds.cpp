#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include <string.h>
#include <thread>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#define ALLOW_OFF 25
#define WDEBUG true
#define SECOND 1000000
#define OUT "out"
#define IN "in"
#define ON "1"
#define OFF "0"
#define SPEED 1000 //in miliseconds

#define DEBUG
//sudo g++ finalVision.cpp -L/usr/local/cuda-7.0/targets/armv7-linux-gnueabihf/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -lopencv_detection_based_tracker -lopencv_esm_panorama -lopencv_facedetect -lopencv_imuvstab -lopencv_tegra -lopencv_vstab -lcufft -lnpps -lnppi -lnppc -lcudart -ltbb -lrt -lpthread -lm -ldl -lX11 -std=c++11 -o thresh



/*
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
*/
std::string asInt(int k){
	std::stringstream ss;
	ss << k;
	std::string str = ss.str();
	return str;
}

/*
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
		setPinDirection("57", OUT); //Pitch
		setPinValue("57", ON);	//Pitch
		setPinDirection("221", OUT); //Yaw
		setPinValue("221", OFF);	//Yaw
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
		setPinDirection("160",OUT);
		setPinValue("160", asInt(getBits(&i,6)));
		setPinDirection("161",OUT);
		setPinValue("161", asInt(getBits(&i,7)));
		setPinDirection("163",OUT);
		setPinValue("163", asInt(getBits(&i,8)));
	}
}

void setAnalogOutput1(unsigned int i){
	if(i<=255){
		if(WDEBUG){
			std::cout << "bits" << std::endl;
		}
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
		setPinDirection("165",OUT); 
		setPinValue("165", asInt(getBits(&i,2))); 
		usleep(SPEED);
		setPinValue("165", "0");		
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
}*/
using namespace std;
/*
void writeToArduino(int getnum){
	FILE *file;
	file = fopen("/dev/ttyTHS1", "w"); //Opening device file
	//fsync(file);	
	fprintf(file, "%d\n", getnum); //Writing to the file 
	fclose(file);
}

void writeToArduino(int yaw, int pitch, int x, int y,int distance){
	
	FILE *file;
	file = fopen("/dev/ttyTHS1", "w"); //Opening device file
	//fsync(file);
	//fprintf(file,'1');
	//fprintf(file, "%d:%d:%d:%d:%d\n", yaw , pitch, x, y,distance); //Writing to the file 
	printf("%d\n",distance);
	fclose(file);
}*/
FILE *file;
/*void writeToArduino(int x, int distance){
	FILE *file;
	
}*/
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

std::string toString(double i){
	std::stringstream ss;
	ss << i;
	return ss.str();
}

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

void writeData(int &fd, string s){
	write(fd, s.c_str(), s.length());
	fcntl(fd,F_SETFL,FNDELAY);
	fdatasync(fd);
}

int main(int argc,char* argv[]){
	#ifdef DEBUG
	namedWindow("Vidia");
	namedWindow("nVidia");
	double avg = 0;
	#endif
	int fd;
	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY | O_APPEND | O_SYNC);
	if(fd == -1){
		perror("open");
		return -1;
	}else{
		fcntl(fd, F_SETFL, 0);
		perror("Port");
	}
	struct termios term;
	tcgetattr(fd, &term);
	cfsetospeed(&term, B115200);
	term.c_cflag &= ~(CLOCAL | CREAD | CSIZE | CSTOPB | PARENB | CRTSCTS | OPOST);
	term.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	term.c_cflag |= (CS8);
	term.c_cflag &= ~CRTSCTS;
	term.c_oflag &= ~OPOST;
	tcsetattr(fd, TCSANOW, &term);
	double amnt = 0;
	Mat element = getStructuringElement( MORPH_RECT,
                                       Size( 7, 7 ),
                                       Point( 3, 3 ) );
	int itl,itr,ibl,ibr;
	int ixtl,ixtr,ixbl,ixbr;
	Mat colorSrc;
	while(true){
		
		IplImage* img=cvQueryFrame(cap);
		src=cvarrToMat(img);
		colorSrc = src.clone();
		if(epilepse)
			threshold(src,src,0,255,THRESH_BINARY);
		else
			threshold(src,src,25,255,THRESH_BINARY);//Its magic, you know
		inRange(src, Scalar(0,0,0), Scalar(255,0,255), src);
		bitwise_not(src,src);
		dst = src.clone();
		removeSmall(&dst, 2);
		int width = src.size().width;
		int height = src.size().height;
		int widt=width*width+height*height;
		double tlM=widt,trM=widt,blM=widt,brM=widt;                                                            
		Point w,br(width, height),tr(width,0),bl(0, height),tl(0,0);         
		Point xbr(width, height),xtr(width,0),xbl(0, height),xtl(0,0);
		dilate(dst,dst,element);
		findContours( dst, contours, hierarchy,CV_RETR_LIST, CV_CHAIN_APPROX_TC89_L1 );
		#ifdef DEBUG
		circle(src,bl,5,Scalar(255,0,0),5);
		circle(src,tr,5,Scalar(0,255,0),5);
		circle(src,tl,5,Scalar(0,0,255),5);
		circle(src,br,5,Scalar(255,255,255),5);
		#endif
		dst = Mat::zeros( dst.size(), CV_8UC3 );
		int finalwidtht, finalwidthb, finalheightl, finalheightr;
		Point finalMid;
		//*
		double distance;
		bool positive;
		for(int x=0;x<contours.size();x++){
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			#ifdef DEBUG       			
			drawContours( dst, contours, x, color, 2, 8, hierarchy, 0, Point() );
			#endif
			Point lbr(-1,-1),lbl(-1,-1),ltr(-1,-1),ltl(-1,-1);
			double lx = 0, ly = 0;
			double ax = 0, ay = 0;
			for(int y=0;y<contours[x].size();y++){
				w=contours[x][y];
				lx += w.x;
				ly += w.y;
				ax++;
				ay++;
				#ifdef DEBUG				
				circle(dst,w,2,Scalar(128,128,128),2);
				#endif				
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
			#ifdef DEBUG			
			circle(src,middle,2,Scalar(128,128,128),2);
			int size = 2;
			putText(src, toString(dist(lbr,lbl)), mid(lbr,lbl), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));
			putText(src, toString(dist(lbl,ltl)), mid(lbl,ltl), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));
			putText(src, toString(dist(ltl,ltr)), mid(ltr,ltr), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));
			putText(src, toString(dist(ltr,lbr)), mid(ltr,lbr), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));
			#endif
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
			#ifdef DEBUG
			line(colorSrc, _br, _bl, Scalar(255,0,255));
			line(colorSrc, _tr, _tl, Scalar(0,255,255));
			#endif
			int _width = (_br.x-_bl.x) + (_tr.x-_tl.x);
			int play = _width/41*5;
			int tape = _width/41*2;
			Point addTL(_tl.x-play, _tl.y-play),addTR(_tr.x+play, _tr.y-play),addBL(_bl.x-play, _bl.y+play),addBR(_br.x+play, _br.y+play);
			Point subTL (_tl.x+play+tape,_tl.y-play),subTR (_tr.x-play-tape,_tr.y-play),subBL (_bl.x+play+tape,_bl.y-play-tape),subBR (_br.x-play-tape,_br.y-tape-play);
			positive = addTL.x > 0 && addBL.x > 0 && addBR.x > 0 && addTR.x > 0;
			//bool positive=true;
			for(int y=0;y<contours[x].size();y++){
				Point w = contours[x][y];
				if(!PointInQuad(w,addTL,addTR,addBR,addBL) || PointInQuad(w,subTL,subTR,subBR,subBL)){
					positive = false;
					#ifdef DEBUG					
					circle(colorSrc, w, 5, Scalar(0,0,255), 5);
					circle(colorSrc, addTL, 5, Scalar(0,0,255), 5);
					circle(colorSrc, addTR, 5, Scalar(0,255,255), 5);
					circle(colorSrc, addBR, 5, Scalar(0,255,0), 5);
					circle(colorSrc, addBL, 5, Scalar(255,255,0), 5);
					line(colorSrc, addTL, subTL, Scalar(0,0,255));
					line(colorSrc, subTL, subBL, Scalar(0,0,255));
					line(colorSrc, subBL, subBR, Scalar(0,0,255));
					line(colorSrc, subBR, subTR, Scalar(0,0,255));
					line(colorSrc, subTR, addTR, Scalar(0,0,255));
					line(colorSrc, addTR, addBR, Scalar(0,0,255));
					line(colorSrc, addBR, addBL, Scalar(0,0,255));
					line(colorSrc, addBL, addTL, Scalar(0,0,255));
					#endif
					break;
				}
			}
			
			if(positive){
				#ifdef DEBUG
				line(colorSrc, addTL, subTL, Scalar(255,255,255));
				line(colorSrc, subTL, subBL, Scalar(255,255,255));
				line(colorSrc, subBL, subBR, Scalar(255,255,255));
				line(colorSrc, subBR, subTR, Scalar(255,255,255));
				line(colorSrc, subTR, addTR, Scalar(255,255,255));
				line(colorSrc, addTR, addBR, Scalar(255,255,255));
				line(colorSrc, addBR, addBL, Scalar(255,255,255));
				line(colorSrc, addBL, addTL, Scalar(255,255,255));
				#endif
				finalwidtht = _tr.x - _tl.x;
				finalwidthb = _width;
				finalheightl = _bl.x - _tl.x;
				finalheightr = _br.x - _tr.x;
				finalMid = middle;
				distance = 21834.83/((finalwidthb+finalwidtht)/2);
				
break;
			}
			

		}//*/
		if(positive){
				
					//x,y,width,height,distance
					
					//thread arduino(writeToArduino,(int)finalMid.x,width,(int)(addTR.x-addTL.x),(int)(addTR.y-addTL.y),(int)(distance));
				//fprintf(file,"%d\n",distance);
				//char buf[50];
				//snprintf(buf,sizeof(buf),"echo '%d:%d:%d:%d:%d' > /dev/ttyTHS1",(int)finalMid.x,width,0,1,(int)(distance));
				//std::system(buf); //Writing to the file
				char hi[25];
				snprintf(hi, sizeof(hi),"%d:%d:%d:%d:%d\n",(int)finalMid.x,width,0,1,(int)(distance));				
				writeData(fd, string(hi));
				}
		#ifdef DEBUG
		putText(colorSrc, toString((double)distance), finalMid, FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255,255));
		putText(colorSrc, toString((double)(finalMid.x)), Point(finalMid.x,finalMid.y+20), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255,255));
		circle(src,xbl,5,Scalar(255,0,0),5);
		circle(src,xtr,5,Scalar(0,255,0),5);
		circle(src,xtl,5,Scalar(0,0,255),5);
		circle(src,xbr,5,Scalar(255,255,255),5);
		#endif
		int widthb=xbr.x-xbl.x;
		int widtht=xtr.x-xtl.x;
		int heightl=xbl.y-xtl.y;
		int heightr=xbr.y-xtr.y;
		int totalWidth = widthb > widtht ? widtht : widthb;	
		int totalHeight = heightl > heightr ? heightr : heightl;		
		int m = waitKey(20);
		#ifdef DEBUG
		if(m == 32){
			avg += (xbr.x - xbl.x) < 0 ? -(xbr.x - xbl.x) : (xbr.x - xbl.x);
			amnt++;
		}else if(m != -1){
			break;
		}
		double fina = avg / amnt;
		std::cout << fina << std::endl;
		imshow("Vidia",dst);	
		imshow("nVidia",colorSrc);
		#endif
	}
	close(fd);
#ifdef DEBUG
	destroyAllWindows();
#endif
}
