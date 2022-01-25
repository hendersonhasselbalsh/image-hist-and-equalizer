/////////////////  bgr  \  hsv

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


Mat equa_bgr(const Mat& img)
{
    Mat im  =  img.clone();

    vector<Mat> canal, canal_eq;

    cv::split(im, canal);

    for(int i = 0; i < 3; i++)
    {
        Mat aux  =  Mat(im.rows, im.cols, CV_8UC1);
        cv::equalizeHist(canal[i], aux);
        canal_eq.push_back( aux );
    }

    Mat result;
    cv::merge(canal_eq, result);

 return result;
}

Mat equa_hsv(const Mat& img)
{
    Mat im  =  img.clone();
    cv::cvtColor(im, im, COLOR_BGR2HSV);


    vector<Mat> canal;
    cv::split(im, canal);


    cv::equalizeHist(canal[2], canal[2]);


    Mat aux, result;
    cv::merge(canal, aux);

    cvtColor(aux, result, COLOR_HSV2BGR);


 return result;
}



int main(int argc, const char** argv)
{

    Mat img  =  imread("floresta.jpg", IMREAD_UNCHANGED);

    Mat bgr  =  equa_bgr( img );

    Mat hsv  =  equa_hsv( img );


    namedWindow("imagem original", WINDOW_GUI_NORMAL);
    namedWindow("sisntema hsv", WINDOW_GUI_NORMAL);
    namedWindow("sisntema bgr", WINDOW_GUI_NORMAL);


    imshow("imagem original", img);
    imshow("sisntema hsv", hsv);
    imshow("sisntema bgr", bgr);


 waitKey();
 return 0;
}