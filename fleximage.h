#ifndef FLEXIMAGE_H
#define FLEXIMAGE_H

#include <array>
#include <functional>
#include <qimage.h>
#include "histogramm.h"
#include "filter.h"

class FlexImage : public QImage{
public:
    FlexImage(QString input);
    FlexImage();
    FlexImage(const FlexImage& image) = default;
    FlexImage(int x, int y);

    int getGray(int x, int y);
    int getRed(int x, int y);
    int getGreen(int x, int y);
    int getBlue(int x, int y);

    std::array<int,256> getGrayScale();
    std::array<int,256> getRedScale();
    std::array<int,256> getGreenScale();
    std::array<int,256> getBlueScale();

    std::array<double,256> getRelativeGrayScale();
    std::array<double,256> getRelativeRedScale();
    std::array<double,256> getRelativeGreenScale();
    std::array<double,256> getRelativeBlueScale();

    double calculateGrayFConst();
    double calculateRedFConst();
    double calculateGreenFConst();
    double calculateBlueFConst();

    double calculateGrayVarianz();
    double calculateRedVarianz();
    double calculateGreenVarianz();
    double calculateBlueVarianz();

    void toGray();
    /*FlexImage filterBlackWhite(int threshold = 128);
    FlexImage filterWith(Filter* f);
    FlexImage filterWith(int** filter, int M,int N );
    FlexImage filterWith(double** filter, int M, int N);*/
    void print();
    void drawLine(const QPoint& from, const QPoint& to, QRgb color, int width );
    void setBrightness(int amount = 0);
    void setContrast(double contrast = 1);

    virtual ~FlexImage();

    QRgb forEachRGB(int x, int y, std::function<int(int)> func);
    void forEachPixel(std::function<void(int,int)> func);
    std::array<int,256> getScale(int (FlexImage::*colorfunc)(int x, int y));
    std::array<double,256> getRelativeScale(std::array<int,256> (FlexImage::*scalefunc)(void));
    double calculateFConst(std::array<double,256> (FlexImage::*scalefunc)(void));
    double calculateVarianz(std::array<double,256> (FlexImage::*scalefunc)(void),double fconst);
};

#endif // FLEXIMAGE_H
