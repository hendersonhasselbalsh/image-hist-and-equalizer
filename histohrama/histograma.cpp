///////

#include "histograma.h"

int main(int argc, const char** argv)
{

    Mat im = imread("carro.jpg");
    Mat layer[3];

    cv::split(im, layer);

    Histograma histo = Histograma(256);

    histo.creat(layer[0]);
    imshow("histograma", histo.get_hist());


 waitKey();
 return 0;
}