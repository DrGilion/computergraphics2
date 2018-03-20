#include "histogrammausgleich.h"
#include "imageviewer.h"
#include "fleximage.h"
#include "controller.h"

HistogrammAusgleich::HistogrammAusgleich() : FunctionBox("Histogramm ausgleichen"){
    layout->addWidget(loadPictureButton.get());
    QObject::connect(loadPictureButton.get(), QPushButton::clicked, this, HistogrammAusgleich::loadReferencePicture );
    layout->addWidget(referencePicture.get());
    layout->addWidget(linearEqualizationButton.get());
    QObject::connect(linearEqualizationButton.get(), QPushButton::clicked, this, HistogrammAusgleich::linearEqualization );
    layout->addWidget(piecewiseLinearEqualizationButton.get());
    QObject::connect(piecewiseLinearEqualizationButton.get(), QPushButton::clicked, this, HistogrammAusgleich::piecewiseLinearEqualization );
    layout->addWidget(successiveEqualizationButton.get());
    QObject::connect(successiveEqualizationButton.get(), QPushButton::clicked, this, HistogrammAusgleich::successiveEqualization );
}

void HistogrammAusgleich::loadReferencePicture(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath());
    if (!fileName.isEmpty()) {
        referenceImage = new FlexImage(fileName);
        referencePicture->setPixmap(QPixmap::fromImage(referenceImage->scaled(256,128)));
    }
}

void HistogrammAusgleich::linearEqualization(){
    Controller::instance()->linearAusgleich();
}

void HistogrammAusgleich::piecewiseLinearEqualization(){
    Controller::instance()->stueckweiserAusgleich(referenceImage);
}

void HistogrammAusgleich::successiveEqualization(){

}
