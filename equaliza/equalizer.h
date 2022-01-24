///////// equalização de imagem

#include <iostream>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#ifndef EQUALIZER_H_INCLUDED
#define EQUALIZER_H_INCLUDED


class Equalizer{

    private:
        Mat img_src;
        Mat hist;
        Mat hist_img;
        Mat freq_acum;
        Mat freq_acum_img;
        unsigned level;

        void plot_hist(const string& img_name);
        void plot_FreqAcum(const string& img_name);


    public:
        Equalizer(int l);
        void creat(Mat& img);
        Mat equalization();
        Mat get_hist() { return hist_img; }
        Mat get_freq() { return freq_acum_img; }

};

Equalizer::Equalizer(int l)
{
    this->level = l;
    this->hist = Mat::zeros(l, 1, CV_32FC1);
    this->freq_acum = Mat::zeros(l, 1, CV_32FC1);
}

void Equalizer::creat(Mat& img)
{
    this->img_src  =  img.clone();  // usado em equalization() -> CV::LUT()


    //----- intensidade dos pixels
    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            int pixel  =  img.at<uchar>(i, j);
            this->hist.at<float>(pixel, 0)++;
        }
    }


    //----- calcula frequqncia acumulada
    float soma_acumulada = 0.0;

    for(int i = 0; i < this->hist.rows; i++)
    {
        float valor  =  this->hist.at<float>(i, 0);
        soma_acumulada  +=  valor;
        this->freq_acum.at<float>(i,0)  =  soma_acumulada;   
    }


    //----- normalizaa soma;
    float total_pixel = (float)(img.rows * img.cols);
    this->hist  /=  total_pixel;
    this->freq_acum  /=  total_pixel;


    //----- cria arquivo txt
    ofstream file_hist  =  ofstream("output\\my_hist.txt");
    ofstream file_freq  =  ofstream("output\\my_freq.txt");
    
    for(int i = 0; i < this->hist.rows; i++)
    {
        file_hist << this->hist.at<float>(i, 0) << endl;
        file_freq << this->freq_acum.at<float>(i, 0) << endl;
    }

    file_hist.close();
    file_freq.close();


    //----- plot hist com gnuplot
    this->plot_hist("output\\histograma.png");
    this->plot_FreqAcum("output\\freq_acumu.png");


    //----- armazenanohist_img
    this->hist_img  =  imread("output\\histograma.png");
    this->freq_acum_img = imread("output\\freq_acumu.png");
}

void Equalizer::plot_hist(const string& img_name)
{
    FILE* pipe  =  _popen("gnuplot", "w");

    fprintf(
        pipe,
        "reset session\n"
        "set term pngcairo dashed\n"
        "set style data histograms\n"
        //"set title \'histograma\'\n"
        "set border 3\n"
        "set arrow from 128, graph 0 to 128, graph 1 nohead lw 2.5 lt 2 dt 3\n"
        "set style fill solid 1.0 noborder\n"
    );

    string output = "set out \'" + img_name + "\'\n";
    fprintf(pipe, output.c_str());
    fprintf(pipe, "set xrange[0:%d] \n", this->level-1);
    fprintf(pipe, "plot \'output\\my_hist.txt\' using 1 notitle\n");

    _pclose(pipe);
}
        
void Equalizer::plot_FreqAcum(const string& img_name)
{
    FILE* pipe = _popen("gnuplot", "w");

    fprintf(
        pipe,
        "reset session\n"
        "set term pngcairo dashed\n"
        "set title \'funcao de destribuicao acumulada\'\n"
        "set yrange[0:1]\n"
        "set ytics(1.0, 0.75, 0.5, 0.25, 0.0)\n"
        "set lmargin 9\n"
        "set rmargin 2\n"
        "set grid\n"
    );

    string output = "set out \'"  + img_name +"\'\n";
    fprintf(pipe, output.c_str());

    fprintf(pipe, "set xrange[0:%d]\n", this->level-1);
    fprintf(pipe, "plot \'output\\my_freq.txt\' using 1 notitle with lines\n");
    
    _pclose(pipe);
}

Mat Equalizer::equalization()
{
    Mat tabela  =  Mat(1, this->level, CV_32F);

    for(unsigned i = 0; i < this->level; i++)
    {
        tabela.at<float>(0, i)  =  this->freq_acum.at<float>(0, i) * (255);
        // equalização:  (faixa de intensidade) = (frequencia acumulada) * (intensidade maxima) 
    }

    tabela.convertTo(tabela, CV_8UC1);
    Mat result;
    cv::LUT(this->img_src, tabela, result);

 return result;
}
/*
Mat Equalizer::equalization()
{
    Mat tabela(1, this->level, CV_32F); /// tabela para guardar o resultado da função

    for(unsigned i = 0; i < this->level; i++)
    {
        tabela.at<float>(i)  =  this->freq_acum.at<float>(i) * 255; 
        //formula da integral: (faixa de intensidade) = (frequencia acumulada) * (intensidade maxima)
    }

    tabela.convertTo(tabela, CV_8UC1);
    Mat result;
    cv::LUT(this->img_src, tabela, result);

 return result;
}
*/




#endif