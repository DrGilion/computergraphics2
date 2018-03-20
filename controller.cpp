#include <memory>
#include <algorithm>
#include <array>
#include <vector>
#include <string>

#include "controller.h"
#include "fleximage.h"
#include "histogramm.h"
#include "imageviewer.h"
#include "utils.h"

//enum class EdgeAlgorithm : int { ZERO_PADDING,CONSTANT,MIRRORED,CYCLIC };

Controller::Controller() : imageviewer(new ImageViewer()) {

}

Controller* Controller::instance(){
    static Controller instance;
    return &instance;
}

void Controller::start(){
    this->imageviewer->show();
}

FlexImage* Controller::getLastImage(){
    return image_stack.top().get();
}

FlexImage *Controller::getSaveCopy(){
    FlexImage* result = new FlexImage(*(image_stack.top().get()));;
    return result;
}

void Controller::removeImage(){
    std::unique_ptr<FlexImage> img(image_stack.top().release());
    image_stack.pop();
    img.reset(nullptr);
}

void Controller::removeLastImage(){
    if(image_stack.size() > 1){
        removeImage();
    }else log("Das ist bereits das letzte Bild!");
}

void Controller::clearImageStorage(){
    while(image_stack.size() > 0){
        removeImage();
    }
    log("Bildspeicher wurde zurückgesetzt!");
}

void Controller::pushImage(FlexImage*& image){
    image_stack.push(std::unique_ptr<FlexImage>(image));
}

void Controller::pushImage(){
    image_stack.push(std::unique_ptr<FlexImage>(new FlexImage(*(imageviewer->image))));
}

void Controller::showImage(){
    imageviewer->image = image_stack.top().get();
    imageviewer->updateImageDisplay();
}

void Controller::restoreOriginalImage(){
    while(image_stack.size() > 1){
        removeImage();
    }
    showImage();
    log("Originalbild wiederhergestellt! ");
}

void Controller::log(std::string message){
    imageviewer->logFile << message.c_str() << std::endl;
    imageviewer->renewLogging();
}

void Controller::restorePreviousImage(){
    removeLastImage();
    showImage();
    log("Vorheriges Bild wiederhergestellt!");
}

void Controller::imageToGray(){
    FlexImage* tmp = getSaveCopy();
    tmp->toGray();
    pushImage(tmp);
    showImage();
    log("Bild ist nun Schwarz-Weiss");
}

void Controller::drawCross(int size, int r, int g, int b){
    FlexImage* tmp = getSaveCopy();
    if (tmp != NULL) {
        QRgb value = qRgb(r, g, b);
        int tmpsize = std::min(tmp->width(),tmp->height());
        for (int i = 0; i < tmpsize; i++) {
            for (int j = 0; j < size; j++) {
                if (i + j < tmpsize) {
                    tmp->setPixel(i + j, i, value);
                    tmp->setPixel(i + j, tmpsize - i - 1, value);
                }
            }
        }

    }

    pushImage(tmp);
    showImage();
    log("Kreuz gemalt");
}

void Controller::changeBrightness(int brightness){
    FlexImage* tmp = getSaveCopy();
    tmp->setBrightness(brightness);
    pushImage(tmp);
    showImage();
    log("Helligkeit wurde geändert");
}

void Controller::changeContrast(double contrast){
    FlexImage* tmp = getSaveCopy();
    tmp->setContrast(contrast);
    pushImage(tmp);
    showImage();
    log("Kontrast wurde geändert");
}

void Controller::automaticContrast(int brightness, double contrast){
    FlexImage* tmp = getSaveCopy();

    int amin = 255;

    tmp->forEachPixel([&](int x, int y){
        QRgb rgb = tmp->pixel(x, y);
        int g = qGray(rgb);
        if (g < amin) {
            amin = g;
        }
    });

    tmp->forEachPixel([&](int x, int y){
        QRgb rgb = tmp->pixel(x, y);
        int g = qGray(rgb) - amin;
        g = g * contrast + brightness + amin;

        g = Utils::clamp(g);
        QRgb grey = qRgb(g, g, g);

        tmp->setPixel(x, y, grey);
    });
    pushImage(tmp);
    showImage();
    log("Automatische Kontrastanpassung wurde durchgeführt");
}

void Controller::robustContrast(int slow, int shigh){
    //Robuste Kontrastanpassung
    FlexImage* tmp = getSaveCopy();

    double alow = 255 * (slow / 100.0);
    double ahigh = 255 * (shigh / 100.0);

    double aminr = 255;
    double amaxr = 0;
    double aming = 255;
    double amaxg = 0;
    double aminb = 255;
    double amaxb = 0;

    tmp->forEachPixel([&](int x, int y){
        QRgb rgb = tmp->pixel(x, y);
        int r = qRed(rgb);
        if (r < aminr) {
            aminr = r;
        }
        if (r > amaxr) {
            amaxr = r;
        }

        int g = qGreen(rgb);
        if (g < aming) {
            aming = g;
        }
        if (g > amaxg) {
            amaxg = g;
        }

        int b = qBlue(rgb);
        if (b < aminb) {
            aminb = b;
        }
        if (b > amaxb) {
            amaxb = b;
        }
    });

    tmp->forEachPixel([&](int x, int y){
        QRgb rgb = tmp->pixel(x, y);

        int r = qRed(rgb);
        r = Utils::clamp(r - std::max(aminr,alow) );
        int rdiff = std::min(ahigh,amaxr) - aminr;
        r = Utils::clamp(r * (255.0 / rdiff));

        int g = qGreen(rgb);
        g = Utils::clamp(g - std::max(aming,alow) );
        int gdiff = std::min(ahigh,amaxg) - aming;
        g = Utils::clamp(g * (255.0 / gdiff));

        int b = qBlue(rgb);
        b = Utils::clamp(b - std::max(aminb,alow) );
        int bdiff = std::min(ahigh,amaxb) - aminb;
        b = Utils::clamp(b * (255.0 / bdiff));

        QRgb color = qRgb(r,g,b);
        tmp->setPixel(x, y, color);
    });
    pushImage(tmp);
    showImage();
    log("Robuste Kontrastanpassung wurde durchgeführt");
}

void Controller::linearAusgleich(){
    FlexImage* tmp = getSaveCopy();
    int pixelcounter = tmp->width() * tmp->height();

    //vector benutzen!!!!!!!!!!
    std::vector<int> imageIn(pixelcounter,0);
    tmp->forEachPixel([&](int x, int y){
        imageIn[y * tmp->width() + x] = qGray(tmp->pixel(x, y));
    });
    std::array<int,256> histogram{{}};
    for (int i = 0; i < pixelcounter; i++) {
        histogram[imageIn[i]]++;
    }

    //build a LookUpTable
    double sum = 0;
    std::array<int,256> lut{{}};
    for (int i = 0; i < 256; i++) {
        sum += histogram[i];
        lut[i] = sum * (255.0 / pixelcounter);
    }
    std::vector<int> imageOut(pixelcounter,0);

    //transform image
    for (int i = 0; i < pixelcounter; i++) {
        imageOut[i] = lut[imageIn[i]];
    }
    tmp->forEachPixel([&](int x, int y){
        QRgb gray = qRgb(imageOut[y * tmp->width() + x], imageOut[y * tmp->width() + x], imageOut[y * tmp->width() + x]);
        tmp->setPixel(x, y, gray);
    });

    pushImage(tmp);
    showImage();
    log("Linearer Histogrammausgleich wurde durchgeführt");
}

void Controller::stueckweiserAusgleich(FlexImage* referencePic){
    if(referencePic != NULL){
        FlexImage* tmp = getSaveCopy();

        /*double pixelcounter = tmpwidth * height;
        double referenzcounter = referenzheight * referenzwidth;

        int referenzCount[(int) referenzcounter];
        int imageIn[(int) pixelcounter];
        int imageHist1[256];
        //int imageHist1Cumul[256];
        createGrayCount(imageHist1, 256);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                imageIn[y * width + x] = qGray(image->pixel(x, y));
                imageHist1[imageIn[y * width + x]]++;
                referenzCount[y * width + x] = qGray(referenzbild->pixel(x, y));
            }
        }

        //createCumulHist(imageHist1, imageHist1Cumul);

        int referenzHist[256];
        int cumulRef[256];

        createGrayCount(referenzHist, 256);
        createGrayCount(cumulRef, 256);

        for (int i = 0; i < referenzcounter; i++) {
            referenzHist[referenzCount[i]]++;
        }
        createCumulHist(referenzHist, cumulRef);

        int histogramm[256];
        createGrayCount(histogramm, 256);

        for (int i = 0; i < pixelcounter; i++) {
            for (int j = 0; j < 256; j++) {
                if (histogramm[j] + imageHist1[j] <= cumulRef[j]) {
                    histogramm[j] += imageHist1[j];
                    cout<<i % width<<", "<<i / width<<endl;
                    image->setPixel(i % width, i / width,
                            qRgb(j, j, j));
                    break;
                }
            }
        }

        int imageCount[(int) pixelcounter];
        int imageHist[256];
        createGrayCount(imageHist, 256);
        int imageRefCumul[256];
        createGrayCount(imageRefCumul,256);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                imageCount[y * width + x] = qGray(image->pixel(x, y));
            }
        }

        for (int i = 0; i < pixelcounter; i++) {
            imageHist[imageCount[i]]++;
        }
        createCumulHist(imageHist, imageRefCumul);

        normieren(cumulRef);
        normieren(imageRefCumul);

        paintItBlack();
        drawPlot1(cumulRef);
        drawPlot2(imageRefCumul);*/

        pushImage(tmp);
        showImage();
        log("Stückweise linearer Histogrammausgleich mit Referenzbild wurde durchgeführt");
    }else{
        log("Referenzbild nicht vorhanden,Bild konnte nicht bearbeitet werden");
    }
}

void Controller::filterLinear(Filter matrix, EdgeAlgorithm edge){
    FlexImage* tmp = getSaveCopy();

    double s = 1.0 / matrix.sum();



    //rechnen
    tmp->forEachPixel([&](int x, int y){
        //Edge
        if(x<2 || x>(tmp->width()-2) || y<2 || y>(tmp->height()-2)){
            switch(edge){
            case EdgeAlgorithm::ZEROPADDING:
                tmp->setPixel(x,y,qRgb(128,128,128));
                break;
            case EdgeAlgorithm::CONSTANT:
                break;
            case EdgeAlgorithm::MIRRORED:
                break;
            case EdgeAlgorithm::CYCLIC:
                break;
            }
        }else{
            int sum = 0;
            for(int i=-2 ; i<2 ; ++i){
                for(int j=-2 ; j<2 ; ++j){
                    QRgb rgb = getLastImage()->pixel(x+i, y+j);
                    int g = qGray(rgb);
                    int c = matrix.get(i+2,j+2);
                    sum = sum + c * g;
                }
            }

            int q = (int) round(s * sum);
            q = Utils::clamp(q);

            QRgb grey = qRgb(q, q, q);
            tmp->setPixel(x,y,grey);
        }
    });

}

void Controller::usm_Filter(){
   /* int gaussradius = gaussradiusField->text().toInt();
    if(gaussradius %2 ==0) gaussradius++;
    double sigma = sigmaField->text().toDouble();


    double alpha = alphaField->text().toDouble();
    //Grenz-Überprüfung 0.2 < alpha < 4
    if(alpha<0.2) alpha = 0.2;
    if(alpha>4.0) alpha = 4.0;

    //Original Gaussfiltern
    FlexImage copy = *image;
    FlexImage gaussBild = *image;
    copy = copy.toGray();
    gaussBild = gaussBild.toGray();
    gaussBild = gaussBild.filterWith(Filter::getGaussFilter(gaussradius,sigma),gaussradius,gaussradius);

    double** maske= new double*[image->width()];
    for(int i = 0; i<image->width(); ++i){
        maske[i] = new double[image->height()];
    }

    //Maske berechnen
    for(int x = 0; x<image->width(); ++x){
        for(int y = 0; y <image->height(); ++y){
            int grauPixel = qGray(copy.pixel(x,y));
            int gaussPixel = qGray(gaussBild.pixel(x,y));
            // M' = alpha * Maske
            double diff = abs(grauPixel - gaussPixel)* alpha;
            maske[x][y] = diff;
        }
    }

    //Ergebniss berechnen I' = I + M'
    for(int x = 0; x<image->width(); ++x){
        for(int y = 0; y <image->height(); ++y){
            int origGray = qGray(image->pixel(x,y));
            double result = origGray+maske[x][y];
            if(result >255.0) result = 255.0;
            int out = (int) result;
            image->setPixel(x,y,qRgb(out,out,out));
        }
    }*/
}

int Controller::getOrientationSector(double dx, double dy){
    /*double cosinus = cos(M_PI/8);
    double sinus = sin(M_PI/8);
    double d_x = cosinus* dx + (-1*sinus*dy);
    double d_y = sinus*dx + cosinus*dy;
    if(d_y <0){
        d_x = -d_x;
        d_y = -d_y;
    }

    if(d_x >=0 && d_x >=d_y) return 0;
    if(d_x >=0 && d_x <d_y) return 1;
    if(d_x <0 && -d_x <d_y) return 2;
    if(d_x <0 && -d_x >=d_y) return 3;


    return -1;
}

bool Controller::isLocalMax(double** Emag, int u, int v, int sTheta, double tlow){
    double mc= Emag[u][v];
    if(mc<tlow) return false;
    else{
        double mL = 0;
        double mR = 0;

        switch(sTheta){
        case 0:
            mL=Emag[u-1][v];
            mR=Emag[u+1][v];
            return (mL<=mc)&&(mc>=mR);
        case 1:
            mL=Emag[u-1][v-1];
            mR=Emag[u+1][v+1];
            return (mL<=mc)&&(mc>=mR);
        case 2:
            mL=Emag[u][v-1];
            mR=Emag[u][v+1];
            return (mL<=mc)&&(mc>=mR);
        case 3:
            mL=Emag[u-1][v+1];
            mR=Emag[u+1][v-1];
            return (mL<=mc)&&(mc>=mR);
        }
    }*/
    return false;
}

void Controller::traceAndThreshold(double** Enms, bool**& Ebin,int M, int N, int u0, int v0, double tlow){
    Ebin[u0][v0] = true;
    double uL= std::max(u0-1,0);
    double uR = std::min(u0+1,M-1);
    double vT = std::max(v0-1,0);
    double vB = std::min(v0+1, N-1);
    for(int u = uL; u<uR ; ++u ){
        for(int v = vT; v<vB; ++v){
            if(Enms[u][v] >= tlow && Ebin[u][v] == 0){
                traceAndThreshold(Enms,Ebin,M,N,u,v,tlow);
            }
        }
    }

}

void Controller::canny_edge(){


    //werte sammeln
   /* int gaussradius = gaussradiusField->text().toInt();
    if(gaussradius%2==0) gaussradius++;
    double Tlow = hysteresisLowField->text().toDouble();
    double Thigh = hysteresisHighField->text().toDouble();
    double sigma = sigmaField->text().toDouble();

    FlexImage grauH = (*image).toGray().filterWith(Filter::getGaussFilter(gaussradius,sigma),gaussradius,gaussradius);
    FlexImage grauV = (*image).toGray().filterWith(Filter::getGaussFilter(gaussradius,sigma),gaussradius,gaussradius);*/
    /*switch(gaussradius){
    case 3:
        grau = grau.filterWith(Filter::Gauss3());
    case 5:
        cout<<"noch nicht implementiert -Micha"<<endl;
        //TODO
    }*/




    //grau.print();

    /*FlexImage horizontal =  grauH.filterWith(Filter::SobelVertical());
    FlexImage vertical =  grauV.filterWith(Filter::SobelHorizontal());

    const int M = grauV.width();
    const int N = grauV.height();

    //create map
    double** Emag = new double*[M];
    for(int i = 0; i< M ; i++){
        Emag[i] = new double[N];
    }

    double** Enms = new double*[M];
    for(int i = 0; i< M ; i++){
        Enms[i] = new double[N];
    }

    bool** Ebin = new bool*[M];
    for(int i = 0; i< M ; i++){
        Ebin[i] = new bool[N];
    }

    for (int u = 0; u < M; u++){
        for (int v = 0; v < N; v++){
            Emag[u][v] = sqrt(   horizontal.getGray(u,v)*horizontal.getGray(u,v) + vertical.getGray(u,v) * vertical.getGray(u,v));
            Enms[u][v] = 0;
            Ebin[u][v] = false;
        }
    }

    for(int u = 1; u<M-2; ++u){
        for(int v = 1; v<N-2; ++v){
            double dx = horizontal.getGray(u,v);
            double dy= vertical.getGray(u,v);
            int sTheta = getOrientationSector(dx,dy);
            if(isLocalMax(Emag,u,v,sTheta,Tlow)){
                Enms[u][v] = Emag[u][v];
            }
        }
    }

    for(int u = 1; u<M-2; ++u){
        for(int v = 1; v<N-2; ++v){
            if((Enms[u][v] >= Thigh) && Ebin[u][v] == 0){
                traceAndThreshold(Enms, Ebin, M, N,u,v, Tlow);
            }
        }
    }


    for(int x = 0; x<M; ++x){
        for(int y = 1; y<N; ++y){
            QRgb color = !Ebin[x][y] ? qRgb(255, 255, 255) : qRgb(0,0,0);
            image->setPixel(x,y,color);
        }
    }*/

}

void Controller::fillHoughAccumulator(){
    /*for(int v = 0; v<image->height() ; ++v){
        for(int u = 0; u<image->width(); ++u){
            if(qGray(image->pixel(u,v)) >0){
                doPixel(u,v);
            }
        }
    }*/
}

void Controller::doPixel(int u, int v){
    int x = u-xCtr;
    int y = v-yCtr;
    for( int a = 0; a < nAng; a++){
        double theta = dAng*a;
        int r = (int) round((x*cos(theta) + y*sin(theta)) / dRad) + nRad/2;
        if(r >= 0 && r <nRad){
            houghArray[a][r]++;
        }
    }
}

void Controller::detect_Lines(){

   /* aSteps = aStepsField->text().toInt();
    rSteps = rStepsField->text().toInt();
    threshold_blackwhite = blackwhiteField->text().toInt();


    xCtr = image->width()/2;
    yCtr = image->height()/2;
    nAng = aSteps;
    dAng = sqrt(M_PI/nAng);
    nRad = rSteps;
    double rMax = sqrt(xCtr*xCtr + yCtr*yCtr);
    dRad = (2*rMax)/nRad;*/
    /*cout<<xCtr<<endl;
    cout<<yCtr<<endl;
    cout<<nAng<<endl;
    cout<<dAng<<endl;
    cout<<nRad<<endl;
    cout<<rMax<<endl;
    cout<<dRad<<endl;
    houghArray = new int*[nAng];
    for( int i = 0; i<nAng; ++i){
        houghArray[i] = new int[nRad];
        for(int j = 0 ; j<nRad ; ++j){
            houghArray[i][j] = 0;
        }
    }
    fillHoughAccumulator();
    //debug code
    for(int x = 0; x<nAng ; ++x){
        for( int y = 0; y<nRad ; ++y){
            cout<<houghArray[x][y]<<" ";
        }
        cout<<endl;
    }
    cout<<"------------------------------------------------------------"<<endl;
    //end of debug

    //paint picture

    //find maximum
    int max =0;
    for(int x = 0; x<nAng ; ++x){
        for( int y = 0; y<nRad ; ++y){
            int tmp =houghArray[x][y];
            if(tmp>max){
                max=tmp;
            }
        }
    }
    FlexImage* houghBild = new FlexImage(nAng,nRad);
    double factor = 255.0/max;
    for(int x = 0; x<nAng ; ++x){
        for( int y = 0; y<nRad ; ++y){
            double tmp = houghArray[x][y]*factor;
            QRgb color = qRgb((int)tmp,(int)tmp,(int)tmp);
            houghBild->setPixel(x,y,color);
        }
    }

    image = houghBild;*/

    //NEU
    /*int thetaAxisSize = aStepsField->text().toInt();
    int rAxisSize = rStepsField->text().toInt();
    int width = image->width();
    int height = image->height();

    minContrast = contrastField->text().toInt();

    int maxRadius =  ceil(hypot(width,height));
    int halfrAxisSize = rAxisSize/2;

    FlexImage *output = new FlexImage(thetaAxisSize,rAxisSize);
    double* sinTable = new double[thetaAxisSize];
    double* cosTable = new double[thetaAxisSize];
    for( int theta = thetaAxisSize-1 ; theta >=0 ; --theta){
        double thetaRad = theta*M_PI/thetaAxisSize;
        sinTable[theta] = sin(thetaRad);
        cosTable[theta] = cos(thetaRad);

    }

    for(int y = height-1 ; y>=0 ; --y){
        for(int x = width-1 ; x>=0 ; --x){
            if(contrast(x,y,minContrast)) {
                for(int theta = thetaAxisSize - 1; theta >= 0; theta--) {
                    double r = cosTable[theta] * x + sinTable[theta] * y;
                    int rScaled = (int) round(r * halfrAxisSize / maxRadius) + halfrAxisSize;
                    accumulate(theta, rScaled, 1, output, theta, rScaled);
                }
            }
        }
    }

    typedef struct  {
        double alpha;
        double radius;
    }Gerade;
    int geradenAnzahl=0;

    //Schwarz/Weiß filtern für geradenbestimmung
    output->filterBlackWhite();

    Gerade* geradenArray = new Gerade[geradenAnzahl];
    int zaehler=0;

    //geraden bestimmen

    for(int x=0;x<output->width();++x){
        for(int y=0; y<output->height();++y){
            int tmp = output->getGray(x,y);
            if(tmp==0){
                double alpha = ((double)x/(double)output->width())*2.0*M_PI;
                geradenArray[zaehler].alpha= alpha;
                double radius =((double)y)/output->height();
                cout<<"radius: "<<radius<<endl;
                geradenArray[zaehler].radius=radius;
                zaehler++;
            }
        }
    }
    Gerade* normalform = new Gerade[geradenAnzahl];
    for(int i=0;i<geradenAnzahl;i++){
        normalform[i].alpha=-(cos(geradenArray[i].alpha)/sin(geradenArray[i].alpha));
        normalform[i].radius=geradenArray[i].radius/sin(geradenArray[i].alpha);
    }
    //Geraden in Original zeichnen
    cout<<"-----------------------------------##################################"<<endl;
    cout<<"-----------------------------------##################################"<<endl;
    cout<<"-----------------------------------##################################"<<endl;
    for(int p=0;p<geradenAnzahl;p++){
        int x=0;
        int y=0;
        double m = normalform[p].alpha;
        int n = normalform[p].radius;
        //cout<<m<<" "<<n<<endl;
        QRgb lineColor = qRgb(255,100,255);
        while(x<image->width()){
            y=(int)(m*x+n);
            if(y>0&&y<image->height()){
                image->setPixel(x,y,lineColor);
            }
            //cout<<x<<"|"<<y<<endl;
            x++;


        }
    }*/


    //image = output;
    //image->drawLine(QPoint(30,30),QPoint(150,200));
}


bool Controller::contrast(int x, int y,int minContrast){
   /* int centerValue = image->getGray(x, y);
    for(int i = 8; i >= 0; i--) {
        if(i == 4) {
            continue;
        }
        int newx = x + (i % 3) - 1;
        int newy = y + (i / 3) - 1;
        if((newx < 0) || (newx >= image->width()) || (newy < 0) || (newy >= image->height())) {
            continue;
        }
        if(abs(image->getGray(newx, newy)) - centerValue >= minContrast) {
            return true;
        }
    }*/
    return false;
}

void Controller::accumulate(int x, int y, int delta, FlexImage*& output, int theta, int rScaled) {
    int val = output->getGray(x, y) + delta;
    QRgb color = qRgb(val, val, val);
    output->setPixel(theta, rScaled, color);
}
