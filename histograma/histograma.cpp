///////

#include "histograma.h"

int main(int argc, const char** argv)
{
    string nome[] = {"output\\blue.png","output\\red.png", "output\\green.png"};

    Mat im = imread("carro.jpg");
    Mat layer[3];

    cv::split(im, layer);

    Histograma histo = Histograma(256);


    for(int i = 0; i < 3; i++)
    {
        histo.creat(layer[i], nome[i]);
        imwrite(nome[i], histo.get_hist() );
    }


 return 0;
}