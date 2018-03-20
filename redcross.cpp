#include "redcross.h"
#include "imageviewer.h"
#include "functionbox.h"
#include "controller.h"

#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>


RedCross::RedCross() : FunctionBox("Buntes Kreuz malen"){
    sizeBox->setRange(0,std::min(Controller::instance()->getLastImage()->height(),Controller::instance()->getLastImage()->width()));
    layout->addRow("Größe: ",sizeBox.get());
    layout->addRow("Rot: ",redSlider.get());
    layout->addRow("Grün: ",greenSlider.get());
    layout->addRow("Blau: ",blueSlider.get());
    layout->addRow(drawButton.get());
    QObject::connect(drawButton.get(), QPushButton::clicked, this, RedCross::drawCross );
}


void RedCross::drawCross(){
    Controller::instance()->drawCross(sizeBox->value(),redSlider->value(),greenSlider->value(),blueSlider->value());
}
