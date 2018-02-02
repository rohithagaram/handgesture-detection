#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h> 
#include<unistd.h>
#include <X11/X.h> 
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include<math.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
using namespace cv;
using namespace std;

int fLowH=68,fHighH=95,fLowS=83,fHighS=160,fLowV=88,fHighV=174;//First Color green
int sLowH=165,sHighH=179,sLowS=95,sHighS=164,sLowV=39,sHighV=146;//Second Color red
int tLowH=110,tHighH=117,tLowS=165,tHighS=210,tLowV=71,tHighV=207;//Third Color blue
int cLowH=32,cHighH=36,cLowS=84,cHighS=255,cLowV=131,cHighV=211;//fourth Color yellow
void control_first();void control_second();void control_third();void control_fourth();void mouseClick(int button);void mouseClick2(int button);
void pngcp();int count1();void avicp();int count2();
int iAngle = 180,iAngle2=180;
int iScale = 50,iScale2=50;
int iBorderMode = 0;
Mat img;
double cosa=0;
int pscale=-1;
int iImageCenterY = 0;
int iImageCenterX = 0;
int vc=0;
const char* pzRotatedImage = "Rotated Image";

double angle(int pt1x,int pt1y,int pt2x,int pt2y,int pt0x,int pt0y )
{
    double dx1 = pt1x - pt0x;
    double dy1 = pt1y - pt0y;
    double dx2 = pt2x - pt0x;
    double dy2 = pt2y - pt0y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

void CallbackForTrackBar(int, void*)
{
 Mat matRotation = getRotationMatrix2D(  Point( iImageCenterX, iImageCenterY ), (iAngle - 180), iScale / 50.0 );
 
 // Rotate the image
 Mat imgRotated;
 warpAffine( img, imgRotated, matRotation, img.size(), INTER_LINEAR, iBorderMode, Scalar() );

 imshow( pzRotatedImage, imgRotated );
 
}

int dist(int a,int b,int c,int d){
	return sqrt(pow((c-a),2)+pow((d-b),2));
}
void midp(int *a,int* b,int x,int y,int x1,int y1){
	if(x<x1) *a=x+(x1-x)/2;
	else *a=x1+(x-x1)/2;
	if(y<y1) *b=y+(y1-y)/2;
	else *b=y1+(y-y1)/2;
}
int main( int argc, char** argv )
 {

		int color_no=0,mouse_up=0,capture=0,wait,wait2;
		VideoWriter video;
		Display *dpy;
		dpy = XOpenDisplay(0);
		namedWindow(pzRotatedImage, CV_WINDOW_NORMAL);
    		setWindowProperty(pzRotatedImage, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
		VideoCapture cap(1);
		if ( !cap.isOpened() ){
		     cout << "Cannot open the web cam" << endl;
		     return -1;
		}
	 namedWindow("Control-Color", CV_WINDOW_AUTOSIZE); 
	 control_first();
		Mat imgTmp;
		cap.read(imgTmp); 
		Mat imgLines;

    while (true)
    {
        Mat imgOriginal;
        bool bSuccess = cap.read(imgOriginal);
         if (!bSuccess){
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }
      
	  Mat imgHSV;
	  cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
	  Mat imgThresholded,imgThresholded2,imgThresholded3,imgThresholded4;
	  //imshow("Thresholde2", imgHSV); 
	  inRange(imgHSV, Scalar(fLowH, fLowS, fLowV), Scalar(fHighH, fHighS, fHighV), imgThresholded); //Threshold the image
	  inRange(imgHSV, Scalar(sLowH, sLowS, sLowV), Scalar(sHighH, sHighS, sHighV), imgThresholded2); //Threshold the image
	  inRange(imgHSV, Scalar(tLowH, tLowS, tLowV), Scalar(tHighH, tHighS, tHighV), imgThresholded3); //Threshold the image
	  inRange(imgHSV, Scalar(cLowH, cLowS, cLowV), Scalar(cHighH, cHighS, cHighV), imgThresholded4); //Threshold the image
	  //morphological opening (removes small objects from the foreground)
	  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	  erode(imgThresholded3, imgThresholded3, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	  dilate( imgThresholded3, imgThresholded3, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	  erode(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	  dilate( imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		
	  erode(imgThresholded4, imgThresholded4, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	  dilate( imgThresholded4, imgThresholded4, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		  
	  //morphological closing (removes small holes from the foreground)
	  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	  
	  dilate( imgThresholded3, imgThresholded3, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	  erode(imgThresholded3, imgThresholded3, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	  
	  dilate( imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	  erode(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	  
	  dilate( imgThresholded4, imgThresholded4, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	  erode(imgThresholded4, imgThresholded4, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	  
      	  if(color_no==0)
	  	imshow("Thresholded Image", imgThresholded);
	  if(color_no==1)
	  	imshow("Thresholded Image", imgThresholded2);
	  if(color_no==2)
	  	imshow("Thresholded Image", imgThresholded3);
	  if(color_no==3)
	  	imshow("Thresholded Image", imgThresholded4);
	
	  if(color_no==0 && waitKey(1)=='n'){
		  destroyWindow("Control-Color");
		  namedWindow("Control-Color", CV_WINDOW_AUTOSIZE); 
		  control_second();color_no++;
	  }
	   if(color_no==1 && waitKey(1)=='n'){
		  destroyWindow("Control-Color");
		  namedWindow("Control-Color", CV_WINDOW_AUTOSIZE); 
		  control_third();color_no++;
	  }
	  if(color_no==2 && waitKey(1)=='n'){
		  destroyWindow("Control-Color");
		  namedWindow("Control-Color", CV_WINDOW_AUTOSIZE); 
		  control_fourth();color_no++;
	  }

	  if(color_no==3 && waitKey(1)=='n'){

		destroyWindow("Control-Color");color_no++;destroyWindow("Thresholded Image");
      }

	  Moments oMoments = moments(imgThresholded);

  double dM01 = oMoments.m01;
  double dM10 = oMoments.m10;
  double dArea = oMoments.m00;

	Moments oMoments2 = moments(imgThresholded2);

  double dM012 = oMoments2.m01;
  double dM102 = oMoments2.m10;
  double dArea2 = oMoments2.m00;

	Moments oMoments3 = moments(imgThresholded3);

  double dM013 = oMoments3.m01;
  double dM103 = oMoments3.m10;
  double dArea3 = oMoments3.m00;
	Moments oMoments4 = moments(imgThresholded4);

  double dM014 = oMoments4.m01;
  double dM104 = oMoments4.m10;
  double dArea4 = oMoments4.m00;
  int x=dM10/dArea;
  int y=dM01/dArea;
  int x2=dM102/dArea2;
  int y2=dM012/dArea2;
  int x3=dM103/dArea3;
  int y3=dM013/dArea3;
  int x4=dM104/dArea4;
  int y4=dM014/dArea4;
  
 //printf("%f %f %d\n",dArea,dArea2,dist(dM01,dM10,dM012,dM102));
// printf("Areas:::%f %f %f %f \nCoordinates::%d %d %d %d %d %d %d %d\nDist::%d\n",dArea,dArea2,dArea3,dArea4,x,y,x2,y2,x3,y3,x4,y4,dist(x2,y2,x,y));
	if(vc==2) video.write(imgOriginal);
	printf("%d\t %d \n",vc,dist(x3,y3,x4,y4));
	if(capture==1 && dArea2<20000 && dArea3<20000 && dArea<20000 && dArea4<20000){
	pngcp();
	imwrite("hello.png",imgOriginal);capture--;
	wait2=-1;
	img=imread( "hello.png", 1 );
	 iImageCenterY = img.rows / 2;
 	iImageCenterX = img.cols / 2;
	}
	
	if( dArea2>20000 && dArea3>20000 && dArea>20000 && dArea4>20000 ){
		if(dist(x3,y3,x4,y4)<100 && dist(x2,y2,x,y)<100){
			int mx,my,mx1,my1;
			midp(&mx,&my,x3,y3,x4,y4);
			midp(&mx1,&my1,x2,y2,x,y);
			int scale=dist(mx,my,mx1,my1);
			double a=angle(mx1,my,mx1,my1,mx,my);
			
			if(cosa==0) {cosa=a;iAngle2=iAngle;}
			if(pscale==-1) {pscale=scale;iScale2=iScale;}
			if(my1>my) iAngle=iAngle2-(cosa-a)*90;
			else iAngle=iAngle2+(cosa-a)*90;
			iAngle%=360;
			iScale=iScale2-(pscale-scale)/10;
			//printf("----- %d %f %f\n--------------------------------------------------\n",iAngle,a,cosa-a);	 
			int iDummy = 0;
			CallbackForTrackBar(iDummy, &iDummy);
		}
		else{ cosa=0;pscale=-1;wait2=0;}
		if(dist(x3,y3,x,y)<100 && dist(x4,y4,x2,y2)<100){
		capture=1;
		}
	}
	if( dArea2<20000 && dArea3>20000 && dArea<20000 && dArea4>20000 ){
		
		if(vc==0 && dist(x3,y3,x4,y4)<100)
		vc=1;
		if(vc==1 && dist(x3,y3,x4,y4)>150){
		avicp();
		vc=2;video=VideoWriter("out.avi",CV_FOURCC('M','J','P','G'),15, imgOriginal.size(),true);
		//VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),15, imgOriginal.size(),true);
		}
		if(vc==2 && dist(x3,y3,x4,y4)<100)
		vc=3;
		if(vc==3 && dist(x3,y3,x4,y4)>150){
		vc=0;video.release();
	}
		//printf("Video Recording");
	}
	
	
	
		   //rectangle( imgLines,Point( x2-100, y2-100 ),Point( x3+100, y3+100) ,Scalar( 0, 0, 255 ),-1,8);
		
        if(dArea > 160000 || dArea2 > 15000 || dArea3>15000 || dArea4 > 15000){
	  imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
	   
	  if (dArea > 160000)
	  {

	   int posX = dM10 / dArea;
	   int posY = dM01 / dArea;        
	   if(color_no>3 && wait2!=-1){
	   	Window root_window;
		root_window = XRootWindow(dpy, 0);
		XSelectInput(dpy, root_window, KeyReleaseMask);
		XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, int(posX*3), int(posY*2));
		XFlush(dpy);
		}
	   rectangle( imgLines,Point( posX-2, posY-2 ),Point( posX+2, posY+2) ,Scalar( 0, 0, 255 ),-1,8);
	  
	  
	   if(  dArea2>0 && ((dArea2<200000&& dist(x,y,x2,y2) < 40) || (dArea2<800000&& dist(x,y,x2,y2) < 62)  || (dArea2>800000&& dist(x,y,x2,y2) < 120))){
	
		if(mouse_up==0 ){
			int posX = dM10 / dArea;
		  	int posY = dM01 / dArea; 
			if(color_no>3 && wait2!=-1){ 
			    Window root_window;
			    root_window = XRootWindow(dpy, 0);
			    XSelectInput(dpy, root_window, KeyReleaseMask);
			    XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, int(posX*3), int(posY*2));	
			    mouseClick(Button1);
			    printf( "Mouse up:\n");
			    mouse_up=1;
			    XFlush(dpy);
			}
		}
	}
		else if(mouse_up==1){
			mouse_up--;	
			int posX = dM10 / dArea;
	  		int posY = dM01 / dArea; 
			if(color_no>3 && wait2!=-1){ 
			 	 Window root_window;
			   	 root_window = XRootWindow(dpy, 0);
			   	 XSelectInput(dpy, root_window, KeyReleaseMask);
			    	 XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, int(posX*3), int(posY*2));
				 mouseClick2(Button1);
				 printf( "Mouse Down:\n");
			    	 XFlush(dpy);
			}
		    

		}
	}
	  if (dArea2 > 16000)
	  {

	   int posX = dM102 / dArea2;
	   int posY = dM012 / dArea2;        
	   
	   rectangle( imgLines,Point( posX-2, posY-2 ),Point( posX+2, posY+2) ,Scalar( 0, 255, 0 ),-1,8);
	  }
	   if (dArea3 > 16000)
	  {

	   int posX = dM103 / dArea3;
	   int posY = dM013 / dArea3;        
	   
	   rectangle( imgLines,Point( posX-2, posY-2 ),Point( posX+2, posY+2) ,Scalar( 100,100 , 100 ),-1,8);
	  }
	  if (dArea4 > 16000)
	  {

	   int posX = dM104 / dArea4;
	   int posY = dM014 / dArea4;        
	   
	   rectangle( imgLines,Point( posX-2, posY-2 ),Point( posX+2, posY+2) ,Scalar( 0,0 ,255 ),-1,8);
	  }
	  
  }

 
  imgOriginal = imgOriginal + imgLines;
  imshow("Original", imgOriginal); 
  wait=waitKey(1);
	if(wait!=-1) wait2=wait;
        if ( wait== 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break; 
       }
	//else printf("%d\n",wait2);




	 }
     
	 
	 
	 
}
void control_first(){
 createTrackbar("LowH", "Control-Color", &fLowH, 179); createTrackbar("HighH", "Control-Color", &fHighH, 179);
 createTrackbar("LowS", "Control-Color", &fLowS, 255); createTrackbar("HighS", "Control-Color", &fHighS, 255);
 createTrackbar("LowV", "Control-Color", &fLowV, 255);createTrackbar("HighV", "Control-Color", &fHighV, 255);
}
void control_second(){
 createTrackbar("LowH", "Control-Color", &sLowH, 179); createTrackbar("HighH", "Control-Color", &sHighH, 179);
 createTrackbar("LowS", "Control-Color", &sLowS, 255); createTrackbar("HighS", "Control-Color", &sHighS, 255);
 createTrackbar("LowV", "Control-Color", &sLowV, 255);createTrackbar("HighV", "Control-Color", &sHighV, 255);
}
void control_third(){
 createTrackbar("LowH", "Control-Color", &tLowH, 179); createTrackbar("HighH", "Control-Color", &tHighH, 179);
 createTrackbar("LowS", "Control-Color", &tLowS, 255); createTrackbar("HighS", "Control-Color", &tHighS, 255);
 createTrackbar("LowV", "Control-Color", &tLowV, 255);createTrackbar("HighV", "Control-Color", &tHighV, 255);
}
void control_fourth(){
 createTrackbar("LowH", "Control-Color", &cLowH, 179); createTrackbar("HighH", "Control-Color", &cHighH, 179);
 createTrackbar("LowS", "Control-Color", &cLowS, 255); createTrackbar("HighS", "Control-Color", &cHighS, 255);
 createTrackbar("LowV", "Control-Color", &cLowV, 255);createTrackbar("HighV", "Control-Color", &cHighV, 255);
}
void mouseClick2(int button)
{



    Display *display = XOpenDisplay(NULL);

    XEvent event;

    if(display == NULL)
    {
        fprintf(stderr, "Cannot initialize the display\n");
        exit(EXIT_FAILURE);
    }

    memset(&event, 0x00, sizeof(event));

   
    event.type = ButtonRelease;
    event.xbutton.state = 0x100;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;

    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

   event.xbutton.subwindow = event.xbutton.window;

    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;

        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }

  if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

   XFlush(display);

    //usleep(100000);


  

    XCloseDisplay(display);
}
void mouseClick(int button)
{



    Display *display = XOpenDisplay(NULL);

    XEvent event;

    if(display == NULL)
    {
        fprintf(stderr, "Cannot initialize the display\n");
        exit(EXIT_FAILURE);
    }

    memset(&event, 0x00, sizeof(event));

    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;

    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

    event.xbutton.subwindow = event.xbutton.window;

    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;

        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

    XFlush(display);

    //usleep(100000);

    //event.type = ButtonRelease;
    //event.xbutton.state = 0x100;

   // if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

    //XFlush(display);

    XCloseDisplay(display);
}
void pngcp()
{
	int coun=count1();
	char str[100];
	sprintf(str, "%s%d%s", "hello",coun+1,".png");
	char te[100]="gallery/";
	strcat(te,str);
    rename("hello.png", te);
}
void avicp()
{
	int coun=count2();
	char str[100];
	sprintf(str, "%s%d%s", "out",coun+1,".avi");
	char te[100]="gallery_vid/";
	strcat(te,str);
    rename("out.avi", te);
}
int count1()
{
	DIR *dp;
	int i=0;
  	struct dirent *ep;     
  	dp = opendir ("gallery");
	if (dp != NULL)
	{
		while (ep = readdir (dp))
			i++;
	    (void) closedir (dp);
 	}
 	else
    	perror ("Couldn't open the directory");
  	return i-2;
}
int count2()
{
	DIR *dp;
	int i=0;
  	struct dirent *ep;     
  	dp = opendir ("gallery_vid");
	if (dp != NULL)
	{
		while (ep = readdir (dp))
			i++;
	    (void) closedir (dp);
 	}
 	else
    	perror ("Couldn't open the directory");
  	return i-2;
}

