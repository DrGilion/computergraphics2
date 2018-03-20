#include "miscoperations.h"
#include "controller.h"

MiscOperations::MiscOperations() : FunctionBox("Sonstige Operationen"){
    QObject::connect(toGrayButton.get(), QPushButton::clicked, this, MiscOperations::toGray );
    layout->addWidget(toGrayButton.get());
    QObject::connect(invertButton.get(), QPushButton::clicked, this, MiscOperations::invert );
    layout->addWidget(invertButton.get());
}

void MiscOperations::toGray(){
    Controller::instance()->imageToGray();
}

void MiscOperations::invert(){
    //TODO
}
