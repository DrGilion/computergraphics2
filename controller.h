#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <stack>
#include <string>

#include "fleximage.h"
#include "imageviewer.h"
#include "controller.h"
#include "filter.h"

class ImageViewer;


enum class EdgeAlgorithm : int { ZEROPADDING,CONSTANT,MIRRORED,CYCLIC };

class Controller{
public:
    std::stack<std::unique_ptr<FlexImage>> image_stack;
    std::unique_ptr<ImageViewer> imageviewer;
    Controller();
    static Controller* instance();

    void start();
    FlexImage* getLastImage();
    FlexImage* getSaveCopy();
    void removeImage();
    void removeLastImage();
    void clearImageStorage();
    void pushImage(FlexImage*& image);
    void pushImage();
    void showImage();
    void restorePreviousImage();
    void restoreOriginalImage();

    void log(std::string message);

    //algorithms

    void imageToGray();

    void drawCross(int size, int r, int g, int b);

    void changeBrightness(int brightness);
    void changeContrast(double contrast);
    void automaticContrast(int brightness, double contrast);
    void robustContrast(int slow, int shigh);

    void linearAusgleich();
    void stueckweiserAusgleich(FlexImage* referencePic);

    void filterLinear(Filter matrix, EdgeAlgorithm edge = EdgeAlgorithm::ZEROPADDING);

    void usm_Filter();

    void canny_edge();
    int getOrientationSector(double dx, double dy);
    bool isLocalMax(double** Emag, int x, int y, int sTheta, double tlow);
    void traceAndThreshold(double** Enms, bool**& Ebin,int M, int N, int u0, int v0, double tlow);

    void fillHoughAccumulator();
    void doPixel(int u, int v);
    int aSteps;
    int rSteps;
    int** houghArray;
    int xCtr;
    int yCtr;
    int nAng;
    double dAng;
    int nRad;
    double dRad;
    int minContrast;
    int threshold_blackwhite;
    void detect_Lines();
    bool contrast(int x, int y,int minContrast);
    void accumulate(int x, int y, int delta, FlexImage*& output, int theta, int rScaled);
};

#endif // CONTROLLER_H
