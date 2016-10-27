#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

//#define IMAGE 1
#define CAMERA 1

#ifndef IMAGE
Mat background, output, hsv,hue,sat,val, frame;
Mat lower_red_hue_range, upper_red_hue_range, red_hue_image,gray;
vector<Mat> channels;
vector<cv::Vec3f> circles;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    namedWindow("edges",1);
    for(;;)
    {
        cap >> frame; // get a new frame from camera
        cv::medianBlur(frame, frame, 3);
        cvtColor(frame,hsv,COLOR_BGR2HSV_FULL);
        cvtColor(frame,gray,COLOR_BGR2GRAY);
        inRange(hsv, Scalar(0, 100, 100), Scalar(10, 255, 255), lower_red_hue_range);
        inRange(hsv, Scalar(220, 100, 100),Scalar(255, 255, 255), upper_red_hue_range);
        addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
        //split(hsv,channels);
        //threshold(channels[0],hue,150,255,0);
        //adaptiveThreshold(channels[0], hue, 10, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 3, 5);
        GaussianBlur( red_hue_image, red_hue_image, Size( 9, 9 ), 3, 3 );
        GaussianBlur( gray, gray, Size( 9, 9 ), 3, 3 );
        Canny( gray, gray, 30, 30*3, 3 );
        HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, red_hue_image.rows/8, 100, 20, 0, 300);
        HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, gray.rows/8, 100, 20, 0, 300);
        if(circles.size()!=0){
        	cout << "Found1" << endl;
        }
        int max_rad = 0;
        Point max_pt(0,0);
        for(size_t i = 0; i < circles.size(); i++ ) {
        	Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        	      int radius = cvRound(circles[i][2]);
        	      if(radius > max_rad) {
        	    	  max_rad = radius;
        	    	  max_pt = Point(cvRound(circles[i][0]), cvRound(circles[i][1]));
        	      }
        }
        // circle center
		circle( frame, max_pt, 3, Scalar(0,255,0), -1, 8, 0 );
		// circle outline
		circle( frame, max_pt, max_rad, Scalar(0,0,255), 3, 8, 0 );
        imshow("edges",red_hue_image);
//        imshow("edges1",upper_red_hue_range);

        //applyColorMap(frame, frame, COLORMAP_HSV);
        imshow("edges2",frame);
        imshow("edges3",gray);
        if(waitKey(3) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
#endif

#ifndef CAMERA
Mat lower_red_hue_range, upper_red_hue_range;
Mat img,hsv;
int main(int, char**){
	img = imread("res/circles.jpg");
	cvtColor(img,hsv,COLOR_BGR2HSV_FULL);
	inRange(hsv, Scalar(0, 100, 100), Scalar(10, 255, 255), lower_red_hue_range);
	inRange(hsv, Scalar(240, 100, 100),Scalar(255, 255, 255), upper_red_hue_range);
    imshow("edges",lower_red_hue_range);
    imshow("edges1",upper_red_hue_range);
    waitKey();
    return 0;
}
#endif
