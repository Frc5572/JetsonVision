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
CvCapture* cap = cvCaptureFromCAM(CV_CAP_ANY);
Mat src, dst;
vector<vector<Point> > contours;
bool epilepse = false;
vector<Vec4i> hierarchy;
int off = 0;
int main(int argc, char* argv[]) {
	namedWindow("Vidia");
	namedWindow("nVidia");
	while (waitKey(10) < 1) {
		IplImage* img = cvQueryFrame(cap);
		src = cvarrToMat(img);
		if (epilepse)
			threshold(src, src, 0, 255, THRESH_BINARY);
		else
			threshold(src, src, 10, 255, THRESH_BINARY); //Its magic, you know
		inRange(src, Scalar(0, 0, 0), Scalar(255, 0, 255), dst);
		bitwise_not(dst, dst);
		Point w, br(-1, -1), tr(-1, -1), bl(-1, -1), tl(-1, -1);
		findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL,
				CV_CHAIN_APPROX_SIMPLE);
		int l = -1, r = -1, t = -1, b = -1;
		for (int x = 0; x < contours.size(); x++) {
			for (int y = 0; y < contours[x].size(); y++) {
				w = contours[x][y];
				if (l == -1 || w.x < l)
					l = w.x;
				if (r == -1 || w.x > r)
					r = w.x;
				if (t == -1 || w.x < t)
					t = w.y;
				if (b == -1 || w.x > b)
					b = w.y;
			}
		}
		Point middle = Point((l + r) / 2, (t + b) / 2);
		for (int x = 0; x < contours.size(); x++) {
			for (int y = 0; y < contours[x].size(); y++) {
				w = contours[x][y];
				if (w.x > middle.x && w.y > middle.y) {
					if ((w.x > br.x && w.y > br.y) || br.x == -1
							|| br.y == -1) {
						br = w;
					}
				} else if (w.x < middle.x && w.y > middle.y) {
					if ((w.x < bl.x && w.y > bl.y) || bl.x == -1
							|| bl.y == -1) {
						bl = w;
					}
				} else if (w.x > middle.x && w.y < middle.y) {
					if ((w.x > tr.x && w.y > tr.y) || tr.x == -1
							|| tr.y == -1) {
						tr = w;
					}
				} else if (w.x < middle.x && w.y < middle.y) {
					if ((w.x > tl.x && w.y > tl.y) || tl.x == -1
							|| tl.y == -1) {
						tl = w;
					}
				}
			}
		}
		circle(src, br, 5, Scalar(0, 0, 255), 2);
		imshow("Vidia", dst);
		imshow("nVidia", src);
	}
	destroyAllWindows();
}
