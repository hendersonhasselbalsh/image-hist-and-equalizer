//////////////

#include <iostream>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#ifndef HISTOGRMA_H_INCLUDED
#define HISTOGRMA_H_INCLUDED


class Histograma{

    private:
        Mat hist;
        Mat hist_img;
        unsigned level;
        void plot_hist(string nome);
        
    public:
        Histograma(unsigned l);
        void creat(Mat img);
        Mat equalizer();
        Mat get_hist() { return hist_img; }

};

Histograma::Histograma(unsigned l)
{
    this->level = l;    this->hist = Mat::zeros(l, 1, CV_32F);
}

void Histograma::plot_hist(string nome)
{
    FILE* pipe = _popen("gnuplot", "w");

    fprintf(
        pipe,
        "reset session\n"
        "set term pngcairo\n"
        "set style data histograms\n"
        "set border 3\n"
        "set arrow from 128, graph 0 to 128, graph 1 lw 1.5 lt 2 dt 3\n"
        "set style fill solid 1.0 noborder\n" 
    );

    string output = "set out \'" + nome + "\'\n";
    fprintf(pipe, output.c_str());
    fprintf(pipe, "set xrange[0:%d]\n", this->level-1);
    fprintf(pipe, "plot \'histograma.txt\' using 1 notitle\n");

    _pclose(pipe);
}

void Histograma::creat(Mat img)
{
    //----- conta as intensidade dos pixels
    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            int pixel = img.at<uchar>(i, j);
            this->hist.at<float>(pixel, 0)++;
        }
    }

    this->hist /= (img.rows * img.cols);


    //----- cria arquivo txt para armazenar os numeros do histograma
    ofstream file_hist = ofstream("histograma.txt");

    for(unsigned i = 0; i < this->level; i++)
    {
        file_hist << this->hist.at<float>(i, 0) << endl;
    }
    
    file_hist.close();
    //----- plotar grafico;
    this->plot_hist("histograma.png");

    //----- carregar a imagem
    this->hist_img = imread("histograma.png");

}









#endif