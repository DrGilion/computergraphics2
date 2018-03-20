#include <QtGui>

#include <array>
#include <iostream>

#include "histogramm.h"


Histogramm::Histogramm(bool pcumul) : QLabel(), cumul(pcumul){
    init();
}

Histogramm::Histogramm(QRgb pcolor, bool pcumul) : QLabel(),color(pcolor), cumul(pcumul){
    init();
}

Histogramm::Histogramm(std::array<int,256> value, QRgb pcolor, bool pcumul) : QLabel(), color(pcolor), cumul(pcumul){
    init();
    resetWith(value);
}


void Histogramm::drawPlot(){
    drawBlack();

    double factor = 256.0/maxValue;
    for (int x = 0; x < 511; x += 2) {
        for (int y = 0; y < (int)(colorscale[x / 2]*factor); y++) {
            plot->setPixel(x, plot->height() - y - 1, color);
            plot->setPixel(x + 1, plot->height() - y - 1, color);
        }
    }

    refresh();
}

void Histogramm::drawBlack(){
    QRgb black = qRgb(0, 0, 0);
    for (int x = 0; x < 511; x++) {
        for (int y = 0; y < 256; y++) {
            plot->setPixel(x, y, black);
        }
    }
    refresh();
}


void Histogramm::refresh(){
    this->setPixmap(QPixmap::fromImage(*plot));
}

void Histogramm::resetWith(std::array<int,256> value){
    if(cumul){
        //colorscale = new int[256];
        colorscale[0] = value[0];
        for (int i = 1; i < 256; i++) {
            colorscale[i] = value[i] + colorscale[i - 1];
        }
    }else{
        //colorscale = value;
        for (int i = 1; i < 256; i++) {
            colorscale[i] = value[i];
        }
    }
    maxValue = 0;
    for(int i =0;i<256; ++i ){
        if(colorscale[i]> maxValue) maxValue = colorscale[i];
    }
    drawPlot();
}

void Histogramm::init(){
    plot = new QImage(512, 256, QImage::Format_RGB888);
    drawBlack();
}
