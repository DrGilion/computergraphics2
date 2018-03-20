#include "pictureinfo.h"
#include "imageviewer.h"
#include <string>
#include <sstream>

PictureInfo::PictureInfo() : FunctionBox("Helligkeit und Kontrast einstellen"){
    layout->addRow("Helligkeit: ",brightSlider.get());
    QObject::connect(brightButton.get(), QPushButton::clicked, this, PictureInfo::applyBrightness );
    layout->addRow(brightButton.get());

    contrastSlider->setValue(100);
    contrastLayout->addRow("Kontrast: ",contrastSlider.get());
    QObject::connect(contrastButton.get(), QPushButton::clicked, this, PictureInfo::applyContrast );
    contrastLayout->addRow(contrastButton.get());
    QObject::connect(autoContrastButton.get(), QPushButton::clicked, this, PictureInfo::applyAutomaticContrast );
    //contrastLayout->addRow(autoContrastButton.get());
    secondaryLayout->addLayout(contrastLayout.get());

    slow->setMinimum(0);
    shigh->setMinimum(0);
    slow->setMaximum(100);
    shigh->setMaximum(100);
    robustContrastLayout->addRow("slow: ",slow.get());
    shigh->setValue(100);
    robustContrastLayout->addRow("shigh: ",shigh.get());
    robustContrastLayout->addRow(robustContrastButton.get());
    QObject::connect(robustContrastButton.get(), QPushButton::clicked, this, PictureInfo::applyRobustContrast );
    secondaryLayout->addLayout(robustContrastLayout.get());

    layout->addRow(secondaryLayout.get());
}

void PictureInfo::applyBrightness(){
    Controller::instance()->changeBrightness(brightSlider->value());
}

void PictureInfo::applyContrast(){
    Controller::instance()->changeContrast(contrastSlider->value()/100.0);
}

void PictureInfo::applyAutomaticContrast(){
    Controller::instance()->automaticContrast(brightSlider->value(),contrastSlider->value()/100.0);
}

void PictureInfo::applyRobustContrast(){
    Controller::instance()->robustContrast(slow->value(),shigh->value());
}


