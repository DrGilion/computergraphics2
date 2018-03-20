#include <QtGui>
#include <algorithm>
#include <iostream>
#include <functional>
#include "fleximage.h"
#include "utils.h"

FlexImage::FlexImage(QString input): QImage(input){
    //updateHistogramm();
}

FlexImage::FlexImage() : QImage(){
    //updateHistogramm();
}

FlexImage::FlexImage(int x, int y) : QImage(x,y,QImage::Format_RGB888){
    //updateHistogramm();
}

int FlexImage::getGray(int x, int y){
    QRgb rgb = this->pixel(x,y);
    return qGray(rgb);
}

int FlexImage::getRed(int x, int y){
    QRgb rgb = this->pixel(x,y);
    return qRed(rgb);
}

int FlexImage::getGreen(int x, int y){
    QRgb rgb = this->pixel(x,y);
    return qGreen(rgb);
}

int FlexImage::getBlue(int x, int y){
    QRgb rgb = this->pixel(x,y);
    return qBlue(rgb);
}


std::array<int,256> FlexImage::getScale(int (FlexImage::*colorfunc)(int x,int y)){
    std::array<int,256> data{{}};

    this->forEachPixel([&](int x ,int y) -> void{
        int col = Utils::clamp((this->*colorfunc)(x,y));
        data[col]++;
    });
    return data;
}

std::array<double,256> FlexImage::getRelativeScale(std::array<int,256> (FlexImage::*scalefunc)(void)){
    int size = this->width() * this->height();
    std::array<int,256> scale = (this->*scalefunc)();
    std::array<double,256> tmp{{}};
    for(int i = 0; i < 256 ; ++i){
        tmp[i] = (double) scale[i];
    }
    for(int i = 0; i < 256 ; ++i){
        tmp[i] /= size;
    }
    return tmp;
}

std::array<int,256> FlexImage::getGrayScale(){
    return getScale(this->getGray);
}

std::array<int,256> FlexImage::getRedScale(){
    return getScale(this->getRed);
}

std::array<int,256> FlexImage::getGreenScale(){
    return getScale(this->getGreen);
}

std::array<int,256> FlexImage::getBlueScale(){
    return getScale(this->getBlue);
}

std::array<double,256> FlexImage::getRelativeGrayScale(){
    return getRelativeScale(this->getGrayScale);
}

std::array<double,256> FlexImage::getRelativeRedScale(){
    return getRelativeScale(this->getRedScale);
}

std::array<double,256> FlexImage::getRelativeGreenScale(){
    return getRelativeScale(this->getGreenScale);
}

std::array<double,256> FlexImage::getRelativeBlueScale(){
    return getRelativeScale(this->getBlueScale);
}

double FlexImage::calculateGrayFConst(){
    return calculateFConst(this->getRelativeGrayScale);
}

double FlexImage::calculateRedFConst(){
    return calculateFConst(this->getRelativeRedScale);
}

double FlexImage::calculateGreenFConst(){
    return calculateFConst(this->getRelativeGreenScale);
}

double FlexImage::calculateBlueFConst(){
    return calculateFConst(this->getRelativeBlueScale);
}

double FlexImage::calculateGrayVarianz(){
    return calculateVarianz(this->getRelativeGrayScale,this->calculateGrayFConst());
}

double FlexImage::calculateRedVarianz(){
    return calculateVarianz(this->getRelativeRedScale,this->calculateRedFConst());
}

double FlexImage::calculateGreenVarianz(){
    return calculateVarianz(this->getRelativeGreenScale,this->calculateGreenFConst());
}

double FlexImage::calculateBlueVarianz(){
    return calculateVarianz(this->getRelativeBlueScale,this->calculateBlueFConst());
}

double FlexImage::calculateFConst(std::array<double,256> (FlexImage::*scalefunc)(void)){
    std::array<double,256> scale = (this->*scalefunc)();
    double fConst = 0;
    for (int i = 0; i < 256; i++) {
        fConst += scale[i] * i;
    }
    return fConst;
}

double FlexImage::calculateVarianz(std::array<double,256> (FlexImage::*scalefunc)(void),double fconst){
    std::array<double,256> scale = (this->*scalefunc)();
    double varianz = 0;
    for (int i = 0; i < 256; i++) {
        varianz += scale[i] * ((i - fconst) * (i - fconst));
    }
    return varianz;
}

void FlexImage::toGray(){
    this->forEachPixel([&](int x,int y)-> void{
        QRgb rgb = this->pixel(x, y);
        int g = qGray(rgb);
        QRgb grey = qRgb(g, g, g);
        this->setPixel(x, y, grey);
    });
}

/*FlexImage FlexImage::filterBlackWhite(int threshold){
    if(threshold < 0) threshold = 0;
    if(threshold > 255) threshold = 255;
    for (int x = 0; x < this->width(); x++) {
        for (int y = 0; y < this->height(); y++) {
            if(qGray(this->pixel(x,y))<threshold){
                this->setPixel(x,y,qRgb(0,0,0));
            }else{
                this->setPixel(x,y,qRgb(255,255,255));
            }
        }
    }
    return *this;
}

FlexImage FlexImage::filterWith(int **filter, int M, int N){
    double s = 0;
    for(int i=0 ; i<M ; ++i){
        for(int j=0 ; j<N ; ++j){
            s+= abs(filter[i][j]);
        }
    }

    s= 1.0/s;


    int width = this->width();
    int height = this->height();
    int K = M/2;
    int L = N/2;


    FlexImage copy = *this;

    //rechnen
    for(int v=0 ; v<height-1 ; ++v){
        for(int u=0 ; u<width-1 ; ++u){
            //Randfall
            if(u<K || u>(width-K) || v<L || v>(height-L)){
                this->setPixel(u,v,qRgb(128,128,128));
            }else{
                int sum = 0;
                for(int j=-L ; j<=L ; ++j){
                    for(int i=-K ; i<=K ; ++i){
                        QRgb rgb = copy.pixel(u+i, v+j);
                        int g = qGray(rgb);
                        int c = filter[j+L][i+K];
                        sum = sum + c * g;
                    }
                }

                int q = (int) round(s * sum);
                if(q<0) q = 0;
                if(q>255) q = 255;

                QRgb grey = qRgb(q, q, q);
                this->setPixel(u,v,grey);
            }
        }
    }

    return *this;
}

FlexImage FlexImage::filterWith(double **filter, int M, int N){
    double s = 0;
    for(int i=0 ; i<M ; ++i){
        for(int j=0 ; j<N ; ++j){
            s+= abs(filter[i][j]);
        }
    }

    s= 1.0/s;


    int width = this->width();
    int height = this->height();
    int K = M/2;
    int L = N/2;


    FlexImage copy = *this;

    //rechnen
    for(int v=0 ; v<height-1 ; ++v){
        for(int u=0 ; u<width-1 ; ++u){
            //Randfall
            if(u<K || u>=(width-K) || v<L || v>=(height-L)){
                this->setPixel(u,v,qRgb(128,128,128));
            }else{
                int sum = 0;
                for(int j=-L ; j<=L ; ++j){
                    for(int i=-K ; i<=K ; ++i){
                        QRgb rgb = copy.pixel(u+i, v+j);
                        int g = qGray(rgb);
                        double c = filter[j+L][i+K];
                        sum = sum + c * g;
                    }
                }

                int q = (int) round(s * sum);
                if(q<0) q = 0;
                if(q>255) q = 255;

                QRgb grey = qRgb(q, q, q);
                this->setPixel(u,v,grey);
            }
        }
    }

    return *this;
}

FlexImage FlexImage::filterWith(Filter* f){
    return filterWith(f->getMatrix(),f->getRows(),f->getColums());
}*/

void FlexImage::print(){
    this->forEachPixel([&](int x, int y)-> void{
        std::cout<<"Pixel"<<x<<","<<y<<": "<<qGray(this->pixel(x,y))<<std::endl;
    });
}

void FlexImage::drawLine(const QPoint& from, const QPoint& to, QRgb color, int width){
    QPainter painter(this);
    QPen pen(color);
    pen.setWidth(width);
    painter.setPen(pen);
    painter.drawLine(from,to);
}

void FlexImage::setBrightness(int amount){
    //positive Werte => heller, negative Werte => dunkler
    this->forEachPixel([&](int x, int y)-> void{
        QRgb result = this->forEachRGB(x,y,[&](int col)->int{ return Utils::clamp(col+amount); });
        this->setPixel(x,y,result);
    });
}

void FlexImage::setContrast(double contrast){
    //Werte < 1 => Kontrast wird schwächer, Werte > 1 => Kontrast wird Stärker
    this->forEachPixel([&](int x, int y)-> void{
        QRgb result = this->forEachRGB(x,y,[&](int col)->int{ return Utils::clamp(col*contrast); });
        this->setPixel(x,y,result);
    });
}

FlexImage::~FlexImage(){

}

QRgb FlexImage::forEachRGB(int x, int y, std::function<int (int)> func){
    QRgb val = this->pixel(x,y);
    QRgb result = qRgb(func(qRed(val)),func(qGreen(val)),func(qBlue(val)));
    return result;
}

void FlexImage::forEachPixel(std::function<void(int,int)> func){
    for(int i = 0; i<this->width(); ++i){
        for(int j = 0; j<this->height(); ++j){
            func(i,j);
        }
    }
}
