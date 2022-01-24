////////////////

#include "equalizer.h"

int main(int argc, const char** argv)
{
    Mat img  =  imread("garota.png");

    string nome[] = { 
        "imagem original", "histograma", "frequencia acumulada",  
        "nova imagem", "novo histograma", "nova frequencia acumulada"
    };


    Equalizer equ  =  Equalizer(256);
    equ.creat( img );
    Mat hist_img  =  equ.get_hist();
    Mat freq_acumulada  =  equ.get_freq();

    Mat equalizado  =  equ.equalization();
    equ.creat( equalizado );
    Mat new_hist  =  equ.get_hist();
    Mat new_freq  =  equ.get_freq();



    namedWindow(nome[0], WINDOW_GUI_NORMAL);
    namedWindow(nome[1], WINDOW_GUI_NORMAL);
    namedWindow(nome[2], WINDOW_GUI_NORMAL);
    
    namedWindow(nome[3], WINDOW_GUI_NORMAL);
    namedWindow(nome[4], WINDOW_GUI_NORMAL);
    namedWindow(nome[5], WINDOW_GUI_NORMAL);



    imshow(nome[0], img);
    imshow(nome[1], hist_img);
    imshow(nome[2], freq_acumulada);

    imshow(nome[3], equalizado);
    imshow(nome[4], new_hist);
    imshow(nome[5], new_freq);


 waitKey();
 return 0;
}