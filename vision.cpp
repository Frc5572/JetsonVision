<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <meta http-equiv="Content-Style-Type" content="text/css">
  <title></title>
  <meta name="Generator" content="Cocoa HTML Writer">
  <meta name="CocoaVersion" content="1038.36">
  <style type="text/css">
    p.p1 {margin: 0.0px 0.0px 0.0px 0.0px; font: 12.0px Helvetica}
    p.p2 {margin: 0.0px 0.0px 0.0px 0.0px; font: 12.0px Helvetica; min-height: 14.0px}
    span.Apple-tab-span {white-space:pre}
  </style>
</head>
<body>
<p class="p1">#include &lt;iostream&gt;</p>
<p class="p1">#include &lt;stdlib.h&gt;</p>
<p class="p1">#include &lt;stdio.h&gt;</p>
<p class="p1">#include &lt;opencv2/opencv.hpp&gt;</p>
<p class="p1">#include &lt;opencv2/gpu/gpu.hpp&gt;</p>
<p class="p1">#include &lt;opencv2/imgproc/imgproc.hpp&gt;</p>
<p class="p1">#include &lt;opencv2/highgui/highgui.hpp&gt;</p>
<p class="p1">#include &lt;unistd.h&gt;</p>
<p class="p1">#include &lt;ctype.h&gt;</p>
<p class="p1">#include &lt;math.h&gt;</p>
<p class="p1">#include &lt;vector&gt;</p>
<p class="p1">#include &lt;string.h&gt;</p>
<p class="p1">#define ALLOW_OFF 25</p>
<p class="p1">#define WDEBUG true</p>
<p class="p1">#define SECOND 1000000</p>
<p class="p1">#define OUT "out"</p>
<p class="p1">#define IN "in"</p>
<p class="p1">#define ON "1"</p>
<p class="p1">#define OFF "0"</p>
<p class="p1">#define SPEED 1000 //in miliseconds</p>
<p class="p1">//sudo g++ finalVision.cpp -L/usr/local/cuda-7.0/targets/armv7-linux-gnueabihf/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -lopencv_detection_based_tracker -lopencv_esm_panorama -lopencv_facedetect -lopencv_imuvstab -lopencv_tegra -lopencv_vstab -lcufft -lnpps -lnppi -lnppc -lcudart -ltbb -lrt -lpthread -lm -ldl -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfontconfig -lgobject-2.0 -lglib-2.0 -lfreetype -lX11 -o thresh</p>
<p class="p2"><br></p>
<p class="p1">void exportPin(std::string pin){</p>
<p class="p1"><span class="Apple-tab-span">	</span>system(("echo "+pin+" &gt; /sys/class/gpio/export").c_str());</p>
<p class="p1"><span class="Apple-tab-span">	</span>usleep(SECOND/8);</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">void unexportPin(std::string pin){</p>
<p class="p1"><span class="Apple-tab-span">	</span>system(("echo "+pin+" &gt; /sys/class/gpio/unexport").c_str());</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">void setPinValue(std::string pin, std::string val){</p>
<p class="p1"><span class="Apple-tab-span">	</span>system(("echo "+val+" &gt; /sys/class/gpio/gpio" + pin + "/active_low").c_str());</p>
<p class="p1"><span class="Apple-tab-span">	</span>system(("echo "+val+" &gt; /sys/class/gpio/gpio" + pin + "/value").c_str());</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">void setPinDirection(std::string pin, std::string direction){</p>
<p class="p1"><span class="Apple-tab-span">	</span>system(("echo "+direction+" &gt; /sys/class/gpio/gpio" + pin + "/direction").c_str());</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">std::string asInt(int k){</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::stringstream ss;</p>
<p class="p1"><span class="Apple-tab-span">	</span>ss &lt;&lt; k;</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::string str = ss.str();</p>
<p class="p1"><span class="Apple-tab-span">	</span>return str;</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">int getBits(unsigned int *a, int div){</p>
<p class="p1"><span class="Apple-tab-span">	</span>int res=((int)*a)&amp;1;</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout&lt;&lt;*a&lt;&lt;"\t";</p>
<p class="p1"><span class="Apple-tab-span">	</span>*a&gt;&gt;=1;</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(*a &gt; 1024) *a = 0;</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; res &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>return res;</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">void setAnalogOutput0(unsigned int i){</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(i&lt;=255){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("57", OUT); //Pitch</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("57", ON);<span class="Apple-tab-span">	</span>//Pitch</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("221", OUT); //Yaw</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("221", OFF);<span class="Apple-tab-span">	</span>//Yaw</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("222",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("222", asInt(getBits(&amp;i,1)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("219",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("219", asInt(getBits(&amp;i,2)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("216",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("216", asInt(getBits(&amp;i,3)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("226",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("226", asInt(getBits(&amp;i,4)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("81",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("81", asInt(getBits(&amp;i,5)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("160",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("160", asInt(getBits(&amp;i,6)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("161",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("161", asInt(getBits(&amp;i,7)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("163",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("163", asInt(getBits(&amp;i,8)));</p>
<p class="p1"><span class="Apple-tab-span">	</span>}</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">void setAnalogOutput1(unsigned int i){</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(i&lt;=255){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(WDEBUG){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "bits" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("221",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("221", asInt(getBits(&amp;i,2)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("220",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("220", asInt(getBits(&amp;i,3)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("223",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("223", asInt(getBits(&amp;i,4)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("225",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("225", asInt(getBits(&amp;i,5)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("166",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("166", asInt(getBits(&amp;i,6)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("165",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("165", asInt(getBits(&amp;i,7)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("164",OUT);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("164", asInt(getBits(&amp;i,8)));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinDirection("165",OUT);<span class="Apple-converted-space"> </span></p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("165", asInt(getBits(&amp;i,2)));<span class="Apple-converted-space"> </span></p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>usleep(SPEED);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("165", "0");<span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span></p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("221", "0");</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("220", "0");</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("223", "0");</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("225", "0");</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("166", "0");</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("165", "0");</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setPinValue("164", "0");</p>
<p class="p1"><span class="Apple-tab-span">	</span>}</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">void exportPins(){</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "222" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("222");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "219" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("219");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "216" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("216");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "226" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("226");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "81" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("81");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "160" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("160");</p>
<p class="p2"><br></p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "57" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("57");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "221" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("221");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "230" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("220");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "223" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("223");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "166" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("166");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "165" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("165");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "164" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("164");</p>
<p class="p2"><br></p>
<p class="p2"><span class="Apple-tab-span">	</span></p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "161" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("161");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "162" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("162");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "163" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("163");</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(WDEBUG)</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::cout &lt;&lt; "225" &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPin("225");</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">void unexportPins(){</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("222");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("219");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("216");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("226");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("81");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("160");</p>
<p class="p2"><span class="Apple-tab-span">	</span></p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("225");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("163");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("162");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("161");</p>
<p class="p2"><br></p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("57");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("221");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("220");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("223");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("166");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("165");</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPin("164");</p>
<p class="p1">}</p>
<p class="p1">using namespace std;</p>
<p class="p1">using namespace cv;</p>
<p class="p1">CvCapture* cap=cvCaptureFromCAM(CV_CAP_ANY);</p>
<p class="p1">Mat src,dst;</p>
<p class="p1">int it=0;</p>
<p class="p1">vector&lt;vector&lt;Point&gt; &gt; contours;</p>
<p class="p1">bool epilepse=false;</p>
<p class="p1">vector&lt;Vec4i&gt; hierarchy;</p>
<p class="p1">int off=0;</p>
<p class="p1">//Scalar(BLUE,GRREEN,RED)</p>
<p class="p1">int offsret=ALLOW_OFF;</p>
<p class="p1">void removeSmall(Mat* s, int size){</p>
<p class="p1"><span class="Apple-tab-span">	</span>Mat element = getStructuringElement( MORPH_RECT,</p>
<p class="p1"><span class="Apple-converted-space">                                       </span>Size( 2*size + 1, 2*size+1 ),</p>
<p class="p1"><span class="Apple-converted-space">                                       </span>Point( size, size ) );</p>
<p class="p1"><span class="Apple-tab-span">	</span>erode( *s, *s, element );</p>
<p class="p1"><span class="Apple-tab-span">	</span>dilate(*s, *s, element );</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">RNG rng(256);</p>
<p class="p2"><br></p>
<p class="p1">std::string toString(double i){</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::stringstream ss;</p>
<p class="p1"><span class="Apple-tab-span">	</span>ss &lt;&lt; i;</p>
<p class="p1"><span class="Apple-tab-span">	</span>return ss.str();</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">std::string toString(int i){</p>
<p class="p1"><span class="Apple-tab-span">	</span>std::stringstream ss;</p>
<p class="p1"><span class="Apple-tab-span">	</span>ss &lt;&lt; i;</p>
<p class="p1"><span class="Apple-tab-span">	</span>return ss.str();</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">int dist(Point a, Point b){</p>
<p class="p1"><span class="Apple-tab-span">	</span>return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">Point mid(Point a, Point b){</p>
<p class="p1"><span class="Apple-tab-span">	</span>return Point((a.x+b.x)/2,(a.y+b.y)/2);</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">Point lerp(Point a, Point b, double c){</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(c &gt; 1)</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>return b;</p>
<p class="p1"><span class="Apple-tab-span">	</span>if(c &lt; 0)</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>return a;</p>
<p class="p1"><span class="Apple-tab-span">	</span>return Point(a.x*(1-c)+b.x*c,a.y*(1-c)+b.y*c);</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">float sign (Point p1, Point p2, Point p3)</p>
<p class="p1">{</p>
<p class="p1"><span class="Apple-converted-space">    </span>return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">bool PointInTriangle (Point pt, Point v1, Point v2, Point v3)</p>
<p class="p1">{</p>
<p class="p1"><span class="Apple-converted-space">    </span>bool b1, b2, b3;</p>
<p class="p2"><br></p>
<p class="p1"><span class="Apple-converted-space">    </span>b1 = sign(pt, v1, v2) &lt; 0;</p>
<p class="p1"><span class="Apple-converted-space">    </span>b2 = sign(pt, v2, v3) &lt; 0;</p>
<p class="p1"><span class="Apple-converted-space">    </span>b3 = sign(pt, v3, v1) &lt; 0;</p>
<p class="p2"><br></p>
<p class="p1"><span class="Apple-converted-space">    </span>return ((b1 == b2) &amp;&amp; (b2 == b3));</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">bool PointInQuad (Point pt, Point v1, Point v2, Point v3, Point v4)</p>
<p class="p1">{</p>
<p class="p1"><span class="Apple-converted-space">    </span>return PointInTriangle(pt,v1,v2,v3) || PointInTriangle(pt,v1,v3,v4);</p>
<p class="p1">}</p>
<p class="p2"><br></p>
<p class="p1">int main(int argc,char* argv[]){<span class="Apple-tab-span">	</span></p>
<p class="p1"><span class="Apple-tab-span">	</span>exportPins();<span class="Apple-tab-span">	</span></p>
<p class="p1"><span class="Apple-tab-span">	</span>namedWindow("Vidia");</p>
<p class="p1"><span class="Apple-tab-span">	</span>namedWindow("nVidia");</p>
<p class="p1"><span class="Apple-tab-span">	</span>double avg = 0;</p>
<p class="p1"><span class="Apple-tab-span">	</span>double amnt = 0;</p>
<p class="p1"><span class="Apple-tab-span">	</span>Mat element = getStructuringElement( MORPH_RECT,</p>
<p class="p1"><span class="Apple-converted-space">                                       </span>Size( 7, 7 ),</p>
<p class="p1"><span class="Apple-converted-space">                                       </span>Point( 3, 3 ) );</p>
<p class="p1"><span class="Apple-tab-span">	</span>int itl,itr,ibl,ibr;</p>
<p class="p1"><span class="Apple-tab-span">	</span>int ixtl,ixtr,ixbl,ixbr;</p>
<p class="p1"><span class="Apple-tab-span">	</span>Mat colorSrc;</p>
<p class="p1"><span class="Apple-tab-span">	</span>while(true){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>IplImage* img=cvQueryFrame(cap);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>src=cvarrToMat(img);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>colorSrc = src.clone();</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(epilepse)</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>threshold(src,src,0,255,THRESH_BINARY);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>else</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>threshold(src,src,25,255,THRESH_BINARY);//Its magic, you know</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>inRange(src, Scalar(0,0,0), Scalar(255,0,255), src);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>bitwise_not(src,src);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>dst = src.clone();</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>removeSmall(&amp;dst, 2);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int width = src.size().width;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int height = src.size().height;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int widt=width*width+height*height;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>double tlM=widt,trM=widt,blM=widt,brM=widt; <span class="Apple-converted-space">                                                           </span></p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>Point w,br(width, height),tr(width,0),bl(0, height),tl(0,0);<span class="Apple-converted-space">         </span></p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>Point xbr(width, height),xtr(width,0),xbl(0, height),xtl(0,0);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>dilate(dst,dst,element);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>findContours( dst, contours, hierarchy,CV_RETR_LIST, CV_CHAIN_APPROX_TC89_L1 );</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(src,bl,5,Scalar(255,0,0),5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(src,tr,5,Scalar(0,255,0),5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(src,tl,5,Scalar(0,0,255),5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(src,br,5,Scalar(255,255,255),5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>dst = Mat::zeros( dst.size(), CV_8UC3 );</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int finalwidtht, finalwidthb, finalheightl, finalheightr;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>Point finalMid;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>//*</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>for(int x=0;x&lt;contours.size();x++){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );</p>
<p class="p1"><span class="Apple-converted-space">       <span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span></span>drawContours( dst, contours, x, color, 2, 8, hierarchy, 0, Point() );</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>Point lbr(-1,-1),lbl(-1,-1),ltr(-1,-1),ltl(-1,-1);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>double lx = 0, ly = 0;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>double ax = 0, ay = 0;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>for(int y=0;y&lt;contours[x].size();y++){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>w=contours[x][y];</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>lx += w.x;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>ly += w.y;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>ax++;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>ay++;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(dst,w,2,Scalar(128,128,128),2);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int magnitude;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>double ctlM=(pow(w.x-tl.x,2)+pow(w.y-tl.y,2));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>double ctrM=(pow(w.x-tr.x,2)+pow(w.y-tr.y,2));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>double cbrM=(pow(w.x-br.x,2)+pow(w.y-br.y,2));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>double cblM=(pow(w.x-bl.x,2)+pow(w.y-bl.y,2));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>//distance bettwen two points = sqrt(pow(x1-x2,2)+pow(y2-y1,2))</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(ctlM&lt;tlM){<span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span></p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>tlM=ctlM;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>xtl=w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>ltl = w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>itl=y;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>ixtl=x;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}if(ctrM&lt;trM){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>trM=ctrM;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>xtr=w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>ltr = w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>itr=y;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>ixtr=x;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}if(cblM&lt;blM){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>blM=cblM;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>xbl=w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>lbl = w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>ibl=y;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>ixbl=x;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}if(cbrM&lt;brM){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>brM=cbrM;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>xbr=w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>lbr = w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>ibr=y;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>ixbr=x;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>Point middle = Point((lbr.x+lbl.x+ltr.x+ltl.x)/4,(lbr.y+lbl.y+ltr.y+ltl.y)/4);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(src,middle,2,Scalar(128,128,128),2);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int size = 2;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>putText(src, toString(dist(lbr,lbl)), mid(lbr,lbl), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>putText(src, toString(dist(lbl,ltl)), mid(lbl,ltl), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>putText(src, toString(dist(ltl,ltr)), mid(ltr,ltr), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>putText(src, toString(dist(ltr,lbr)), mid(ltr,lbr), FONT_HERSHEY_SIMPLEX, size, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>Point _tl(ltl.x, ltl.y), _tr(contours[x][0].x,contours[x][0].y), _bl(contours[x][0].x,contours[x][0].y), _br(contours[x][0].x,contours[x][0].y);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int _ltl = 0;int _ltr = 0;int _lbl = 0;int _lbr = 0;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>for(int y=0;y&lt;contours[x].size();y++){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>w=contours[x][y];</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(w.x &gt; middle.x){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(w.y &lt; middle.y){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(dist(middle,w) &gt; _ltr){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>_ltr = dist(middle,w);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>_tr = w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(w.y &gt; middle.y){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(dist(middle,w) &gt; _lbr){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>_lbr = dist(middle,w);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>_br = w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(w.x &lt; middle.x){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(w.y &lt; middle.y){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(dist(middle,w) &gt; _ltl){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>_ltl = dist(middle,w);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>_tl = w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(w.y &gt; middle.y){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(dist(middle,w) &gt; _lbl){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>_lbl = dist(middle,w);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>_bl = w;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, _br, _bl, Scalar(255,0,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, _tr, _tl, Scalar(0,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int _width = (_br.x-_bl.x) + (_tr.x-_tl.x);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int play = _width/41*5;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int tape = _width/41*2;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>Point addTL(_tl.x-play, _tl.y-play),addTR(_tr.x+play, _tr.y-play),addBL(_bl.x-play, _bl.y+play),addBR(_br.x+play, _br.y+play);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>Point subTL (_tl.x+play+tape,_tl.y-play),subTR (_tr.x-play-tape,_tr.y-play),subBL (_bl.x+play+tape,_bl.y-play-tape),subBR (_br.x-play-tape,_br.y-tape-play);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>bool positive = true;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>for(int y=0;y&lt;contours[x].size();y++){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>Point w = contours[x][y];</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(!PointInQuad(w,addTL,addTR,addBR,addBL) || PointInQuad(w,subTL,subTR,subBR,subBL)){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>positive = false;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(colorSrc, w, 5, Scalar(0,0,255), 5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(colorSrc, addTL, 5, Scalar(0,0,255), 5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(colorSrc, addTR, 5, Scalar(0,255,255), 5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(colorSrc, addBR, 5, Scalar(0,255,0), 5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(colorSrc, addBL, 5, Scalar(255,255,0), 5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, addTL, subTL, Scalar(0,0,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, subTL, subBL, Scalar(0,0,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, subBL, subBR, Scalar(0,0,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, subBR, subTR, Scalar(0,0,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, subTR, addTR, Scalar(0,0,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, addTR, addBR, Scalar(0,0,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, addBR, addBL, Scalar(0,0,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, addBL, addTL, Scalar(0,0,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>break;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(positive){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, addTL, subTL, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, subTL, subBL, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, subBL, subBR, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, subBR, subTR, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, subTR, addTR, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, addTR, addBR, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, addBR, addBL, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>line(colorSrc, addBL, addTL, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>finalwidtht = _tr.x - _tl.x;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>finalwidthb = _width;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>finalheightl = _bl.x - _tl.x;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>finalheightr = _br.x - _tr.x;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>finalMid = middle;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>double distance = 21834.83/((finalwidthb+finalwidtht)/2);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(positive){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setAnalogOutput1((unsigned int)distance);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>setAnalogOutput1((unsigned int)finalMid.y);</p>
<p class="p2"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span></p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>break;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}//*/</p>
<p class="p2"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span></p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>putText(colorSrc, toString(distance), finalMid, FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>putText(colorSrc, toString((finalwidthb+finalwidtht)/2), Point(finalMid.x,finalMid.y+20), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255,255));</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(src,xbl,5,Scalar(255,0,0),5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(src,xtr,5,Scalar(0,255,0),5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(src,xtl,5,Scalar(0,0,255),5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>circle(src,xbr,5,Scalar(255,255,255),5);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int widthb=xbr.x-xbl.x;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int widtht=xtr.x-xtl.x;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int heightl=xbl.y-xtl.y;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int heightr=xbr.y-xtr.y;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int totalWidth = widthb &gt; widtht ? widtht : widthb;<span class="Apple-tab-span">	</span></p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int totalHeight = heightl &gt; heightr ? heightr : heightl;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>int m = waitKey(10);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>if(m == 32){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>avg += (xbr.x - xbl.x) &lt; 0 ? -(xbr.x - xbl.x) : (xbr.x - xbl.x);</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>amnt++;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}else if(m != -1){</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>break;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>double fina = avg / amnt;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>std::cout &lt;&lt; fina &lt;&lt; std::endl;</p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>imshow("Vidia",dst);<span class="Apple-tab-span">	</span></p>
<p class="p1"><span class="Apple-tab-span">	</span><span class="Apple-tab-span">	</span>imshow("nVidia",colorSrc);</p>
<p class="p1"><span class="Apple-tab-span">	</span>}</p>
<p class="p1"><span class="Apple-tab-span">	</span>unexportPins();</p>
<p class="p1"><span class="Apple-tab-span">	</span>destroyAllWindows();</p>
<p class="p1">}</p>
</body>
</html>
