#include "histogrammtab.h"
#include "histogramm.h"
#include "fleximage.h"
#include "controller.h"
#include <iostream>

HistogrammTab::HistogrammTab() : QWidget(){
    histLayout->addWidget(blackHist.get());
    histLayout->addWidget(rHist.get());
    histLayout->addWidget(gHist.get());
    histLayout->addWidget(bHist.get());
    layout->addLayout(histLayout.get());

    cumulHistLayout->addWidget(blackHistCumul.get());
    cumulHistLayout->addWidget(rHistCumul.get());
    cumulHistLayout->addWidget(gHistCumul.get());
    cumulHistLayout->addWidget(bHistCumul.get());
    layout->addLayout(cumulHistLayout.get());
}

void HistogrammTab::updateHistogramms(){
    FlexImage* pic = Controller::instance()->getLastImage();
    std::array<int,256> gray = pic->getGrayScale();
    std::array<int,256> red = pic->getRedScale();
    std::array<int,256> green = pic->getGreenScale();
    std::array<int,256> blue = pic->getBlueScale();

    blackHist->resetWith(gray);
    rHist->resetWith(red);
    gHist->resetWith(green);
    bHist->resetWith(blue);

    blackHistCumul->resetWith(gray);
    rHistCumul->resetWith(red);
    gHistCumul->resetWith(green);
    bHistCumul->resetWith(blue);

}
